/*
 * i2c_test.c
 *
 * Created: 4/1/2015 4:10:04 PM
 *  Author: filos433
 */ 

#include <avr/io.h>
#define F_CPU 18432000UL
#include <avr/interrupt.h>
#include <util/delay.h>

#include "I2C.h"

void handle_recieved_message() {
	i2c_write(0x18, 0x66,1);
}


int main(void)
{
	uint8_t busvar = 0x00;
	uint8_t busdata[16];
	cli();
	DDRB = 0x01; //PB0 output
	PORTB = (0<<PB0);
	sei();
	i2c_init(atmega18br, atmega18pc, 0x18); // Test module
	while(1)
    {
		//_delay_ms(10);
		//uint8_t data[3] = {0x41,0x42,0x43};
		//i2c_write(communication_unit, data, 9);
		//i2c_write(sensor_unit, 0x12);
		//i2c_write(sensor_unit, 0xA4);
		/*busvar = i2c_get_buffer(busdata);
        if(!(busvar == 0x00)) {
			PORTB = (1<<PB0);
			_delay_ms(100);
		}*/
		/*i2c_clear_buffer();
		busvar = i2c_get_buffer();
		if(busvar == 0x00) {
			PORTB = (0<<PB0);
			_delay_ms(100);
			
		}*/
		//i2c_write(communication_unit, data, 3);
		_delay_ms(1000);
    }
}