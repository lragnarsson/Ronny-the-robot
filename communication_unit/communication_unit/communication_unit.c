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
	i2c_start();
	i2c_write(0xF3);
	i2c_stop();
	while(1) {
		if(UART_not_empty()) {
			UART_putstring(UART_get_buffer());
			Send_to_PC('\n');
			UART_empty();
		}
	}
	return 0;
}