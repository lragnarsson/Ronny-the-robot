/* 
 * UART.h
 * Ronny-the-robot/communication_unit
 * ------------------------------
 * This file contains the functions required to send and receive data to and from the PC-module.
 * ------------------------------ 
 * Author: F. Östman
 */ 


#ifndef UART_H_
#define UART_H_

volatile uint8_t buffer[8];
volatile uint8_t UART_flag;

void UART_get_buffer(uint8_t* data);
void UART_empty(void);
uint8_t UART_not_empty();
void Init_UART(uint16_t baudrate);
void Send_to_PC(unsigned char data);
uint8_t Receive_from_PC(void);
void UART_Flush(void);

#endif /* UART_H_ */