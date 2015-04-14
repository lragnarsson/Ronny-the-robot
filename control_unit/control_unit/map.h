/*
 * map.h
 */
#ifndef MAP_H_
#define MAP_H_

#define MAP_SIZE 33

#define LEFT 1
#define BACKWARD 2
#define RIGHT 3
#define ROUTE_END 10

#define UNMAPPED 254
#define WALL 255
#define START_POSITION_X 15
#define START_POSITION_Y 15

typedef enum { NORTH, EAST, SOUTH, WEST } direction;

uint8_t map[MAP_SIZE][MAP_SIZE];
uint8_t position_x;
uint8_t position_y;
uint8_t goal_position_x;
uint8_t goal_position_y;
	
direction current_direction;
direction current_route[64];
uint8_t route_index;

void init_map();
void set_current_direction(direction dir);
void move_map_position_forward();
void set_wall_left();
void set_wall_right();
void set_wall_front();
void set_goal_position();

#endif /* MAP_H_ */