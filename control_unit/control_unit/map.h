/* 
 * map.h
 * Ronny-the-robot/control_unit
 * ------------------------------
 * This file contains the map representation and flood fill map.
 * ------------------------------ 
 * Author: L. Ragnarsson, E. Sköld
 */

#ifndef MAP_H_
#define MAP_H_

#define MAP_SIZE 33

#define LEFT 1
#define BACKWARD 2
#define RIGHT 3
#define ROUTE_END 10

#define NOT_WALL 253
#define UNMAPPED 254
#define WALL 255
#define FF_DEFAULT 255
#define START_POSITION_X 16
#define START_POSITION_Y 16

typedef enum { NORTH, EAST, SOUTH, WEST } direction;
typedef struct {
	uint8_t x, y;
} coordinate;

uint8_t map[MAP_SIZE][MAP_SIZE];
uint8_t ff_map[MAP_SIZE][MAP_SIZE];
coordinate start_position;
coordinate current_position;
coordinate goal_position;

direction current_direction;
direction current_route[64];

void init_map();
void move_map_position_forward();
void set_walls(uint8_t wall_front, uint8_t wall_left, uint8_t wall_right);
uint8_t current_sqaure_not_wall();
void set_current_sqaure_not_wall();

#endif /* MAP_H_ */