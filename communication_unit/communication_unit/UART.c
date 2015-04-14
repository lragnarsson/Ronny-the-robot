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
volatile int i=0;
volatile uint8_t buffer[20];
volatile uint8_t StrRxFlag=0;
ISR(USART0_RX_vect)
{
	buffer[i]=UDR0;
	if(buffer[i++]=='\r')
	{
		StrRxFlag=1;
		buffer[i-1]=0x00;
		i=0;
	}
}

void UART_Flush() {
	buffer[0] = '\0';
}

uint8_t UART_not_empty(void) {
	return StrRxFlag;
}

void UART_empty(void) {
	StrRxFlag = 0;
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


void UART_putstring(unsigned char* StringPtr) {
// sends the characters from the string one at a time to the USART
	while(*StringPtr != 0x00)
	{
		Send_to_PC(*StringPtr);
		StringPtr++;
	}
}

void Send_to_PC(unsigned char data) {

	while (!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}

unsigned char Receive_from_PC(void) {
	while (!(UCSR0A & (1<<RXC0)));
	
	return UDR0;
}
