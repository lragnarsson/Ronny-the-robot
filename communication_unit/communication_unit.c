/*
 * communication.c
 *
 * Created: 2015-03-23 22:37:02
 *  Author: ostma_000
 */ 
#include <avr/io.h>
#define F_CPU 18432000UL
#include <avr/interrupt.h>
#include <util/delay.h>
/*
 * Initiate the UART
 */
/*
void UART_Flush() {
	unsigned char flush;
	while (UCSR0A & (1<<RXC0))flush = UDR0;
}
*/

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
void Init_UART(uint16_t baudrate) {
	cli(); //Disable interrupts
	unsigned int baud = baudrate;
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L |= (unsigned char)baud;
	//DDRD = 0<<DDD1; // RX in, TX out
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); //Enable RX, TX and Rx interrupt
	UCSR0C |= (0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00); //Format 8 data bits and one stop bit
	PORTB |= (1<<PB5); //input
	//DDRD = (1<<PORTD1);
	//UART_Flush();
	//DDRD |= 0x02;
	//PORTD |= (1<< PIND1);
	sei();
}


void UART_putstring(char* StringPtr)
// sends the characters from the string one at a time to the USART
{
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

int main(void)
{
	Init_UART(9); //Set baudrate to 115.2kkbps and initiate UART
	unsigned char debugg;
	/*
	while(1) {
		//debugg = Receive_from_PC();
		debugg = Receive_from_PC();
		Send_to_PC(debugg);
		_delay_ms(100);
	} */
	while(1)
	{
		if(StrRxFlag)
		{
			UART_putstring(buffer);
			Send_to_PC('\n');
			StrRxFlag=0;
		}
	}
	return 0;
}