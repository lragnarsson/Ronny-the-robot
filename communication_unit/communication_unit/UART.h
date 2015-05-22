/*
 * UART.h
 *
 * Created: 2015-03-31
 * Author: Filip
 */ 

#ifndef UART_H_
#define UART_H_

volatile uint8_t buffer[8];

void UART_get_buffer(uint8_t* data);
void UART_empty(void);
uint8_t UART_not_empty();
void Init_UART(uint16_t baudrate);
void UART_putstring(unsigned char* StringPtr);
void Send_to_PC(unsigned char data);
uint8_t Receive_from_PC(void);
void UART_Flush(void);

#endif /* UART_H_ */