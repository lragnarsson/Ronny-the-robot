/*
 * bus_communication.c
 *
 */ 
#include "bus_communication.h"
#include "control_system.h"
#include "control_unit.h"
#include <stdlib.h>

void init_bus_communication() {
	i2c_init(atmega20br, atmega20pc, CONTROL_UNIT);
	
	last_manual_command = M_FORWARD;
	
	tape_square = 0;
	
	left_wall_distance = 0;
	right_wall_distance = 0;
	front_wall_distance = 0;
	
	current_distance_error = 0;
	current_derivative_error = 0;
	
	distance_travelled = 0;
	absolute_rotation = 0;
}

void update_sensor_readings(uint8_t rear_left_h, uint8_t rear_left_l, uint8_t front_left_h, uint8_t front_left_l,
							uint8_t rear_right_h, uint8_t rear_right_l, uint8_t front_right_h, uint8_t front_right_l, uint8_t front_h, uint8_t front_l) {
	/* Pack 16 bit sensor data */
	uint16_t rear_left = rear_left_h;
	rear_left = rear_left << 8;
	rear_left |= rear_left_l;
	
	uint16_t front_left = front_left_h;
	front_left = front_left << 8;
	front_left |= front_left_l;
	
	uint16_t rear_right = rear_right_h;
	rear_right = rear_right << 8;
	rear_right |= rear_right_l;
	
	uint16_t front_right = front_right_h;
	front_right = front_right << 8;
	front_right |= front_right_l;
	
	front_wall_distance = front_h;
	front_wall_distance = front_wall_distance << 8;
	front_wall_distance |= front_l;	
	
	/* Keep previous values */
	static int16_t previous_distance_left;
	static int16_t previous_distance_right;
	previous_distance_left = left_wall_distance;
	previous_distance_right = right_wall_distance;
	
	/* Calculate new values */
	left_wall_distance = (front_left + rear_left) / 2;
	right_wall_distance = (front_right + rear_right) / 2;

	/* Calculate control errors */
	current_distance_error = left_wall_distance - right_wall_distance;
	int16_t current_derivative_left = (left_wall_distance - previous_distance_left) * SAMPLE_FREQUENCY;
	int16_t current_derivative_right = -(right_wall_distance - previous_distance_right) * SAMPLE_FREQUENCY;
	current_derivative_error = current_derivative_left + current_derivative_right;
	
	/* Ignore bad values */
	if (abs(current_derivative_left) > 450 || abs(current_derivative_right) > 450 || rear_left > 300 || front_left > 300 || rear_right > 300 || front_right > 300)
	{
		if (abs(current_derivative_left) < 450 && rear_left < 300 && front_left < 300)
		{
			current_distance_error = left_wall_distance - (200 - 60);
			current_derivative_error = current_derivative_left * 2;
		} else if (abs(current_derivative_right) < 450 && rear_right < 300 && front_right < 300)
		{
			current_distance_error = 200 - 60 - right_wall_distance;
			current_derivative_error = current_derivative_right * 2;
		} else
		{
			current_distance_error = 0;
			current_derivative_error = 0;
		}
	}
}

void handle_received_message() {
	switch(busbuffer[0]) {
		case SENSOR_READINGS:
			update_sensor_readings(busbuffer[1], busbuffer[2], busbuffer[3], busbuffer[4], busbuffer[5], busbuffer[6], busbuffer[7], busbuffer[8], busbuffer[9], busbuffer[10]);
			break;
		case MOVED_DISTANCE_AND_ANGLE:
			distance_travelled += (int8_t)busbuffer[1];
			absolute_rotation += (int8_t)busbuffer[2];
			break;
		case TAPE_FOUND:
			tape_square = 1;
			break;
		case DRIVE_FORWARD:
			last_manual_command = M_FORWARD;
			break;
		case TURN_RIGHT:
			last_manual_command = M_RIGHT;
			break;
		case TURN_LEFT:
			last_manual_command = M_LEFT;
			break;
		case DRIVE_FORWARD_RIGHT:
			last_manual_command = M_FORWARD_RIGHT;
			break;
		case DRIVE_FORWARD_LEFT:
			last_manual_command = M_FORWARD_LEFT;
			break;
		case DRIVE_BACKWARD:
			last_manual_command = M_BACKWARD;
			break;
		default:
			break;
	}
}