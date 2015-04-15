/*
 * map.c
 */

#include <avr/io.h>
#include "map.h"

void init_map() {
	for (int i = 0; i < MAP_SIZE; ++i)
		for(int j = 0; j < MAP_SIZE; ++j)
			map[i][j] = UNMAPPED;
			
	current_position = (coordinate) {START_POSITION_X, START_POSITION_Y};
	current_direction = NORTH;
	current_route[0] = NORTH;
	current_route[1] = 10;
}

/* Sets the current direction (NORTH, EAST, SOUTH or WEST) */
void set_current_direction(direction dir) {
	current_direction = dir;
}

/* Called when Ronny has moved one grid square in 'current_direction' to update position */
void move_map_position_forward() {
	switch(current_direction) {
		case NORTH:
			--current_position.y;
			break;
		case EAST:
			++current_position.x;
			break;
		case SOUTH:
			++current_position.y;
			break;
		case WEST:
			--current_position.x;
			break;
	}
	map[current_position.x][current_position.y] = 0; // Not unmapped value
}

/* Updated map with a detected wall to the left of Ronny */
void set_wall_left() {
	switch(current_direction) {
		case NORTH:
			map[current_position.x - 1][current_position.y] = WALL;
			break;
		case EAST:
			map[current_position.x][current_position.y - 1] = WALL;
			break;
		case SOUTH:
			map[current_position.x + 1][current_position.y] = WALL;
			break;
		case WEST:
			map[current_position.x][current_position.y + 1] = WALL;
			break;
	}
}

/* Updated map with a detected wall to the right of Ronny */
void set_wall_right() {
	switch(current_direction) {
		case NORTH:
			map[current_position.x + 1][current_position.y] = WALL;
			break;
		case EAST:
			map[current_position.x][current_position.y + 1] = WALL;
			break;
		case SOUTH:
			map[current_position.x - 1][current_position.y] = WALL;
			break;
		case WEST:
			map[current_position.x][current_position.y - 1] = WALL;
			break;
	}
}

/* Updated map with a detected wall in front of Ronny */
void set_wall_front() {
	switch(current_direction) {
		case NORTH:
			map[current_position.x][current_position.y - 1] = WALL;
			break;
		case EAST:
			map[current_position.x + 1][current_position.y] = WALL;
			break;
		case SOUTH:
			map[current_position.x][current_position.y + 1] = WALL;
			break;
		case WEST:
			map[current_position.x - 1][current_position.y] = WALL;
			break;
	}
}

uint8_t is_wall_left() {
	switch(current_direction) {
		case NORTH:
			return (uint8_t)(map[current_position.x - 1][current_position.y] == WALL);
		break;
		case EAST:
			return (uint8_t)(map[current_position.x][current_position.y - 1] == WALL);
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
			return (uint8_t)(map[current_position.x + 1][current_position.y] == WALL);
			break;
		case EAST:
			return (uint8_t)(map[current_position.x][current_position.y + 1] == WALL);
			break;
		case SOUTH:
			return (uint8_t)(map[current_position.x - 1][current_position.y] == WALL);
			break;
		case WEST:
			return (uint8_t)(map[current_position.x][current_position.y - 1] == WALL);
			break;
		default:
			return 0;
	}
}

/* Updated map with the detected goal */
void set_goal_position() {
	goal_position = current_position;
}