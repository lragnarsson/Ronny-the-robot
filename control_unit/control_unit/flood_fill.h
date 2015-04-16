/*
 * flood_fill.h
 *
 */ 

#ifndef FLOOD_FILL_H_
#define FLOOD_FILL_H_

#include "map.h"

void swap_wavefronts(coordinate** wavefront1, coordinate** wavefront2);
void reset_flood_fill_values();
void calculate_route(coordinate destination);
void flood_fill_to_destination(coordinate destination);
void flood_fill_to_unmapped();
void flood_fill_home_optimistic();

#endif /* FLOOD_FILL_H_ */