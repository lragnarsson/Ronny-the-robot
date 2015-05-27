/*
 * UART.c
 *
 * Created: 2015-03-31
 *  Author: Filip
 */ 
#include <avr/io.h>
#define F_CPU 18432000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"
#include "I2C.h"

ISR(USART0_RX_vect)
{
	static uint8_t i = 0;
	static uint8_t expected_length = 1;
	
	buffer[i]=UDR0;
	switch (buffer[i])
	{
		case DRIVE_FORWARD:
		case DRIVE_BACKWARD:
		case TURN_LEFT:
		case TURN_RIGHT:
		case DRIVE_FORWARD_LEFT:
		case DRIVE_FORWARD_RIGHT:
			expected_length = 1;
			break;
		case P_PARAMETER:
		case D_PARAMETER:
		case MOTOR_TRIM:
			expected_length = 2;
			break;
		default:
			break;
	}
	
	if (++i == expected_length)
	{
		UART_flag = 1;
		i = 0;
		expected_length = 1;
	}
}

void UART_Flush() {
	buffer[0] = '\0';
}

uint8_t UART_not_empty() {
	return UART_flag;
}

void UART_empty(void) {
	cli();
	UART_flag = 0;
	for(uint8_t j=0; j<8; j++) {
		buffer[j] = 0x00;
	}
	sei();
}

void UART_get_buffer(uint8_t* data) {
	for(uint8_t j=0; (!(buffer[j]==0x00)); j++) {
	data[j] = buffer[j];
	}
}

void Init_UART(uint16_t baudrate) {
	cli(); //Disable interrupts
	unsigned int baud = baudrate;
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L |= (unsigned char)baud;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); //Enable RX, TX and Rx interrupt
	UCSR0C |= (0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00); //Format 8 data bits and one stop bit
	PORTB |= (1<<PB5); //input
	UART_Flush();
	sei();
}

void Send_to_PC(unsigned char data) {

	while (!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}

unsigned char Receive_from_PC(void) {
	while (!(UCSR0A & (1<<RXC0)));
	
	return UDR0;
}
