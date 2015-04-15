/*
 * flood_fill.h
 *
 */ 

#ifndef FLOOD_FILL_H_
#define FLOOD_FILL_H_

#include "map.h"

void calculate_route(coordinate destination);
void find_closest_unmapped();
void calculate_best_path();

#endif /* FLOOD_FILL_H_ */