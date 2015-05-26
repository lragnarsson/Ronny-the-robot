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
	
	last_manual_command = DO_NOTHING;
	last_manual_command_updated = 0;
	
	tape_square = 0;
	
	left_wall_distance = 0;
	right_wall_distance = 0;
	front_wall_distance = 0;
	
	wall_left = 0;
	wall_right = 0;
	wall_front = 0;
	
	current_distance_error = 0;
	current_derivative_error = 0;
	
	distance_travelled = 0;
	absolute_rotation = 0;
	
	intersection = 0;
}

void update_sensor_readings(uint8_t rear_left_h, uint8_t rear_left_l, uint8_t front_left_h, uint8_t front_left_l,
							uint8_t rear_right_h, uint8_t rear_right_l, uint8_t front_right_h, uint8_t front_right_l, uint8_t front_h, uint8_t front_l) {
	
	static uint16_t rear_left = 0;
	static uint16_t front_left = 0;
	static uint16_t rear_right = 0;
	static uint16_t front_right = 0;
	
	/* Keep previous values */
	int16_t previous_distance_left = left_wall_distance;
	int16_t previous_distance_right = right_wall_distance;
	
	int16_t previous_distance_rear_left = rear_left;
	int16_t previous_distance_front_left = front_left;
	int16_t previous_distance_rear_right = rear_right;
	int16_t previous_distance_front_right = front_right;
	
	/* Pack 16 bit sensor data */
	rear_left = rear_left_h;
	rear_left = rear_left << 8;
	rear_left |= rear_left_l;
	
	front_left = front_left_h;
	front_left = front_left << 8;
	front_left |= front_left_l;
	
	rear_right = rear_right_h;
	rear_right = rear_right << 8;
	rear_right |= rear_right_l;
	
	front_right = front_right_h;
	front_right = front_right << 8;
	front_right |= front_right_l;
	
	front_wall_distance = front_h;
	front_wall_distance = front_wall_distance << 8;
	front_wall_distance |= front_l;
	
	/* Calculate new values */
	left_wall_distance = (front_left + rear_left) / 2;
	right_wall_distance = (front_right + rear_right) / 2;
	
	static uint8_t left_wall_count = 0;
	static uint8_t right_wall_count = 0;
	static uint8_t front_wall_count = 0;
	left_wall_count = CLAMP(left_wall_count - 1 + (uint8_t)(left_wall_distance < 300) * 2, 0, 5);
	right_wall_count = CLAMP(right_wall_count - 1 + (uint8_t)(right_wall_distance < 300) * 2, 0, 5);
	front_wall_count = CLAMP(front_wall_count - 1 + (uint8_t)(front_wall_distance < 300) * 2, 0, 5);
	wall_left = (uint8_t)(left_wall_distance < 300);//(uint8_t)(left_wall_count > 3);
	wall_right = (uint8_t)(right_wall_distance < 300);//(uint8_t)(right_wall_count > 3);
	wall_front = (uint8_t)(front_wall_distance < 300);//(uint8_t)(front_wall_count > 3);

	/* Calculate control errors */
	current_distance_error = left_wall_distance - right_wall_distance;
	
	int16_t current_derivative_left = (left_wall_distance - previous_distance_left) * SAMPLE_FREQUENCY;
	int16_t current_derivative_right = -(right_wall_distance - previous_distance_right) * SAMPLE_FREQUENCY;
	
	int16_t current_derivative_rear_left = (rear_left - previous_distance_rear_left)*SAMPLE_FREQUENCY;
	int16_t current_derivative_front_left = (front_left - previous_distance_front_left)*SAMPLE_FREQUENCY;
	int16_t current_derivative_rear_right = -(rear_right - previous_distance_rear_right)*SAMPLE_FREQUENCY;
	int16_t current_derivative_front_right = -(front_right - previous_distance_front_right)*SAMPLE_FREQUENCY;
	
	current_derivative_error = current_derivative_left + current_derivative_right;
	
	intersection = 0;// (current_derivative_front_left > 350 || current_derivative_front_right > 350);
	
	//PORTD &= ~(1<<DEBUG_LED_RED);
	//PORTD |= (1<<DEBUG_LED_GREEN);
	
	/* Ignore bad values */
	if (abs(current_derivative_left) > 350 || abs(current_derivative_right) > 350 || rear_left > 300 || front_left > 300 || rear_right > 300 || front_right > 300)
	{
		//PORTD &= ~(1<<DEBUG_LED_GREEN);
		//PORTD |= (1<<DEBUG_LED_RED);
		
		if (abs(current_derivative_left) < 350 && rear_left < 300 && front_left < 300)
		{
			current_distance_error = left_wall_distance - (200 - 60);
			current_derivative_error = current_derivative_left * 2;
		} else if (abs(current_derivative_right) < 350 && rear_right < 300 && front_right < 300)
		{
			current_distance_error = 200 - 60 - right_wall_distance;
			current_derivative_error = current_derivative_right * 2;
		} /*else if (current_derivative_rear_left < 350 && current_derivative_rear_right < 350 && rear_left < 300 && rear_right < 300)
		{
			current_distance_error = rear_left - rear_right;
			current_derivative_error = ( current_derivative_rear_left + current_derivative_rear_right ) * 2;
		} else if (current_derivative_front_left < 350 && current_derivative_front_right < 350 && front_left < 300 && front_right < 300)
		{
			current_distance_error = front_left - front_right;
			current_derivative_error = ( current_derivative_front_left + current_derivative_front_right ) * 2;
		}*/
		else
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
			last_manual_command_updated = 1;
			break;
		case TURN_RIGHT:
			last_manual_command = M_RIGHT;
			last_manual_command_updated = 1;
			break;
		case TURN_LEFT:
			last_manual_command = M_LEFT;
			last_manual_command_updated = 1;
			break;
		case DRIVE_FORWARD_RIGHT:
			last_manual_command = M_FORWARD_RIGHT;
			last_manual_command_updated = 1;
			break;
		case DRIVE_FORWARD_LEFT:
			last_manual_command = M_FORWARD_LEFT;
			last_manual_command_updated = 1;
			break;
		case DRIVE_BACKWARD:
			last_manual_command = M_BACKWARD;
			last_manual_command_updated = 1;
			break;
		case AUTONOMOUS_MODE:
			current_mode = AUTONOMOUS;
			break;
		case MANUAL_MODE:
			current_mode = MANUAL;
			break;
		case P_PARAMETER:
			P_COEFFICIENT = busbuffer[1];
			break;
		case D_PARAMETER:
			D_COEFFICIENT = busbuffer[1];
			break;
		case MOTOR_TRIM:
			motor_trim = busbuffer[1];
			break;
		default:
			break;
	}
}