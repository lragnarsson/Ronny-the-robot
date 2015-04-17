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
	for(uint8_t j=0; busbuffer[j]!=0x00; j++) {
		Send_to_PC(busbuffer[j]);
	}
	switch(busbuffer[0]) {
		case ABSOLUTE_X_Y:
		case AUTONOMOUS_MODE:
		case MANUAL_MODE:
		case TAPE_FOUND:
			Send_to_PC(busbuffer[0]);
			break;
		case MAPPED_SQUARE:
		case MAPPED_WALL:
		case MAPPED_GOAL:
		case MOVED_DISTANCE_AND_ANGLE:
			for(uint8_t j=0; j<3; j++) {
				Send_to_PC(busbuffer[j]);
			}
			break;
		case SENSOR_READINGS:
			for(uint8_t j=0; j<11; j++) {
				Send_to_PC(busbuffer[j]);
			}
			break;
		default:
			Send_to_PC('F');
			Send_to_PC('E');
			Send_to_PC('L');
			break;
	}
}

int main(void) {
	Init_UART(9); //Set baudrate to 115.2kbps and initiate UART
	i2c_init(atmega18br, atmega18pc, communication_unit);
	//init_personality();
	while(1) {
		if(UART_not_empty()) {
			switch(buffer[0]) {
				//Correct byte
				case 0xc6:
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD_LEFT);
					break;
				case 0xc1:
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD);
					break;
				case 0xc5:
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD_RIGHT);
					break;
				case 0xc3:
					i2c_write_byte(control_unit, (uint8_t)TURN_LEFT);
					break;
				case 0xc4:
					i2c_write_byte(control_unit, (uint8_t)DRIVE_BACKWARD);
					break;
				case 0xc2:
					i2c_write_byte(control_unit, (uint8_t)TURN_RIGHT);
					break;
				//Testing
				case 'q':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD_LEFT);
					break;
				case 'w':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD);
					break;
				case 'e':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD_RIGHT);
					break;
				case 'a':
					i2c_write_byte(control_unit, (uint8_t)TURN_LEFT);
					break;
				case 's':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_BACKWARD);
					break;
				case 'd':
					i2c_write_byte(control_unit, (uint8_t)TURN_RIGHT);
					break;
				default:
					break;
			}
			UART_empty();
		}
	}
	return 0;
}