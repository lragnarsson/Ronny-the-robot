﻿/*
 * map.c
 */

#include <avr/io.h>
#include "map.h"
#include "bus_communication.h"

											// 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32
/*extern uint8_t map[MAP_SIZE][MAP_SIZE] = { {254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 0
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 1
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 2
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 3
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 4
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 5
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 6
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 7
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 253, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 8
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 253, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 9
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 253, 255, 255, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 10
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 253, 255, 255, 253, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 11
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 253, 253, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 12
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 253, 255, 253, 255, 255, 253, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 13
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 253, 255, 253, 253, 253, 253, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 14
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 253, 255, 253, 255, 255, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 15
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 16
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 17
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 18
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 19
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 20
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 21
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 22
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 23
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 24
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 25
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 26
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 27
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 28
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 29
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 30
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 31
											{254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254} }; // 32

*/
extern uint8_t map[MAP_SIZE][MAP_SIZE];
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


/* Called when Ronny has moved one grid square in 'current_direction' to update position */
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
	
	// Send current position
	uint8_t msg[] = { ABSOLUTE_X_Y, current_position.x, current_position.y };
	i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
	uint8_t msg_[] = { MAPPED_SQUARE, current_position.x, current_position.y };
	i2c_write(COMMUNICATION_UNIT, msg_, sizeof(msg_));
}

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
				map[left_wall.y][++left_wall.y] = WALL;
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
	
	// Send map coordinates
	if (wall_front)
	{
		uint8_t msg[] = { MAPPED_WALL, front_wall.x, front_wall.y };
		i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
	}
	if (wall_left)
	{
		uint8_t msg[] = { MAPPED_WALL, left_wall.x, left_wall.y };
		i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
	}
	if (wall_right)
	{
		uint8_t msg[] = { MAPPED_WALL, right_wall.x, right_wall.y };
		i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
	}
}

/* Is current square NOT_WALL? */
uint8_t current_sqaure_not_wall()
{
	return (map[current_position.x][current_position.y] == NOT_WALL);
}

/* Set current square to NOT_WALL */
void set_current_sqaure_not_wall()
{
	map[current_position.x][current_position.y] = NOT_WALL;
}