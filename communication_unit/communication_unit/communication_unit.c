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

#include "UART.h"
#include "I2C.h"


int main(void) {
	Init_UART(9); //Set baudrate to 115.2kbps and initiate UART
	i2c_init(atmega18br, atmega18pc, communication_unit);
	while(1) {
		/*if(UART_not_empty()) {
			//uint8_t data = UART_get_buffer();
			//UART_putstring(data);
			//Send_to_PC('\n');
			i2c_send(sensor_unit, 0x52);
			UART_empty();
		}*/
		_delay_ms(1000);
		i2c_send(sensor_unit, 0x41);
		_delay_ms(5000);
	}
	return 0;
}