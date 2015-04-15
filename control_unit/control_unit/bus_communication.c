/*
 * bus_communication.c
 *
 */ 
#include "bus_communication.h"

volatile uint16_t left_wall_distance;
volatile uint16_t right_wall_distance;

void init_bus_communication() {
	i2c_init(atmega20br, atmega20pc, control_unit);
	current_angle_error = 0;
	current_angle_left = 0;
	current_angle_right = 0;
	current_distance_error = 0;
	left_wall_distance = 0;
	right_wall_distance = 0;
	last_tick_angle_left = 0;
	last_tick_angle_right = 0;
	inverse_sampling_speed = 1;
}

void update_sensor_readings(uint8_t front_left, uint8_t rear_left, uint8_t front_right, uint8_t rear_right, uint8_t front) {
	left_wall_distance = front_left + rear_left;
	right_wall_distance = front_right + rear_right;
	current_distance_error = left_wall_distance - right_wall_distance;

	last_tick_angle_left = current_angle_left;
	last_tick_angle_right = current_angle_right;
	current_angle_left = front_left - rear_left;
	current_angle_right = front_right - rear_right;
	current_angle_error = current_angle_left + current_angle_right;
}

void update_distance_and_angle(uint8_t distance, uint8_t angle) {
	if(distance_remaining > distance)
		distance_remaining -= distance;
	else
		distance_remaining = 0;
	if(angle_remaining > angle)
		angle_remaining -= angle;
	else
		angle_remaining = 0;
}

void tape_found() {
	goal_found = 1;
	//TODO
}

void manual_drive_forward() {
	last_manual_command = M_FORWARD;
	if(distance_remaining < 100)
		distance_remaining += 20;
}

void manual_turn_right() {
	last_manual_command = M_RIGHT;
	if(angle_remaining < 20)
		angle_remaining += 5;
}

void manual_turn_left() {
	last_manual_command = M_LEFT;
	if(angle_remaining < 20)
		angle_remaining += 5;
}

void manual_drive_forward_right() {
	last_manual_command = M_FORWARD_RIGHT;
	if(distance_remaining < 100)
		distance_remaining += 20;
}

void manual_drive_forward_left() {
	last_manual_command = M_FORWARD_LEFT;
	if(distance_remaining < 100)
		distance_remaining += 20;
}

void manual_drive_backward() {
	last_manual_command = M_BACKWARD;
	if(distance_remaining < 100)
		distance_remaining += 20;
}

void handle_recieved_message() {
	cli();
	switch(busbuffer[0]) {
		case SENSOR_READINGS:
			update_sensor_readings(busbuffer[1], busbuffer[2], busbuffer[3], busbuffer[4], busbuffer[5]);
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
	sei();
}