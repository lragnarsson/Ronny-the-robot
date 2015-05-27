/* 
 * I2C.c
 * Ronny-the-robot/sensor_unit
 * ------------------------------
 * This file contains the functions required to use the I2C-bus.
 * ------------------------------ 
 * Author: F. Östman
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "I2C.h"

volatile uint8_t helpaddress;
volatile uint8_t helpdata[16];
volatile uint8_t startpointer;
volatile uint8_t endpointer;

/*
 *  Init I2C.
 */
void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t address) {
	receiverstart = 0x00;
	receiverstop = 0x00;
	TWBR = bitrate;
	TWSR |= prescaler;
	TWAR = address | (1<<TWGCE);		//Set address and enable General Call
	TWCR = RESET;
	is_sending = 0;
}

/*
 * Clear the buffer to prepare for new data.
 */
void i2c_clear_buffer() {
	memset(busbuffer, 0, sizeof(busbuffer));
	receiverstart = 0x00;
	receiverstop = 0x00;
}

void i2c_start() {
	TWCR = START;
}

/* 
 * Write multiple bytes to I2C.
 */
uint8_t i2c_write(uint8_t address, uint8_t* data, uint8_t length) {
	if(!is_sending) {
		helpaddress = address | 0; // Write
		startpointer = 0x00;
		endpointer = 0x00;
		while(endpointer < length) {
			helpdata[endpointer] = data[endpointer];
			endpointer++;
		}
		is_sending = 0x01;
		i2c_start();
		return 1;
	}
	else
		return 0;
}

/* 
 * Write one byte to I2C.
 */
uint8_t i2c_write_byte(uint8_t address, uint8_t data) {
	if (!is_sending) {
		helpaddress = address | 0; //Write
		startpointer = 0x00;
		endpointer = 0x01;
		helpdata[0] = data;
		is_sending = 0x01;
		i2c_start();
		return 1;
	}
	else
		return 0;
}

/* 
 * Interrupt vector 
 */
ISR(TWI_vect) {
	switch (TWSR & 0xf8) {
		//Status codes for master transmitter mode
		case 0x08:									// Start condition has been transmitted
		case 0x10:									// Repeted start condition has been transmitted
			TWDR = helpaddress;
			TWCR = SEND;
			break;
		case 0x18:									// SLA+W has been transmitted, ACK has been received
		case 0x28:									// Data byte has been transmitted, ACK has been received
			if(startpointer < endpointer) {
				TWDR = helpdata[startpointer++];
				TWCR = SEND;
			}
			else {
				startpointer = 0x00;
				is_sending = 0x00;
				TWCR = STOP;
			}
			break;
		case 0x20:									// SLA+W has been transmitted, NOT ACK has been received
			TWCR = START;
			break;
		case 0x30:									// Data byte has been transmitted, NOT ACK has been received
			TWCR = START;
			break;
		case 0x38:									// Arbitration lost in SLA+W or data bytes
			TWCR = START;
			break;
			
		// Status codes for slave receiver mode
		case 0x60:									// Own SLA+W has been received, ACK has been returned
			TWCR = ACK;
			break;
		case 0x68:									// Arbitration lost in SLA+R/W as Master, own SLA+W has been received; ACK has been returned
			TWCR = ACK;
			break;
		case 0x70:									// General call address has been received, ACK has been returned
			TWCR = ACK;
			break;
		case 0x78:									//  Arbitration lost in SLA+R/W as Master, General call address has been received, ACK has been returned
			TWCR = NACK;
			break;
		case 0x80:									// Previously addressed with own SLA+W, data has been received, ACK has been returned
		case 0x90:									// Previously addressed with general call, data has been received, ACK has been returned
			busbuffer[receiverstop++] = TWDR;
			TWCR = ACK;
			break;
		case 0x88:									// Previously addressed with own SLA+W, data has been received, NOT ACK has been returned
		case 0x98:									// Previously addressed with general call, data has been received, NOT ACK has been returned
			TWCR = NACK;							// Switched to not addressed slave mode
			break;
		case 0xA0:									// A STOP condition or repeated START condition has been received while still addressed as Slave
			TWCR = RESET;							// Switched to the not addressed Slave mode, own SLA will be recognized
			handle_received_message();
			i2c_clear_buffer();
			break;
			
		// Misc. states
		case 0xF8:									// No relevant state information available, TWINT = "0"
		case 0x00:									// Bus error due to an illegal START or STOP condition
		default:
			break;
	}
}
