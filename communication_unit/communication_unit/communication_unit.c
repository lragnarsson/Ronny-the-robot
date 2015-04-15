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

void handle_recieved_message() {
	for(uint8_t j=0; busbuffer[j]!=0x00; j++) {
		Send_to_PC(busbuffer[j]);
	}
}

int main(void) {
	Init_UART(9); //Set baudrate to 115.2kbps and initiate UART
	i2c_init(atmega18br, atmega18pc, communication_unit);
	uint8_t busvar = 0x00;
	uint8_t busdata[16];
	uint8_t data[16];
	//init_personality();
	while(1) {
		//_delay_ms(100);
	
		/*if(UART_not_empty()) {
			UART_get_buffer(data);
			switch(data[0] & 0x30) { //Check end destination
				case 0x00: //To control_unit
					i2c_write(control_unit, data, 1);
					break;
				case 0x10: // To sensor_unit
					i2c_write(sensor_unit, data, 1);
					break;
				default: // Should not happen
					break;
			}
			//UART_putstring(data);
			//Send_to_PC('\n');
				//i2c_write(sensor_unit, data,4);
			//i2c_send(sensor_unit, 0x52);
			UART_empty();
		}*/
		if(UART_not_empty()) {
			//UART_get_buffer(data);
			switch(buffer[0]) {
				case 'q':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD_LEFT, 1);
					break;
				case 'w':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD, 1);
					break;
				case 'e':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_FORWARD_RIGHT, 1);
					break;
				case 'a':
					i2c_write_byte(control_unit, (uint8_t)TURN_LEFT, 1);
					break;
				case 's':
					i2c_write_byte(control_unit, (uint8_t)DRIVE_BACKWARD, 1);
					break;
				case 'd':
					i2c_write_byte(control_unit, (uint8_t)TURN_RIGHT, 1);
					break;
				default:
					break;
			}
			UART_empty();
		}
	}
		//UART_putstring(data);
		//Send_to_PC('\n');
		//i2c_write(sensor_unit, data,4);
		//i2c_send(sensor_unit, 0x52);
		
	
		//}
		/*_delay_ms(1000);
		uint8_t data[2] = {0xf6,0x51};
		i2c_write(sensor_unit, data, 2);
		//i2c_write(sensor_unit, 0x12);
		//i2c_write(sensor_unit, 0xA4);
		_delay_ms(1000);*/
		
		/*
		busvar = i2c_get_buffer(busdata);
		//_delay_ms(10);
		if(!(busvar == 0x00)) {
			cli();
			_delay_ms(100); 
			for(uint8_t i=0; i<=busvar; i++) {
				unsigned char j = busdata[i];
				Send_to_PC(j);
				Send_to_PC('\n');
			}
			i2c_write(sensor_unit,busdata,9);
			sei();
		}
		i2c_clear_buffer();
		_delay_ms(100);
		*/
	//}

	return 0;
}
	/*	if(buffer[0]=='a') {
			for(uint8_t j=0; buffer[j]!=0x00; j++) {
				Send_to_PC(buffer[j]);
			}
		}
		UART_empty();
		}*/