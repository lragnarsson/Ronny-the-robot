/* 
 * communication_unit.c
 * Ronny-the-robot/communication_unit
 * ------------------------------
 * This file contains the logic for sending data to and from the PC-module.
 * ------------------------------ 
 * Author: F. Östman
 */ 
#include <avr/io.h>
#define F_CPU 18432000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include "personality.h"
#include "UART.h"
#include "I2C.h"

int8_t moved_distance = 0;
void init_personality() {
	TCCR1A = (0<<WGM11)|(0<<WGM10)|(1<<COM1A1)|(0<<COM1A0);
	TCCR1B = (1<<WGM13)|(0<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
	DDRD = (1<<DDD5);
	OCR1AH = (100 >> 8);
	OCR1AL = 100;

	TCCR3A = TIMER3_CLEAR_ON_MATCH_L;
	TCCR3B = TIMER3_CLEAR_ON_MATCH_H | TIMER3_PRESCALE_64;
	TIMSK3 = TIMER3_INTERRUPT_ENABLE;
	OCR3A = TIMER3_MATCH_FREQUENCY_100HZ;
	
	current_sound = 0;
	sound_phase = 2;
	sound_index = 0;
	sound_clock = 0;

}

void handle_received_message() {
	static uint8_t sensor_count = 0;
	switch(busbuffer[0]) {
		case 0xFF:
			Send_to_PC(busbuffer[1]);
			break;
		case AUTONOMOUS_MODE:
		case MANUAL_MODE:
		case TAPE_FOUND:
			Send_to_PC(busbuffer[0]);
			break;
		case TAPE_SENSOR_VALUE:
			Send_to_PC(busbuffer[0]);
			Send_to_PC(busbuffer[1]);
			break;
		case ABSOLUTE_X_Y:
		case MAPPED_SQUARE:
		case MAPPED_WALL:
		case MAPPED_GOAL:
			for(uint8_t j=0; j<3; j++) {
				Send_to_PC(busbuffer[j]);
			}
			break;
		case PLAY_SOUND:
			sound_index = 0;
			sound_clock = 0;
			if (busbuffer[1] == 11)
				sound_phase = 2;
			else
			{
				current_sound = busbuffer[1];
				sound_phase = 0;
			}
			break;
		case MOVED_DISTANCE_AND_ANGLE:
			moved_distance += (int8_t*)busbuffer[1];
			if (abs(moved_distance) >= 100) {
				Send_to_PC(busbuffer[0]);
				Send_to_PC(moved_distance);
				moved_distance = 0;
			}
			break;
		case SENSOR_READINGS:
			if (sensor_count == 5) {
				for(uint8_t j=0; j<11; j++) {
					Send_to_PC(busbuffer[j]);
					
				}
				sensor_count = 0;
			}
			else {
				++sensor_count;
			}
			break;
		default:
			Send_to_PC('F');
			Send_to_PC('E');
			Send_to_PC('L');
			Send_to_PC(busbuffer[0]);
			break;
	}
}

int main(void) {
	Init_UART(9); //Set baudrate to 115.2kbps and initiate UART
	i2c_init(BITRATE_18MHZ, PRESCALER_18MHZ, COMMUNICATION_UNIT);
	init_personality();
	while(1) {
		_delay_ms(50);
		is_sending = 0;
		if(UART_not_empty()) {
			switch(buffer[0]) {
				case DRIVE_FORWARD_LEFT:
				case 'q':
					i2c_write_byte(CONTROL_UNIT, (uint8_t)DRIVE_FORWARD_LEFT);
					break;
				case DRIVE_FORWARD:
				case 'w':
					i2c_write_byte(CONTROL_UNIT, (uint8_t)DRIVE_FORWARD);
					break;
				case DRIVE_FORWARD_RIGHT:
				case 'e':
					i2c_write_byte(CONTROL_UNIT, (uint8_t)DRIVE_FORWARD_RIGHT);
					break;
				case TURN_LEFT:
				case 'a':
					i2c_write_byte(CONTROL_UNIT, (uint8_t)TURN_LEFT);
					break;
				case DRIVE_BACKWARD:
				case 's':
					i2c_write_byte(CONTROL_UNIT, (uint8_t)DRIVE_BACKWARD);
					break;
				case TURN_RIGHT:
				case 'd':
					i2c_write_byte(CONTROL_UNIT, (uint8_t)TURN_RIGHT);
					break;
				case CALIBRATE_TAPE_SENSOR:
					i2c_write_byte(SENSOR_UNIT, (uint8_t)CALIBRATE_TAPE_SENSOR);
					break;
				case P_PARAMETER:
				case D_PARAMETER:
				case MOTOR_TRIM:
					i2c_write(CONTROL_UNIT, buffer, 2);
					break;
				default:
					break;
			}
			UART_empty();
		}
	}
	return 0;
}

ISR(TIMER3_COMPA_vect)
{
	++sound_clock;
	switch(sound_phase)
	{
		case 0: 
			if (sound_clock == music[current_sound][sound_index])
			{
				++sound_index;
				if (music[current_sound][sound_index] == 0)
					sound_phase = 2;
				else
				{
					sound_phase = 1;
					ICR1H = (music[current_sound][sound_index] >> 8);
					ICR1L = music[current_sound][sound_index];
					sound_clock = 0;
					++sound_index;
				}	
			}
			break;
		case 1:
			if (sound_clock == music[current_sound][sound_index])
			{
				ICR1H = (0 >> 8);
				ICR1L = 0;
				sound_clock = 0;
				++sound_index;
				sound_phase = 0;
			}
			break;
		case 2:
		if (sound_clock == 100)
		{
			ICR1H = (0 >> 8);
			ICR1L = 0;
			sound_clock = 0;
		}
		break;
	}
}