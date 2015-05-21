/*
 * control_unit.c
 * main() function for the control unit and high level logic state machine.
 */
#include "control_unit.h"
#include "flood_fill.h"


int main(void)
{
	next_state = enter_map_state;
	init_map();
	init_bus_communication();
	init_control_system();
	DDRD |= (1<<DDD1)|(1<<DDD2); // LED output
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	sei();
	
	while (PIND & (1<<PIND0))
	{
		_delay_ms(1);
	}
	
	if (PIND & (1<<PIND3))
	{
		current_mode = AUTONOMOUS;
		i2c_write_byte(COMMUNICATION_UNIT, AUTONOMOUS_MODE);
	}
	else
	{
		current_mode = MANUAL;
		i2c_write_byte(COMMUNICATION_UNIT, MANUAL_MODE);
	}
	
	_delay_ms(100);
	
	while (1) {
		if(current_mode == MANUAL)
			manual_mode();
		else if(current_mode == AUTONOMOUS)
			autonomous_mode();
		else if (current_mode == TEST)
			test_mode();
	}
}

/* Autonomous mode */
void autonomous_mode()
{
	while (current_mode == AUTONOMOUS)
	{
		next_state();
	}
}

/* Test mode */
void test_mode()
{

}

/* Enter map state */
void enter_map_state()
{
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	set_desired_engine_speed(MAPPING_SPEED);
	int16_t last_distance_travelled = distance_travelled;
	
	for (;;)
	{
		int16_t square_diff = distance_travelled - last_distance_travelled;
		_delay_ms(1);
		
		if (square_diff > 400 || (square_diff > 100 && front_wall_distance < 230))
			break;
	}
	set_desired_engine_speed(0);
	tape_square = 0;
	next_state = search_state;
}

/* Search state */
void search_state()
{
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	PORTD |= (1<<DEBUG_LED_GREEN)|(0<<DEBUG_LED_RED);
	
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	_delay_ms(500);
	
	/* Handle unawareness of initial surroundings */
	set_walls(wall_front, wall_left, wall_right);
	set_current_sqaure_not_wall();
	
	// We might end up in tape square directly from navigate
	if (tape_square)
	{
		set_desired_engine_speed(0);
		
		goal_position = current_position;
		next_state = return_state;
		
		return;
	}
	
	// Or we might end up in an intersection...
	if (wall_front)
	{
		if (!wall_left)
			rotate_left_90();
		else if (!wall_right)
			rotate_right_90();
		else
		{
			set_desired_engine_speed(0);
			
			flood_fill_to_unmapped();
			if (current_route[0] == ROUTE_END) // no route found
			{
				next_state = end_state;
			}
			else
			{
				next_state = navigate_state;
				follow_up_state = search_state;
			}
			
			return;
		}
	}
	
	/* Start searching */
	int16_t last_distance_travelled = distance_travelled;
	
	for (;;)
	{
		int16_t sqaure_diff = distance_travelled - last_distance_travelled;
		
		if (intersection)
			set_desired_engine_speed(INTERSECTION_SPEED);
		else
			set_desired_engine_speed(MAPPING_SPEED);
		
		_delay_ms(1);
		
		if (sqaure_diff > 400 || (sqaure_diff > 100 && front_wall_distance < 230))
		{
			//PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
			is_sending = 0;
			
			/* Update map stuff */
			move_map_position_forward();
			set_walls(wall_front, wall_left, wall_right);
			
			/* Decide what to do */
			if (tape_square)
			{
				set_desired_engine_speed(0);
				set_current_sqaure_not_wall();
				
				goal_position = current_position;
				next_state = return_state;
				
				return;
			}
			else if (current_sqaure_not_wall())
			{
				set_desired_engine_speed(0);
				
				flood_fill_to_unmapped();	
				if (current_route[0] == ROUTE_END) // no route found
				{
					next_state = end_state;
				}
				else
				{
					next_state = navigate_state;
					follow_up_state = search_state;
				}
				
				return;
			}
			else
			{
				set_current_sqaure_not_wall();
				
				if (wall_front)
				{
					if (!wall_left)
						rotate_left_90();
					else if (!wall_right)
						rotate_right_90();
					else
					{
						set_desired_engine_speed(0);
						
						flood_fill_to_unmapped();
						if (current_route[0] == ROUTE_END) // no route found
						{
							next_state = end_state;
						}
						else
						{
							next_state = navigate_state;
							follow_up_state = search_state;
						}
						
						return;
					}
				}
			}
			
			/* Reset */
			last_distance_travelled = distance_travelled;
		} // end if
	} // end for
}

void navigate_state()
{
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	PORTD |= (0<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
	
	set_desired_engine_speed(0);
	_delay_ms(500);
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	_delay_ms(500);
	
	uint8_t route_index = 0;
	direction next_direction = current_route[route_index];
	
	while (next_direction != ROUTE_END)
	{
		/* Decide what turn to make based on current direction and next direction in route */
		uint8_t next_turn = (current_direction - next_direction) & 3;
		switch (next_turn)
		{
			case LEFT:
				rotate_left_90();
				break;
			case RIGHT:
				rotate_right_90();
				break;
			case BACKWARD:
				rotate_180();
				break;
			default:
				break;
		}
		
		/* Drive one square */
		int16_t last_distance_travelled = distance_travelled;
		
		for (;;)
		{
			int16_t square_diff = distance_travelled - last_distance_travelled;
			
			if (intersection)
				set_desired_engine_speed(INTERSECTION_SPEED);
			else
				set_desired_engine_speed(SUPER_SPEED);
				
			_delay_ms(1);
			
			if (square_diff > 400 || (square_diff > 100 && front_wall_distance < 230))
			{
				//PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
				is_sending = 0;
				move_map_position_forward();
				break;
			}
		}
		
		next_direction = current_route[++route_index];
	} // end while
	
	current_route[0] = ROUTE_END;
	next_state = follow_up_state;
	set_desired_engine_speed(0);
}

/* Return state */
void return_state()
{
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	PORTD |= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
	
	set_desired_engine_speed(0);
	_delay_ms(500);
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	_delay_ms(500);
	
	// Do we already know the best route?
	flood_fill_home_optimistic(current_position);
	uint8_t optimistic_distance_from_goal = ff_map[start_position.x][start_position.y];
	flood_fill_to_destination(start_position);
	uint8_t known_distance = ff_map[start_position.x][start_position.y];
	
	if(known_distance <= optimistic_distance_from_goal)
	{
		next_state = navigate_state;
		follow_up_state = grab_package_state;
		tape_square = 0;
		current_route[0] = ROUTE_END;
		
		return;
	}
	
	
	while (!(current_position.x == START_POSITION_X && current_position.y == START_POSITION_Y))
	{
		flood_fill_home_optimistic(goal_position);
		optimistic_distance_from_goal = ff_map[start_position.x][start_position.y];
		
		flood_fill_to_destination(goal_position);
		uint8_t distance_to_goal = ff_map[goal_position.x][goal_position.y];
		
		flood_fill_home_optimistic(current_position);
		uint8_t optimistic_distance_from_current_position = ff_map[start_position.x][start_position.y];
		
		// Have we moved in the wrong direction? Then start over from goal position.
		if (optimistic_distance_from_goal - distance_to_goal < optimistic_distance_from_current_position)
		{
			flood_fill_to_destination(goal_position);
		}
		
		direction next_direction = current_route[0];
		
		/* Decide what turn to make based on current direction and next direction in route */
		uint8_t next_turn = (current_direction - next_direction) & 3;
		switch (next_turn)
		{
			case LEFT:
				rotate_left_90();
				break;
			case RIGHT:
				rotate_right_90();
				break;
			case BACKWARD:
				rotate_180();
				break;
			default:
				break;
		}
		
		/* Drive forward one square and map */
		int16_t last_distance_travelled = distance_travelled;
		
		set_desired_engine_speed(MAPPING_SPEED);
		
		for (;;)
		{
			int16_t square_diff = distance_travelled - last_distance_travelled;
			
			if (intersection)
				set_desired_engine_speed(INTERSECTION_SPEED);
			else
				set_desired_engine_speed(MAPPING_SPEED);
			
			_delay_ms(1);
			
			if (square_diff > 400 || (square_diff > 100 && front_wall_distance < 230))
			{
				//PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
				is_sending = 0;
				
				/* Update map stuff */
				move_map_position_forward();
				set_walls(wall_front, wall_left, wall_right);
				set_current_sqaure_not_wall();
				
				/* Decide what to do */
				break;
			}
		}
	}
	
	set_desired_engine_speed(0);
	
	tape_square = 0;
	current_route[0] = ROUTE_END;
	next_state = grab_package_state;
}

/* Grab package state */
void grab_package_state()
{
	uint8_t next_turn = (current_direction - SOUTH) & 3;
	switch (next_turn)
	{
		case LEFT:
			rotate_left_90();
			break;
		case RIGHT:
			rotate_right_90();
			break;
		case BACKWARD:
			rotate_180();
			break;
		default:
			break;
	}

	open_claw();
	_delay_ms(250);
	
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	set_desired_engine_speed(MAPPING_SPEED);
	int16_t last_distance_travelled = distance_travelled;
	int16_t square_diff = distance_travelled - last_distance_travelled;
	
	for (;;)
	{
		square_diff = distance_travelled - last_distance_travelled;
		_delay_ms(1);
		
		if (square_diff > 400)
			break;
	}
	
	set_desired_engine_speed(0);

	_delay_ms(500);
	close_claw();
	_delay_ms(250);
	rotate_180();
	_delay_ms(250);
	
	last_distance_travelled = distance_travelled;
	set_desired_engine_speed(MAPPING_SPEED);
	for (;;)
	{
		square_diff = distance_travelled - last_distance_travelled;
		_delay_ms(1);
		
		if (square_diff > 400)
			break;
	}
	set_desired_engine_speed(0);
	
	flood_fill_to_destination(goal_position);

	next_state = navigate_state;
	follow_up_state = drop_package_state;
}

/* Drop package state */
void drop_package_state()
{
	_delay_ms(500);
	force_engine_direction(ENGINE_DIRECTION_BACKWARD);
	set_desired_engine_speed(MAPPING_SPEED);
	int16_t last_distance_travelled = distance_travelled;
	
	for (;;)
	{
		int16_t square_diff = distance_travelled - last_distance_travelled;

		_delay_ms(1);
		
		if (square_diff < -100)
			break;
	}
	
	set_desired_engine_speed(0);

	open_claw();
	_delay_ms(250);

	last_distance_travelled = distance_travelled;
	set_desired_engine_speed(MAPPING_SPEED);
	for (;;)
	{
		int16_t square_diff = distance_travelled - last_distance_travelled;

		_delay_ms(1);
		
		if (square_diff < -250)
			break;
	}

	set_desired_engine_speed(0);

	close_claw();
	_delay_ms(250);

	current_direction = (current_direction + 2) & 3;
	move_map_position_forward();
	current_direction = (current_direction + 2) & 3;

	flood_fill_to_destination(start_position);

	next_state = navigate_state;
	follow_up_state = end_state;
}

/* End state */
void end_state()
{
	set_desired_engine_speed(0);
	
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	PORTD |= (1<<DEBUG_LED_GREEN)|(0<<DEBUG_LED_RED);
	
	uint8_t next_turn = (current_direction - SOUTH) & 3;
	switch (next_turn)
	{
		case LEFT:
		rotate_left_90();
		break;
		case RIGHT:
		rotate_right_90();
		break;
		case BACKWARD:
		rotate_180();
		break;
		default:
		break;
	}

	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	set_desired_engine_speed(MAPPING_SPEED);
	int16_t last_distance_travelled = distance_travelled;
	int16_t square_diff = distance_travelled - last_distance_travelled;

	for (;;)
	{
		square_diff = distance_travelled - last_distance_travelled;
		_delay_ms(1);
		
		if (square_diff > 400)
		break;
	}
	set_desired_engine_speed(0);
	_delay_ms(200);

	while (1)
	{
		PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
		_delay_ms(200);
	}
}

/* Rotate left 90 degrees */
void rotate_left_90()
{
	set_desired_engine_speed(0);
	_delay_ms(250);
	force_engine_direction(ENGINE_DIRECTION_LEFT);
	_delay_ms(250);
	
	int16_t last_absolute_rotation = absolute_rotation;
	force_engine_speed(TURN_SPEED);
	
	while (absolute_rotation - last_absolute_rotation < 85)
	{
		_delay_ms(1);
	}
	
	current_direction = (current_direction + 3) & 3;
	
	force_engine_speed(0);
	_delay_ms(250);
	force_engine_direction(ENGINE_DIRECTION_FORWARD);
	_delay_ms(250);
}

/* Rotate right 90 degrees */
void rotate_right_90()
{
	set_desired_engine_speed(0);
	_delay_ms(250);
	force_engine_direction(ENGINE_DIRECTION_RIGHT);
	_delay_ms(250);
	
	int16_t last_absolute_rotation = absolute_rotation;
	force_engine_speed(TURN_SPEED);
	
	while (absolute_rotation - last_absolute_rotation > -85)
	{
		_delay_ms(1);
	}
	
	current_direction = (current_direction + 1) & 3;
	
	force_engine_speed(0);
	_delay_ms(250);
	force_engine_direction(ENGINE_DIRECTION_FORWARD);
	_delay_ms(250);
}

/* Rotate left or right 180 degrees */
void rotate_180()
{
	set_desired_engine_speed(0);
	
	int16_t last_absolute_rotation = absolute_rotation;
	
	if (left_wall_distance < right_wall_distance)
	{
		_delay_ms(250);
		force_engine_direction(ENGINE_DIRECTION_RIGHT);
		_delay_ms(250);
		force_engine_speed(TURN_SPEED);
		while (absolute_rotation - last_absolute_rotation > -200)
		{
			_delay_ms(1);
		}
	}
	else
	{
		_delay_ms(250);
		force_engine_direction(ENGINE_DIRECTION_LEFT);
		_delay_ms(250);
		force_engine_speed(TURN_SPEED);
		while (absolute_rotation - last_absolute_rotation < 200)
		{
			_delay_ms(1);
		}
	}
	
	current_direction = (current_direction + 2) & 3;
	
	force_engine_speed(0);
	_delay_ms(250);
	force_engine_direction(ENGINE_DIRECTION_FORWARD);
	_delay_ms(250);
}

/* Manual mode */
void manual_mode()
{
	while (current_mode == MANUAL)
	{
		last_manual_command_updated = 0;
		
		switch (last_manual_command)
		{
			case DO_NOTHING:
			force_engine_speed(0);
			break;
			case M_FORWARD:
			{
				force_engine_direction(ENGINE_DIRECTION_FORWARD);
				set_manual_forward_engine_speed();
				
				int16_t last_distance_travelled = distance_travelled;
				while (distance_travelled - last_distance_travelled < 50)
				{
					_delay_ms(1);
				}
				
				if (!last_manual_command_updated)
				last_manual_command = DO_NOTHING;
				
				break;
			}
			case M_BACKWARD:
			{
				force_engine_direction(ENGINE_DIRECTION_BACKWARD);
				set_manual_forward_engine_speed();
				
				int16_t last_distance_travelled = distance_travelled;
				while (distance_travelled - last_distance_travelled > -50)
				{
					_delay_ms(1);
				}
				
				if (!last_manual_command_updated)
				last_manual_command = DO_NOTHING;
				
				break;
			}
			case M_LEFT:
			{
				force_engine_direction(ENGINE_DIRECTION_LEFT);
				set_manual_turn_engine_speed();
				
				int16_t last_absolute_rotation = absolute_rotation;
				while (absolute_rotation - last_absolute_rotation < 20)
				{
					_delay_ms(1);
				}
				
				if (!last_manual_command_updated)
				last_manual_command = DO_NOTHING;
				
				break;
			}
			case M_RIGHT:
			{
				force_engine_direction(ENGINE_DIRECTION_RIGHT);
				set_manual_turn_engine_speed();
				
				int16_t last_absolute_rotation = absolute_rotation;
				while (absolute_rotation - last_absolute_rotation > -20)
				{
					_delay_ms(1);
				}
				
				if (!last_manual_command_updated)
				last_manual_command = DO_NOTHING;
				
				break;
			}
			case M_FORWARD_LEFT:
			{
				force_engine_direction(ENGINE_DIRECTION_FORWARD);
				set_manual_forward_left_engine_speed();
				
				int16_t last_distance_travelled = distance_travelled;
				while (distance_travelled - last_distance_travelled < 50)
				{
					_delay_ms(1);
				}
				
				if (!last_manual_command_updated)
				last_manual_command = DO_NOTHING;
				
				break;
			}
			case M_FORWARD_RIGHT:
			{
				force_engine_direction(ENGINE_DIRECTION_FORWARD);
				set_manual_forward_right_engine_speed();
				
				int16_t last_distance_travelled = distance_travelled;
				while (distance_travelled - last_distance_travelled < 50)
				{
					_delay_ms(1);
				}
				
				if (!last_manual_command_updated)
				last_manual_command = DO_NOTHING;
				
				break;
			}
			default:
			break;
		}
	}
}


/* Drive forward until distance_remaining is 0. Will temporarily switch to step_forward to turn off sensor feedback if passing a crossroad. 
void drive_forward()
{
	square_distance_remaining = 400;
	uint8_t has_moved_map_position_forward = 0;
	while (distance_remaining) //|| !has_moved_map_position_forward)
	{
		has_moved_map_position_forward = 0;
		
		set_desired_speed(state_speed);
		set_controlled_engine_speed();

		if (square_distance_remaining < 200 && front_wall_distance < 200)
			distance_remaining = square_distance_remaining = 0;

		if (square_distance_remaining == 0) {
			move_map_position_forward();
			square_distance_remaining = 400;
			has_moved_map_position_forward = 1;
		}
	}
	
	stop_engines();
}*/

/* Rotate 90 degrees to the left based on wheel encoder feedback 
void rotate_left_90() {
	stop_engines();
	_delay_ms(500);
	angle_remaining = 90;
	PORTB = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(TURN_SPEED);

	while(angle_remaining != 0) {
		set_same_engine_speed();
	}

	--current_direction;
	current_direction &= 3;
	stop_engines();
	_delay_ms(500);
}*/

/* Rotate 90 degrees to the right based on wheel encoder feedback
void rotate_right_90() {
	stop_engines();
	_delay_ms(500);
	angle_remaining = -90;
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	set_desired_speed(TURN_SPEED);

	while(angle_remaining != 0)
		set_same_engine_speed();

	++current_direction;
	current_direction &= 3;
	stop_engines();
	_delay_ms(500);
} */

/* Rotate 180 degrees based on wheel encoder feedback
void rotate_180() {
	stop_engines();
	_delay_ms(500);
	if (left_wall_distance < right_wall_distance)
	{
		angle_remaining = -190; // because of reasons
		PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	}
	else
	{
		angle_remaining = 190; // because of reasons
		PORTB = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	}
	
	set_desired_speed(TURN_SPEED);

	while(angle_remaining != 0)
		set_same_engine_speed();

	current_direction += 2;
	current_direction &= 3;
	stop_engines();
	_delay_ms(500);
} */
/*
uint8_t map_surroundings(uint8_t turn) {
	if(left_wall_distance < 250)
		set_wall_left();

	if(right_wall_distance < 250)
		set_wall_right();

	if(front_wall_distance < 250) {
		set_wall_front();
		
		if(!turn)
			return 1;

		if (!is_wall_left())
			rotate_left_90();
		else if(!is_wall_right())
			rotate_right_90();
		else
			return 0;
	}

	return 1;
}*/

/* Drive forward and map surroundings until reaching a wall. Will temporarily switch to step_forward to turn off sensor feedback if passing a crossroad. 
uint8_t drive_and_map() {
	distance_remaining = 10000;
	square_distance_remaining = 400;
	uint8_t find_closest_unmapped = 0;
	
	set_desired_speed(state_speed);
	if (!map_surroundings(1))
		return goal_found;

	while (goal_found != 1) {
		set_desired_speed(state_speed);
		set_controlled_engine_speed();

		if (square_distance_remaining < 200 && front_wall_distance < 200)
			square_distance_remaining = distance_remaining = 0;

		if (square_distance_remaining == 0) { // Ronny is in the middle of the next square, update map.
			distance_remaining = 10000;
			square_distance_remaining = 400;
			find_closest_unmapped = move_map_position_forward();
			find_closest_unmapped |= !map_surroundings(1);
			if (find_closest_unmapped)  // Update map coordinates  and update map with surrounding walls and rotate if needed, else return 0 to navigate to closest unmapped
			{
				uint8_t msg[2] = {0xFF, 0xFF};
				i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
				stop_engines();
				return goal_found;
			}
		}
	}

	map_surroundings(0);
	stop_engines();
	return goal_found;
}*/

/* Contains all logic for mapping and searching through the map 
void search_state() { 
	
	
	
	
	
	
	
	if(drive_and_map()) { //The goal was found
		current_task = RETRIEVE;
		state_speed = SUPER_SPEED;
	} else
		flood_fill_to_unmapped();// calculate route to closest unmapped square
		
}*/

/* Ronny is back at the start to pick up the package (in a shady way) 
void grab_package_state() {
	close_claw();
	current_task = DELIVER;
	state_speed = SUPER_SPEED;
	flood_fill_to_destination(goal_position);
}*/

/* Ronny is going to retrieve the package but wants to make sure he knows the shortest path first 
void retrieve_state() {
	uint8_t route_index = 0;
	direction next_direction = current_route[route_index];
	
	while(!(current_position.x = START_POSITION_X && current_position.y == START_POSITION_Y)) 
	{
		map_surroundings(0);
		flood_fill_home_optimistic();
		
		/* Decide what turn to make based on current direction and next direction in route 
		uint8_t next_turn = (current_direction - next_direction) & 3;
		switch (next_turn) {
			case LEFT:
				rotate_left_90();
				break;
			case RIGHT:
				rotate_right_90();
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
	
	current_route[0] = ROUTE_END;
	grab_package_state();
}*/

/* Drops the package and positions Ronny to avoid running it over later 
void drop_package_state() {
	open_claw();
	current_task = RETURN;
	state_speed = SUPER_SPEED;
	flood_fill_to_destination(start_position);
}*/

/* Ronny has reached the goal and does a victory dance 
void end_state() {
	
}*/

/* Follows the current path until the end and then selects state transition 
state_function navigate() {
	uint8_t route_index = 0;
  	direction next_direction = current_route[route_index];
	  
	/* Follow current path 
	while(next_direction != ROUTE_END) {
		distance_remaining = 0;
		/* Calculate straight line distance remaining in current direction
		do {
			distance_remaining += 400;
			++route_index;
		} while (next_direction == current_route[route_index]);
		
		/* Decide what turn to make based on current direction and next direction in route
		uint8_t next_turn = (current_direction - next_direction) & 3;
		switch (next_turn) {
			case LEFT:
				rotate_left_90();
				break;
			case RIGHT:
				rotate_right_90();
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
	
	current_route[0] = ROUTE_END;
	/* Figure out the correct state transition
	state_function state_transition;
	switch (current_task) {
		case SEARCH:
			state_transition = &search_state; // Navigated to a un-mapped area, keep searching.
			break;
		case RETRIEVE:
			//state_transition = &retrieve_state;  // Find the shortest path home
			state_transition = &end_state;
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
}*/
/*
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
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_BACKWARD:
				PORTB = (0<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
				state_speed = MAPPING_SPEED;
				set_desired_speed(state_speed);
				while(distance_remaining != 0) {
					set_same_engine_speed();
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_LEFT:
				PORTB = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
				state_speed = TURN_SPEED;
				set_desired_speed(state_speed);
				while(angle_remaining != 0) {
					set_same_engine_speed();
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_RIGHT:
				PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
				state_speed = TURN_SPEED;
				set_desired_speed(state_speed);
				while(angle_remaining != 0) {
					set_same_engine_speed();
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_FORWARD_LEFT:
				PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
				state_speed = MAPPING_SPEED;
				set_desired_speed(state_speed);
				while(distance_remaining != 0) {
					set_manual_forward_left_engine_speed();
				}
				last_manual_command = DO_NOTHING;
				break;
			case M_FORWARD_RIGHT:
				PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
				state_speed = MAPPING_SPEED;
				set_desired_speed(state_speed);
				while(distance_remaining != 0) {
					set_manual_forward_right_engine_speed();
				}
				last_manual_command = DO_NOTHING;
				break;
			default:
				break;
		}
		stop_engines();
	}
}*/

/*
void test_mode()
{

	distance_remaining = 10000;
	drive_forward();
	_delay_ms(2000);
	//current_mode = AUTONOMOUS;
	//i2c_write_byte(GENERAL_CALL, TAPE_FOUND);
}
*/