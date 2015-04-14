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

int main(void)
{
	uint8_t busvar = 0x00;
	uint8_t busdata[16];
	cli();
	DDRB = 0x01; //PB0 output
	PORTB = (0<<PB0);
	sei();
	i2c_init(atmega18br, atmega18pc, sensor_unit);
	while(1)
    {
		_delay_ms(10);
		uint8_t data[9] = {0x41,0x42,0x43,0x82,0x92,0x63,0x51,0x12,0x03};
		i2c_write(communication_unit, data, 9);
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
    }
}