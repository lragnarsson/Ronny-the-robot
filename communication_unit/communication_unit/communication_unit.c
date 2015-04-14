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

int main(void) {
	Init_UART(9); //Set baudrate to 115.2kbps and initiate UART
	i2c_init(atmega18br, atmega18pc, communication_unit);
	uint8_t busvar = 0x00;
	uint8_t busdata[16];
	//init_personality();
	while(1) {
		//if(UART_not_empty()) {
			//uint8_t data = UART_get_buffer();
			//UART_putstring(data);
			//Send_to_PC('\n');
			//i2c_write(sensor_unit, data);
			//i2c_send(sensor_unit, 0x52);
			//UART_empty();
		//}
		/*_delay_ms(1000);
		uint8_t data[2] = {0xf6,0x51};
		i2c_write(sensor_unit, data, 2);
		//i2c_write(sensor_unit, 0x12);
		//i2c_write(sensor_unit, 0xA4);
		_delay_ms(1000);*/
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
	}

	return 0;
}