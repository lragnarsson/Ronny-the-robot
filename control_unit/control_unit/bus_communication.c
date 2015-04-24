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
	current_angle_error = 0;
	current_angle_left = 0;
	current_angle_right = 0;
	current_distance_error = 0;
	left_wall_distance = 0;
	right_wall_distance = 0;
	front_wall_distance = 0;
	last_tick_angle_left = 0;
	last_tick_angle_right = 0;
	distance_remaining = 0;
	square_distance_remaining = 0;
	inverse_sampling_speed = 1;
	last_manual_command = M_FORWARD;
}

void update_sensor_readings(uint8_t rear_left_h, uint8_t rear_left_l, uint8_t front_left_h, uint8_t front_left_l,
							uint8_t rear_right_h, uint8_t rear_right_l, uint8_t front_right_h, uint8_t front_right_l, uint8_t front_h, uint8_t front_l) {
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
	uint16_t front = front_h;
	front = front << 8;
	front |= front_l;	
	
	left_wall_distance = (front_left + rear_left) / 2;
	right_wall_distance = (front_right + rear_right) / 2;
	current_distance_error = left_wall_distance - right_wall_distance;

	last_tick_angle_left = current_angle_left;
	last_tick_angle_right = current_angle_right;
	current_angle_left = front_left - rear_left;
	current_angle_right = rear_right - front_right;
	current_angle_error = current_angle_left + current_angle_right;
}

void update_distance_and_angle(int8_t distance, int8_t angle) {
	if(distance_remaining > 0)
	{
		if(distance_remaining > distance)
			distance_remaining -= distance;
		else
			distance_remaining = 0;
	} else if(distance_remaining < 0)
	{
		if(distance_remaining < distance)
			distance_remaining -= distance;
		else
			distance_remaining = 0;
	}

	if(square_distance_remaining > 0)
	{
		if(square_distance_remaining > distance)
			square_distance_remaining -= distance;
		else
			square_distance_remaining = 0;
	} else if(square_distance_remaining < 0)
	{
		if(square_distance_remaining < distance)
			square_distance_remaining -= distance;
		else
			square_distance_remaining = 0;
	}

	if(angle_remaining > 0)
	{
		if(angle_remaining > angle)
			angle_remaining -= angle;
		else
			angle_remaining = 0;
	} else if (angle_remaining < 0)
	{
		if(angle_remaining < angle)
			angle_remaining -= angle;
		else
			angle_remaining = 0;
	}
}

void tape_found() {
	tape_square = 1;
}

void manual_drive_forward() {
	//P_COEFFICIENT += 0x0500;
	last_manual_command = M_FORWARD;
	distance_remaining = 100;
}

void manual_turn_right() {
	//D_COEFFICIENT += 0x0500;
	last_manual_command = M_RIGHT;
	angle_remaining = -20;
}

void manual_turn_left() {
	//D_COEFFICIENT -= 0x0500;
	last_manual_command = M_LEFT;
	angle_remaining = 20;
}

void manual_drive_forward_right() {
	//state_speed += 5;
	last_manual_command = M_FORWARD_RIGHT;
	distance_remaining = 100;
}

void manual_drive_forward_left() {
	//state_speed -= 5;
	last_manual_command = M_FORWARD_LEFT;
	distance_remaining = 100;
}

void manual_drive_backward() {
	//P_COEFFICIENT -= 0x0500;
	last_manual_command = M_BACKWARD;
	distance_remaining = -100;
}

void handle_received_message() {
	switch(busbuffer[0]) {
		case SENSOR_READINGS:
			update_sensor_readings(busbuffer[1], busbuffer[2], busbuffer[3], busbuffer[4], busbuffer[5], busbuffer[6], busbuffer[7], busbuffer[8], busbuffer[9], busbuffer[10]);
			break;
		case MOVED_DISTANCE_AND_ANGLE:
			update_distance_and_angle(busbuffer[1], busbuffer[2]);
			break;
		case TAPE_FOUND:
			tape_found();
			break;
		case DRIVE_FORWARD:
			manual_drive_forward();
			break;
		case TURN_RIGHT:
			manual_turn_right();
			break;
		case TURN_LEFT:
			manual_turn_left();
			break;
		case DRIVE_FORWARD_RIGHT:
			manual_drive_forward_right();		
			break;
		case DRIVE_FORWARD_LEFT:
			manual_drive_forward_left();
			break;
		case DRIVE_BACKWARD:
			manual_drive_backward();
			break;
		default:
			break;
	}
}