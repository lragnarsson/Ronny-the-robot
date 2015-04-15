/*
 * flood_fill.c
 *
 */ 

#include <avr/io.h>

#include "flood_fill.h"

typedef struct {
	coordinate position;
	direction added_from;
} wavefront_element;

uint8_t current_wavefront_size;
uint8_t new_wavefront_size;
wavefront_element current_wavefront[100];
wavefront_element new_wavefront[100];

void calculate_route(coordinate destination) {
	current_wavefront_size = 1;
	new_wavefront_size = 1;
	
	
}

void find_closest_unmapped() {
	
}

void calculate_best_path() {
	
}