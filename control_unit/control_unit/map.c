/* 
 * map.c
 * Ronny-the-robot/control_unit
 * ------------------------------
 * This file contains the map representation and flood fill map.
 * ------------------------------ 
 * Author: L. Ragnarsson, E. Sköld
 */ 

#include <avr/io.h>

#include "map.h"
#include "bus_communication.h"

extern uint8_t map[MAP_SIZE][MAP_SIZE];

/*
 * Initiates the maps with default values.
 */
void init_map() {
	
	for (int i = 0; i < MAP_SIZE; ++i)
		for(int j = 0; j < MAP_SIZE; ++j)
			map[i][j] = UNMAPPED;

	for (int i = 0; i < MAP_SIZE; ++i)
	{
		map[0][i] = WALL;
		map[MAP_SIZE - 1][i] = WALL;
		map[i][0] = WALL;
		map[i][MAP_SIZE - 1] = WALL;
	}
		
	for (int i = 0; i < MAP_SIZE; ++i)
		for(int j = 0; j < MAP_SIZE; ++j)
			ff_map[i][j] = FF_DEFAULT;

	start_position = current_position = (coordinate) {START_POSITION_X, START_POSITION_Y};
	map[start_position.x][start_position.y] = NOT_WALL;
	current_direction = NORTH;
	current_route[0] = ROUTE_END;
}


/*
 * Called when the robot has moved one grid square in current_direction to update position.
 */
void move_map_position_forward() {
	switch(current_direction)
	{
		case NORTH:
			--current_position.x;
			break;
		case EAST:
			++current_position.y;
			break;
		case SOUTH:
			++current_position.x;
			break;
		case WEST:
			--current_position.y;
			break;
	}
	
	/* Send current position */
	uint8_t msg[] = { ABSOLUTE_X_Y, current_position.x, current_position.y };
	i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
	_delay_ms(10);
	uint8_t msg_[] = { MAPPED_SQUARE, current_position.x, current_position.y };
	i2c_write(COMMUNICATION_UNIT, msg_, sizeof(msg_));
	_delay_ms(10);
}

/*
 * Updates the map with walls based on the robots direction and if walls are present left, right and front.
 */
void set_walls(uint8_t wall_front, uint8_t wall_left, uint8_t wall_right)
{
	coordinate front_wall = current_position;
	coordinate left_wall = current_position;
	coordinate right_wall = current_position;
	
	switch(current_direction)
	{
		case NORTH:
			if (wall_front)
				map[--front_wall.x][front_wall.y] = WALL;
			if (wall_left)
				map[left_wall.x][--left_wall.y] = WALL;
			if (wall_right)
				map[right_wall.x][++right_wall.y] = WALL;
			break;
		case EAST:
			if (wall_front)
				map[front_wall.x][++front_wall.y] = WALL;
			if (wall_left)
				map[--left_wall.x][left_wall.y] = WALL;
			if (wall_right)
				map[++right_wall.x][right_wall.y] = WALL;
			break;
		case SOUTH:
			if (wall_front)
				map[++front_wall.x][front_wall.y] = WALL;
			if (wall_left)
				map[left_wall.x][++left_wall.y] = WALL;
			if (wall_right)
				map[right_wall.x][--right_wall.y] = WALL;
			break;
		case WEST:
			if (wall_front)
				map[front_wall.x][--front_wall.y] = WALL;
			if (wall_left)
				map[++left_wall.x][left_wall.y] = WALL;
			if (wall_right)
				map[--right_wall.x][right_wall.y] = WALL;
			break;
	}
	
	/* Send map coordinates */
	if (wall_front)
	{
		uint8_t msg[] = { MAPPED_WALL, front_wall.x, front_wall.y };
		i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
		_delay_ms(10);
	}
	if (wall_left)
	{
		uint8_t msg[] = { MAPPED_WALL, left_wall.x, left_wall.y };
		i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
		_delay_ms(10);
	}
	if (wall_right)
	{
		uint8_t msg[] = { MAPPED_WALL, right_wall.x, right_wall.y };
		i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
		_delay_ms(10);
	}
}

/*
 *Is current square NOT_WALL?
 */
uint8_t current_sqaure_not_wall()
{
	return (map[current_position.x][current_position.y] == NOT_WALL);
}

/*
 * Set current square to NOT_WALL
 */
void set_current_sqaure_not_wall()
{
	map[current_position.x][current_position.y] = NOT_WALL;
}