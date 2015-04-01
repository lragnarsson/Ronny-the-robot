/*
 * UART.h
 *
 * Created: 2015-03-31
 * Author: Filip
 */ 

#ifndef UART_H_
#define UART_H_
/*volatile int i=0;
volatile uint8_t buffer[20];
volatile uint8_t StrRxFlag=0;*/
uint8_t UART_not_empty(void);
uint8_t UART_get_buffer(void);
void UART_empty(void);
void Init_UART(uint16_t baudrate);
void UART_putstring(char* StringPtr);
void Send_to_PC(unsigned char data);
unsigned char Receive_from_PC(void);
void UART_Flush(void);
#endif /* UART_H_ */