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
	cli();
	DDRB = 0x01; //PB0 output
	PORTB = (0<<PB0);
	sei();
	i2c_init(atmega18br, atmega18pc, sensor_unit);
	while(1)
    {
		
		busvar = i2c_get_buffer();
        if(!(busvar == 0x00)) {
			PORTB = (1<<PB0);
			//_delay_ms(5000);
			//PORTB = (0<<PB0);
			//_delay_ms(1000);
		}
		//i2c_clear_buffer();
    }
}