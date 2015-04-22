/*
 * control_unit.c
 * main() function for the control unit and high level logic state machine.
 */
#include "control_unit.h"
#include "flood_fill.h"
uint8_t state_speed = 0;
uint8_t know_shortest_path = 0;

/* Initialize starting conditions for the robot */
void init_state() {
	current_mode = TEST;
	current_task = SEARCH;
	set_current_direction(NORTH);
	state_speed = MAPPING_SPEED;
}

/* Initialize all hardware ports */
void initialize_control_unit() {
	init_map();
	init_bus_communication();
	init_control_system();
	init_state();
}

/* Drive forward until distance_remaining is 0 or until entering a corridor from a crossroad-section. */
void step_forward() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(state_speed);
	while (distance_remaining != 0) {
		set_same_engine_speed();
		//--distance_remaining;
		if(current_task==SEARCH && square_distance_remaining == 0)
			return; // In the middle of a crossroad section during search
		if (corner_detected_left() || corner_detected_right())
			return; // Reentered the corridor
		_delay_ms(5);
	}
	stop_engines();
}

/* Drive forward until distance_remaining is 0. Will temporarily switch to step_forward to turn off sensor feedback if passing a crossroad. */
void drive_forward() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(state_speed);
	while (distance_remaining != 0) {
		set_controlled_engine_speed();
		//--distance_remaining; // Should be controlled by wheel encoders
		if (corner_detected_left() || corner_detected_right())
			step_forward(); // Entered a crossroad section (turn off sensor feedback temporarily)
		_delay_ms(5);
	}
	stop_engines();
}

/* Rotate 90 degrees to the left based on wheel encoder feedback */
void rotate_left_90() {
	angle_remaining = 90;
	PORTB = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(TURN_SPEED);
	while(angle_remaining != 0) {
		set_same_engine_speed();
		_delay_ms(3);
		//--angle_remaining; // simulering
	}
	--current_direction;
	current_direction &= 3;
	stop_engines();	
}

/* Rotate 90 degrees to the right based on wheel encoder feedback */
void rotate_right_90() {
	angle_remaining = 90;
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(TURN_SPEED);
	while(angle_remaining != 0) {
		set_same_engine_speed();
		_delay_ms(3);
		//--angle_remaining; // simulering
	}
	++current_direction;
	current_direction &= 3;
	stop_engines();	
}

/* Rotate 180 degrees based on wheel encoder feedback */
void rotate_180() {
	angle_remaining = 180;
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(TURN_SPEED);
	while(angle_remaining != 0) {
		set_same_engine_speed();
		_delay_ms(5);
		//--angle_remaining; // simulering
	}
	current_direction += 2;
	current_direction &= 3;
	stop_engines();
}

/* Drive forward and map surroundings until reaching a wall. Will temporarily switch to step_forward to turn off sensor feedback if passing a crossroad. */
uint8_t drive_and_map() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	distance_remaining = 1000;
	square_distance_remaining = 40;
	set_desired_speed(state_speed);
	while (goal_found != 1) {
		set_controlled_engine_speed();
		--distance_remaining; // Should be controlled by wheel encoders
		if (square_distance_remaining == 0) { // Ronny is in the middle of the next square, update map.
			move_map_position_forward(); // Update map coordinates
			if(left_wall_distance < 30)
				set_wall_left();
			if(right_wall_distance < 30)
				set_wall_right();
			if(front_wall_distance < 30) {
				set_wall_front();
				if (!is_wall_left())
					rotate_left_90();
				else if(!is_wall_right())
					rotate_right_90();
				else
					return 0;
			}
			distance_remaining = 1000;
			square_distance_remaining = 40;
		}
		if (corner_detected_left() || corner_detected_right())
			step_forward(); // Entered a crossroad section (turn off sensor feedback temporarily)
		_delay_ms(5); //TEMP
	}
	return 1;
}

/* Creates and sets current_route based on map data */
void create_route() {
	
}

/* Contains all logic for mapping and searching through the map */
void search_state() { 
	if(drive_and_map()) { //The goal was found
		distance_remaining = 200;
		step_forward();
		move_map_position_forward(); // Update map coordinates
		if(left_wall_distance < 30)
			set_wall_left();
		if(right_wall_distance < 30)
			set_wall_right();
		if(front_wall_distance < 30)
			set_wall_front();
		//TEMP COMMENT: calculate route to start
		current_task = RETRIEVE;
		state_speed = SUPER_SPEED;
	} else {
		//TEMP COMMENT: calculate route to closest unmapped square
	}
}

/* Ronny is going to retrieve the package but wants to make sure he knows the shortest path first */
void retrieve_state() {

	know_shortest_path = 1;
}

/* Ronny is back at the start to pick up the package (in a shady way) */
void grab_package_state() {
	current_task = DELIVER;
	state_speed = SUPER_SPEED;
}

/* Drops the package and positions Ronny to avoid running it over later */
void drop_package_state() {
	current_task = RETURN;
	state_speed = SUPER_SPEED;
}

/* Ronny has reached the goal and does a victory dance */
void end_state() {
	
}

/* Follows the current path until the end and then selects state transition */
state_function navigate() {
	uint8_t route_index = 0;
	direction next_direction = current_route[route_index];
	/* Follow current path */
	while(next_direction != ROUTE_END) {
		distance_remaining = 0;
		/* Calculate straight line distance remaining in current direction */
		do {
			distance_remaining += 400;
			++route_index;
		} while (next_direction == current_route[route_index]);
		/* Decide what turn to make based on current direction and next direction in route */
		uint8_t next_turn = (current_direction - next_direction) & 3;
		switch (next_turn) {
			case LEFT:
				rotate_left_90();
				step_forward();
				break;
			case RIGHT:
				rotate_right_90();
				step_forward();
				break;
			case BACKWARD:
				rotate_180();
				break;
			default:
				break;
		}
		drive_forward();
		next_direction = current_route[route_index];
	}
	/* Figure out the correct state transition*/
	state_function state_transition;
	switch (current_task) {
		case SEARCH:
			state_transition = &search_state; // Navigated to a un-mapped area, keep searching.
			break;
		case RETRIEVE:
			if (know_shortest_path)
				state_transition = &grab_package_state;  // The last route was the shortest path to the start - grab package.
			else
				state_transition = &retrieve_state;  // The shortest path between the goal and start is not yet known, the last route was to an unmapped area - keep looking for shortest path.
			break;
		case DELIVER:
			state_transition = &drop_package_state; // The last route was from start to the goal - drop the package.
			break;
		case RETURN:
			state_transition = &end_state; // The last route was from goal to start - do a victory dance!
			break;
		default:
			state_transition = &end_state; // Should never happen.
			break;
	}
	return state_transition;
}

void manual_drive() {
	while(current_mode == MANUAL) {
		switch(last_manual_command) {
			case DO_NOTHING:
				break;
			case M_FORWARD:
				PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
				state_speed = MAPPING_SPEED;
				set_desired_speed(state_speed);
				while(distance_remaining != 0) {
					set_same_engine_speed();
					//--distance_remaining; //simulation
					//_delay_ms(10);
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_BACKWARD:
				PORTB = (0<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
				state_speed = MAPPING_SPEED;
				set_desired_speed(state_speed);
				while(distance_remaining != 0) {
					set_same_engine_speed();
					//--distance_remaining;
					//_delay_ms(10);
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_LEFT:
				PORTB = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
				state_speed = TURN_SPEED;
				set_desired_speed(state_speed);
				while(angle_remaining != 0) {
					set_same_engine_speed();
					//--angle_remaining;
					//_delay_ms(10);
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_RIGHT:
				PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
				state_speed = TURN_SPEED;
				set_desired_speed(state_speed);
				while(angle_remaining != 0) {
					set_same_engine_speed();
					//--angle_remaining;
					//_delay_ms(10);
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_FORWARD_LEFT:
				PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
				state_speed = MAPPING_SPEED;
				set_desired_speed(state_speed);
				while(distance_remaining != 0) {
					set_manual_forward_left_engine_speed();
					//--distance_remaining;
					//_delay_ms(10);
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_FORWARD_RIGHT:
				PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
				state_speed = MAPPING_SPEED;
				set_desired_speed(state_speed);
				while(distance_remaining != 0) {
					set_manual_forward_right_engine_speed();
					//--distance_remaining;
					//_delay_ms(10);
				}
				last_manual_command = DO_NOTHING;
				break;
			default:
				break;
		}
		stop_engines();
	}
}

void autonomous_drive() {
	state_function state_transition;
	// ## TEMP_COMMENT ## Will currently follow the predefined path 4 times in a row since no new path is ever calculated for next task.
	do {
		state_transition = navigate();
		(*state_transition)();
	} while (current_task != IDLE);
	stop_engines();
	while(current_mode == AUTONOMOUS)
		_delay_ms(25);
}

void test_mode()
{
	//flood_fill_to_destination((coordinate){12, 15});
	//navigate();
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(state_speed);
	while (distance_remaining != 0) {
		set_controlled_engine_speed();
		//--distance_remaining; // Should be controlled by wheel encoders
		/*if (corner_detected_left() || corner_detected_right())
		step_forward(); // Entered a crossroad section (turn off sensor feedback temporarily)
		_delay_ms(5);*/
	}
	stop_engines();
}

int main(void) {
	initialize_control_unit();
	sei();
	
	_delay_ms(2000);
	distance_remaining = 500; //TEST
	
	while (1) {
		if(current_mode == MANUAL)
			manual_drive();
		else if(current_mode == AUTONOMOUS)
			autonomous_drive();
		else if (current_mode == TEST)
		{
			test_mode();
		}
		_delay_ms(5);
	}
}