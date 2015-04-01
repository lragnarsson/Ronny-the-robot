/*
 * control_unit.c
 */
#include "control_unit.h"

/* Initialize all hardware ports */
void init_ports() {
	TCCR0A = (0<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0); // PWM for left and right engine
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00); // PWM for left and right engine
	DDRB = (1<<DDB0)|(1<<DDB1)|(1<<DDB3)|(1<<DDB4);	// Left and right engine speed and direction as output
	DDRD = 0<<DDD1; // Input, reset button
}

/* Set current speed to predefined value and update control_speed_max */
void set_current_speed(uint8_t speed) {
	current_speed = speed;
	control_speed_max = 0.49 * speed;
}

/* Initialize starting conditions for the robot */
void init_state() {
	current_mode = AUTONOMOUS;
	current_task = SEARCH;
	current_direction = NORTH;
	set_current_speed(MAPPING_SPEED);
}

/* Stop engines by settings PWM duty cycles to 0. Direction unaffected */
void stop_engines() {
	ENGINE_LEFT_SPEED =	ENGINE_RIGHT_SPEED = 0;
}

/* Absolute value of linear approximation of left "angle" derivative */
uint8_t get_angle_change_left() {
	if (current_angle_left > last_tick_angle_left)
		return inverse_sampling_speed * (current_angle_left - last_tick_angle_left);
	return inverse_sampling_speed * (last_tick_angle_left - current_angle_left);
}

/* Absolute value of linear approximation of right "angle" derivative */
uint8_t get_angle_change_right() {
	if (current_angle_right > last_tick_angle_right)
		return inverse_sampling_speed * (current_angle_right - last_tick_angle_right);
	return inverse_sampling_speed * (last_tick_angle_right - current_angle_right);
}

/* Calculates the control_speed based on current angle and error in distance to the middle of the corridor */
void calculate_control_speed() {
	uint16_t control_speed_16 = P_COEFFICIENT * current_distance_error + D_COEFFICIENT * current_angle;
	if (control_speed_16 > control_speed_max)
		control_speed = control_speed_max;
	else
		control_speed = (uint8_t) control_speed_16;
}

/* Drive forward until distance_remaining is 0 or until entering a corridor from a crossroad-section. */
void step_forward() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = current_speed;
	while (distance_remaining != 0) {
		--distance_remaining;
		if (get_angle_change_left() > ANGLE_CHANGE_THRESHOLD || get_angle_change_right() > ANGLE_CHANGE_THRESHOLD)
			return; // Reentered the corridor
		_delay_ms(2);
	}
	stop_engines();
}

/* Drive forward until distance_remaining is 0. Will temporarily switch to step_forward to turn off sensor feedback if passing a crossroad. */
void drive_forward() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	while (distance_remaining != 0) {
		calculate_control_speed();
		ENGINE_LEFT_SPEED = current_speed - control_speed;
		ENGINE_RIGHT_SPEED = current_speed + control_speed;
		--distance_remaining; // Should be controlled by wheel encoders
		if (get_angle_change_left() > ANGLE_CHANGE_THRESHOLD || get_angle_change_right() > ANGLE_CHANGE_THRESHOLD)
			step_forward(); // Entered a crossroad section (turn off sensor feedback temporarily)
		_delay_ms(2);
	}
	stop_engines();
}

/* Drive forward and map surroundings until reaching a wall. Will temporarily switch to step_forward to turn off sensor feedback if passing a crossroad. */
void drive_and_map() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	while (distance_remaining != 0) {
		calculate_control_speed();
		ENGINE_LEFT_SPEED = current_speed - control_speed;
		ENGINE_RIGHT_SPEED = current_speed + control_speed;
		--distance_remaining; // Should be controlled by wheel encoders
		if (get_angle_change_left() > ANGLE_CHANGE_THRESHOLD || get_angle_change_right() > ANGLE_CHANGE_THRESHOLD)
		step_forward(); // Entered a crossroad section (turn off sensor feedback temporarily)
		_delay_ms(2);
	}
	stop_engines();
}

/* Rotate 90 degrees to the left based on wheel encoder feedback */
void rotate_left_90() {
	angle_remaining = 90;
	PORTB = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = TURN_SPEED;
	while(angle_remaining != 0) {
		_delay_ms(3);
		-- angle_remaining; // simulering
	}
	--current_direction;
	current_direction &= 3;
	stop_engines();	
}

/* Rotate 90 degrees to the right based on wheel encoder feedback */
void rotate_right_90() {
	angle_remaining = 90;
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = TURN_SPEED;
	while(angle_remaining != 0) {
		_delay_ms(3);
		-- angle_remaining; // simulering
	}
	++current_direction;
	current_direction &= 3;
	stop_engines();	
}

/* Rotate 180 degrees based on wheel encoder feedback */
void rotate_180() {
	angle_remaining = 180;
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = TURN_SPEED;
	while(angle_remaining != 0) {
		_delay_ms(3);
		-- angle_remaining; // simulering
	}
	current_direction += 2;
	current_direction &= 3;
	stop_engines();
}

/* Creates and sets current_route based on map data */
void create_route() {
	
}

/* Contains all logic for mapping and searching through the map */
void search_state() {
	set_current_speed(MAPPING_SPEED);
	route_index = 0;
	current_task = RETRIEVE;
}

/* Ronny is going to retrieve the package but wants to make sure he knows the shortest path first */
void retrieve_state() {
	set_current_speed(SUPER_SPEED);
	know_shortest_path = 1;
}

/* Ronny is back at the start to pick up the package (in a shady way) */
void grab_package_state() {
	route_index = 0;
	current_task = DELIVER;	
}

/* Drops the package and positions Ronny to avoid running it over later */
void drop_package_state() {
	route_index = 0;
	current_task = RETURN;	
}

/* Ronny has reached the goal and does a victory dance */
void end_state() {
	
}

/* Follows the current path until the end and then selects state transition */
state_function navigate() {
	direction next_direction = current_route[route_index];
	/* Follow current path */
	while(next_direction != ROUTE_END) {
		distance_remaining = 0;
		/* Calculate straight line distance remaining in current direction */
		do {
			distance_remaining += 100;
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

int main(void) {
	init_ports();
	init_state();

	state_function state_transition;
	// ## TEMP_COMMENT ## Will currently follow the predefined path 4 times in a row since no new path is ever calculated for next task.
	do {
		state_transition = navigate();
		(*state_transition)();
	} while (current_task != IDLE);
	
	stop_engines();
}