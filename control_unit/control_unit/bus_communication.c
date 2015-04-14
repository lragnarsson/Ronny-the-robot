/*
 * bus_communication.c
 *
 */ 
#include "bus_communication.h"

uint16_t left_wall_distance;
uint16_t right_wall_distance;

void init_bus_communication() {
	current_angle = 0;
	current_angle_left = 0;
	current_angle_right = 0;
	current_distance_error = 0;
	left_wall_distance = 0;
	right_wall_distance = 0;
	last_tick_angle_left = 0;
	last_tick_angle_right = 0;
	inverse_sampling_speed = 1;
}
