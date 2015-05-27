/* 
 * flood_fill.h
 * Ronny-the-robot/control_unit
 * ------------------------------
 * This file contains the algorithms for finding the shortest paths. 
 * ------------------------------ 
 * Author: L. Ragnarsson, E. Sköld
 */ 

#ifndef F_CPU
#define F_CPU 20000000UL
#endif

#ifndef FLOOD_FILL_H_
#define FLOOD_FILL_H_

#include "map.h"

void reset_flood_fill_values();
void calculate_route(coordinate destination);
void flood_fill_to_destination(coordinate destination);
void flood_fill_to_unmapped();
void flood_fill_home_optimistic();

#endif /* FLOOD_FILL_H_ */