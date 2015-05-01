/*
 * communication.c
 *
 * Created: 2015-03-23 22:37:02
 *  Author: Filip
 */ 
#include <avr/io.h>
#define F_CPU 18432000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include "personality.h"

#include "UART.h"
#include "I2C.h"
//#include "sound.c"

void init_personality() {
	TCCR1A = (0<<WGM11)|(0<<WGM10)|(1<<COM1A1)|(0<<COM1A0);
	TCCR1B = (1<<WGM13)|(0<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
	DDRD = (1<<DDD5);
	OCR1AH = (100 >> 8);
	OCR1AL = 100;

	for (int i=0; i < 250; i++) {
		ICR1H = (imperial_march[i] >> 8);
		ICR1L = imperial_march[i];
		++i;
		delay_ms(imperial_march[i]);
		ICR1H = (0 >> 8);
		ICR1L = 0;
		++i;
		delay_ms(imperial_march[i]);		
	}
}

void handle_received_message() {
	static uint8_t sensor_count = 0;
	switch(busbuffer[0]) {
		/*case 0xFF:
			Send_to_PC(busbuffer[1]);
			break;*/
		case ABSOLUTE_X_Y:
		case AUTONOMOUS_MODE:
		case MANUAL_MODE:
		case TAPE_FOUND:
			//Send_to_PC(busbuffer[0]);
			break;
		case MAPPED_SQUARE:
		case MAPPED_WALL:
		case MAPPED_GOAL:
		case MOVED_DISTANCE_AND_ANGLE:
			/*for(uint8_t j=0; j<3; j++) {
				Send_to_PC(busbuffer[j]);
			}*/
			break;
		case SENSOR_READINGS:
			if (sensor_count == 25) {
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
			/*Send_to_PC('F');
			Send_to_PC('E');
			Send_to_PC('L');
			Send_to_PC(busbuffer[0]);*/
			break;
	}
}

int main(void) {
	Init_UART(9); //Set baudrate to 115.2kbps and initiate UART
	i2c_init(atmega18br, atmega18pc, COMMUNICATION_UNIT);
	//init_personality();
	while(1) {
		_delay_ms(50);
		if(UART_not_empty()) {
			switch(buffer[0]) {
				//Correct byte
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