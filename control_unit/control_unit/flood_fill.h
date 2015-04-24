/*
 * flood_fill.h
 *
 */ 

#ifndef FLOOD_FILL_H_
#define FLOOD_FILL_H_

#include "map.h"

void reset_flood_fill_values();
void calculate_route(coordinate destination);
void flood_fill_to_destination(coordinate destination);
void flood_fill_to_unmapped();
uint8_t flood_fill_home_optimistic();

#endif /* FLOOD_FILL_H_ */