/* 
 * control_unit.c
 * Ronny-the-robot/control_unit
 * ------------------------------
 * This file is the entry point an backbone for the control unit. 
 * The state machine for the completion of the mission is implemented here. 
 * ------------------------------ 
 * Author: L. Ragnarsson, E. Sköld
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

/*
 * Runs the state machine.
 * next_state is a variable containing a function pointer.
 */
void autonomous_mode()
{
	while (current_mode == AUTONOMOUS)
	{
		next_state();
	}
}

/*
 * For development purposes 
 */
void test_mode()
{

}

/*
 * drive one square forward to enter the maze.
 */
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

/*
 * Searches the maze until the goal is found.
 */
void search_state()
{
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	PORTD |= (1<<DEBUG_LED_GREEN)|(0<<DEBUG_LED_RED);
	
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	
	/* Handle unawareness of initial surroundings */
	set_walls(wall_front, wall_left, wall_right);
	set_current_sqaure_not_wall();
	
	/* We might end up in tape square directly from navigate */
	if (tape_square)
	{
		set_desired_engine_speed(0);
		
		uint8_t msg[] = { MAPPED_GOAL, current_position.x, current_position.y};
		i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
		_delay_ms(10);
		
		goal_position = current_position;
		next_state = return_state;
		
		return;
	}
	
	/* Or we might end up in an intersection... */
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
			if (current_route[0] == ROUTE_END)
			{
				/* No route found */
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
			PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
			is_sending = 0;
			
			/* Update map stuff */
			move_map_position_forward();
			set_walls(wall_front, wall_left, wall_right);
			
			/* Decide what to do */
			if (tape_square)
			{
				set_desired_engine_speed(0);
				set_current_sqaure_not_wall();
				
				uint8_t msg[] = { MAPPED_GOAL, current_position.x, current_position.y};
				i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
				_delay_ms(10);
				
				goal_position = current_position;
				next_state = return_state;
				
				return;
			}
			else if (current_sqaure_not_wall())
			{
				set_desired_engine_speed(0);
				
				flood_fill_to_unmapped();	
				if (current_route[0] == ROUTE_END)
				{
					/* No route found */
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
						if (current_route[0] == ROUTE_END)
						{
							/* No route found */
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

/*
 * Follows a route defined from the variable current_route.
 */
void navigate_state()
{
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	PORTD |= (0<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
	
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	
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
		uint8_t corner_correction = 0;
		
		for (;;)
		{
			int16_t square_diff = distance_travelled - last_distance_travelled;
			if (!corner_correction && intersection && square_diff > 130)
			{
				int8_t square_diff_error = 190 - square_diff;
				last_distance_travelled -= square_diff_error;
				square_diff = distance_travelled - last_distance_travelled;
				corner_correction = 1;
			}
			
			set_desired_engine_speed(MAPPING_SPEED);
			
			_delay_ms(1);
			
			if (square_diff > 400 || (square_diff > 100 && front_wall_distance < 230))
			{
				PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
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

/*
 * When the goal id found, this state explores the maze (if necessary) to ensure the
 * shortets path between start and goal is known and then returns to the start of the maze.
 */
void return_state()
{
	PORTD &= ~((1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED));
	PORTD |= (1<<DEBUG_LED_GREEN)|(0<<DEBUG_LED_RED);
	
	set_desired_engine_direction(ENGINE_DIRECTION_FORWARD);
	
	/* Do we already know the best route? */
	flood_fill_home_optimistic(current_position);
	uint8_t optimistic_distance_from_goal = ff_map[start_position.x][start_position.y];
	flood_fill_to_destination(start_position);
	uint8_t known_distance = ff_map[start_position.x][start_position.y];
	
	if(known_distance <= optimistic_distance_from_goal)
	{
		next_state = navigate_state;
		follow_up_state = grab_package_state;
		tape_square = 0;
		
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
		
		/* Have we moved in the wrong direction? Then start over from goal position. */
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
			
			_delay_ms(1);
			
			if (square_diff > 400 || (square_diff > 100 && front_wall_distance < 230))
			{
				PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
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

/* 
 * Grabs the pachage from the start square and drives back to the start of the maze.
 */
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

/*
 * Drops the package in the goal square and backs up one square.
 */
void drop_package_state()
{
	set_desired_engine_speed(0);
	_delay_ms(100);
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

	current_direction = (current_direction + 2) & 3;
	move_map_position_forward();
	current_direction = (current_direction + 2) & 3;

	flood_fill_to_destination(start_position);

	next_state = navigate_state;
	follow_up_state = end_state;
}

/*
 * Steps uot of the maze, sends all map data again and stops.
 */
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

	/* Send all map data again */
	for(uint8_t i = 1; i<MAP_SIZE-1; ++i)
	{
		for(uint8_t j = 1; j<MAP_SIZE-1; ++j)
		{
			if(map[i][j] == WALL)
			{
				uint8_t msg[] = {MAPPED_WALL, i, j};
				while(!i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg))){_delay_ms(1);}
				_delay_ms(100);	
			}
			if(map[i][j] == NOT_WALL)
			{
				uint8_t msg[] = {MAPPED_SQUARE, i, j};
				while(!i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg))){_delay_ms(1);}
				_delay_ms(100);
			}
		}
	}

	while (1)
	{
		PORTD ^= (1<<DEBUG_LED_GREEN)|(1<<DEBUG_LED_RED);
		_delay_ms(200);
	}
}

/*
 * Turns the robot left 90 degrees. Is not a state in the state machine.
 */
void rotate_left_90()
{
	set_desired_engine_speed(0);
	_delay_ms(250);
	force_engine_direction(ENGINE_DIRECTION_LEFT);
	_delay_ms(250);
	
	int16_t last_absolute_rotation = absolute_rotation;
	force_engine_speed(TURN_SPEED);
	
	/* SIC! not an error */
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

/*
 * Turns the robot right 90 degrees. Is not a state in the state machine.
 */
void rotate_right_90()
{
	set_desired_engine_speed(0);
	_delay_ms(250);
	force_engine_direction(ENGINE_DIRECTION_RIGHT);
	_delay_ms(250);
	
	int16_t last_absolute_rotation = absolute_rotation;
	force_engine_speed(TURN_SPEED);
	
	/* SIC! not an error */
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

/*
 * Turns the robot 180 degrees in the direction with the most clearance. Is not a state in the state machine.
 */
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
		
		/* SIC! not an error */
		while (absolute_rotation - last_absolute_rotation > -190)
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
		
		/* SIC! not an error */
		while (absolute_rotation - last_absolute_rotation < 190)
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

/*
 * Controls the robots movements in manual mode.
 */
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