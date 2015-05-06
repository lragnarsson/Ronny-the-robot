/*
 * map.c
 */

#include <avr/io.h>
#include "map.h"
#include "bus_communication.h"

											// 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32
extern uint8_t map[MAP_SIZE][MAP_SIZE] = { {254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254}, // 0
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
	current_direction = NORTH;
	current_route[0] = ROUTE_END;
	goal_found = 0;
}

/* Sets the current direction (NORTH, EAST, SOUTH or WEST) */
void set_current_direction(direction dir) {
	current_direction = dir;
}

/* Called when Ronny has moved one grid square in 'current_direction' to update position */
void move_map_position_forward() {
	switch(current_direction) {
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
	if (!goal_found && tape_square) // If goal has not yet been found and a tape was detected while entering current square
	{
		goal_position = current_position;
		goal_found = 1;
		uint8_t msg[3] = {MAPPED_GOAL, goal_position.x, goal_position.y};
		i2c_write(COMMUNICATION_UNIT, msg, 3);
	}
	tape_square = 0;
	map[current_position.x][current_position.y] = NOT_WALL;
	uint8_t msg[3] = {MAPPED_SQUARE, current_position.x, current_position.y};
	i2c_write(COMMUNICATION_UNIT, msg, 3);
}

/* Updated map with a detected wall to the left of Ronny */
void set_wall_left() {
	coordinate wall_pos = current_position;
	switch(current_direction) {
		case NORTH:
			--wall_pos.y;
			break;
		case EAST:
			--wall_pos.x;
			break;
		case SOUTH:
			++wall_pos.y;
			break;
		case WEST:
			++wall_pos.x;
			break;
	}
	map[wall_pos.x][wall_pos.y] = WALL;
	uint8_t msg[3] = {MAPPED_WALL, wall_pos.x, wall_pos.y};
	i2c_write(COMMUNICATION_UNIT, msg, 3);
}

/* Updated map with a detected wall to the right of Ronny */
void set_wall_right() {
	coordinate wall_pos = current_position;
	switch(current_direction) {
		case NORTH:
			++wall_pos.y;
			break;
		case EAST:
			++wall_pos.x;
			break;
		case SOUTH:
			--wall_pos.y;
			break;
		case WEST:
			--wall_pos.x;
			break;
	}
	map[wall_pos.x][wall_pos.y] = WALL;
	uint8_t msg[3] = {MAPPED_WALL, wall_pos.x, wall_pos.y};
	i2c_write(COMMUNICATION_UNIT, msg, 3);
}

/* Updated map with a detected wall in front of Ronny */
void set_wall_front() {
	coordinate wall_pos = current_position;
	switch(current_direction) {
		case NORTH:
			--wall_pos.x;
			break;
		case EAST:
			++wall_pos.y;
			break;
		case SOUTH:
			++wall_pos.x;
			break;
		case WEST:
			--wall_pos.y;
			break;
	}
	map[wall_pos.x][wall_pos.y] = WALL;
	uint8_t msg[3] = {MAPPED_WALL, wall_pos.x, wall_pos.y};
	i2c_write(COMMUNICATION_UNIT, msg, 3);
}

uint8_t is_wall_left() {
	switch(current_direction) {
		case NORTH:
			return (uint8_t)(map[current_position.x][current_position.y - 1] == WALL);
			break;
		case EAST:
			return (uint8_t)(map[current_position.x - 1][current_position.y] == WALL);
			break;
		case SOUTH:
			return (uint8_t)(map[current_position.x][current_position.y + 1] == WALL);
			break;
		case WEST:
			return (uint8_t)(map[current_position.x + 1][current_position.y] == WALL);
			break;
		default:
			return 0;
	}
}

uint8_t is_wall_right() {
	switch(current_direction) {
		case NORTH:
			return (uint8_t)(map[current_position.x][current_position.y + 1] == WALL);
			break;
		case EAST:
			return (uint8_t)(map[current_position.x + 1][current_position.y] == WALL);
			break;
		case SOUTH:
			return (uint8_t)(map[current_position.x][current_position.y - 1] == WALL);
			break;
		case WEST:
			return (uint8_t)(map[current_position.x - 1][current_position.y] == WALL);
			break;
		default:
			return 0;
	}
}

/* Updated map with the detected goal */
void set_goal_position() {
	goal_position = current_position;
}