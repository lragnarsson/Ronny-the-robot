/*
 * map.c
 */

#include <avr/io.h>
#include "map.h"

void init_map() {
	for (int i = 0; i < MAP_SIZE; ++i)
		for(int j = 0; j < MAP_SIZE; ++j)
			map[i][j] = UNMAPPED;
			
	position_x = START_POSITION_X;
	position_y = START_POSITION_Y;
	route_index = 0;
	current_route[0] = 10;
}

/* Sets the current direction (NORTH, EAST, SOUTH or WEST) */
void set_current_direction(direction dir) {
	current_direction = dir;
}

/* Called when Ronny has moved one grid square in 'current_direction' to update position */
void move_map_position_forward() {
	switch(current_direction) {
		case NORTH:
			--position_y;
			break;
		case EAST:
			++position_x;
			break;
		case SOUTH:
			++position_y;
			break;
		case WEST:
			--position_x;
			break;
	}
	map[position_x][position_y] = 0; // Not unmapped value
}

/* Updated map with a detected wall to the left of Ronny */
void set_wall_left() {
	switch(current_direction) {
		case NORTH:
			map[position_x - 1][position_y] = WALL;
			break;
		case EAST:
			map[position_x][position_y - 1] = WALL;
			break;
		case SOUTH:
			map[position_x + 1][position_y] = WALL;
			break;
		case WEST:
			map[position_x][position_y + 1] = WALL;
			break;
	}
}

/* Updated map with a detected wall to the right of Ronny */
void set_wall_right() {
	switch(current_direction) {
		case NORTH:
			map[position_x + 1][position_y] = WALL;
			break;
		case EAST:
			map[position_x][position_y + 1] = WALL;
			break;
		case SOUTH:
			map[position_x - 1][position_y] = WALL;
			break;
		case WEST:
			map[position_x][position_y - 1] = WALL;
			break;
	}
}

/* Updated map with a detected wall in front of Ronny */
void set_wall_front() {
	switch(current_direction) {
		case NORTH:
			map[position_x][position_y - 1] = WALL;
			break;
		case EAST:
			map[position_x + 1][position_y] = WALL;
			break;
		case SOUTH:
			map[position_x][position_y + 1] = WALL;
			break;
		case WEST:
			map[position_x - 1][position_y] = WALL;
			break;
	}
}

/* Updated map with the detected goal */
void set_goal_position() {
	goal_position_x = position_x;
	goal_position_y = position_y;
}