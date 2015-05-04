/*
 * I2C.c
 *
 * Created: 2015-03-26 19:16:02
 *  Author: Filip
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "I2C.h"
#include <string.h>

// Initialize I2C
void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t address) {
	TWBR = bitrate;
	TWSR |= prescaler;
	TWAR = address | (1<<TWGCE); //Set address and general call enable
	TWCR = RESET;	//enable TWI, TWI interrupts, TWI enable ack
	
	read_start = 0;
	read_end = 0;
	write_start = 0;
	write_end = 0;
	memset(write_address, 0xFF, BUFFER_SIZE);
}

// Start condition
void i2c_start() {
	TWCR = START;
}

// STOP condition
void i2c_stop() {
	TWCR = STOP;
}

// Write multiple bytes to I2C
void i2c_write(uint8_t address, uint8_t* data, uint8_t length) {
	write_address[write_end] = address | 0;
	
	//if (write_start == write_end)
		//write_msg_end = (write_start + length) % BUFFER_SIZE;
	
	//memcpy(&write_buffer[write_end], data, length);
	
	for (uint8_t i = 0; i < length; ++i)
	{
		write_buffer[write_end] = data[i];
		
		write_end = (write_end + 1) % BUFFER_SIZE;
	}
	
	i2c_start();
}

// Write one byte to I2C
void i2c_write_byte(uint8_t address, uint8_t data) {
	write_address[write_end] = address | 0;
	write_buffer[write_end] = data;
	
	write_end = (write_end + 1) % BUFFER_SIZE;
	
	i2c_start();
}

ISR(TWI_vect) {
	switch (TWSR & 0xf8) {
		//status codes for master transmitter mode
		case 0x08: // Start condition has been transmitted
		case 0x10: // Repeated start condition has been transmitted
			TWDR = write_address[write_start];
			TWCR = SEND;
			write_address[write_start] = 0xFF;
			break;
		case 0x18: // SLA+W has been transmitted, ACK has been received
		case 0x28: // Data byte has been transmitted, ACK has been received
			if (write_start != write_end)
			{
				if (write_address[write_start] != 0xFF)
				{
					TWCR = START;	// Restart
				}
				else
				{
					TWDR = write_buffer[write_start];
					TWCR = SEND;
					write_start = (write_start + 1) % BUFFER_SIZE;	
				}
			}
			else
			{
				TWCR = STOP;
			}
			break;
		case 0x20: // SLA+W has been transmitted, NOT ACK has been received
			TWCR = START;
			break;

		case 0x30: // Data byte has been transmitted, NOT ACK has been received
			TWCR = START;
			break;
		case 0x38: //  Arbitration lost in SLA+W or data bytes
			TWCR = START;
			break;
			
		// Status codes for slave receiver mode
		case 0x60: // Own SLA+W has been received, ACK has been returned
			TWCR = ACK;
			break;
		case 0x68: // Arbitration lost in SLA+R/W as Master, own SLA+W has been received; ACK has been returned
			TWCR = ACK;
			break;
		case 0x70: // General call address has been received, ACK has been returned
			TWCR = ACK;
			break;
		case 0x78: //  Arbitration lost in SLA+R/W as Master, General call address has been received, ACK has been returned
			TWCR = NACK;
			break;
		case 0x80: // Previously addressed with own SLA+W, data has been received, ACK has been returned
		case 0x90: // Previously addressed with general call, data has been received, ACK has been returned
			read_buffer[read_end] = TWDR;
			TWCR = ACK;
			read_end = (read_end + 1) % BUFFER_SIZE;
			break;
		case 0x88: // Previously addressed with own SLA+W, data has been received, NOT ACK has been returned
		case 0x98: // Previously addressed with general call, data has been received, NOT ACK has been returned
			TWCR = NACK; // Switched to not addressed slave mode
			break;
		case 0xA0: // A STOP condition or repeated START condition has been received while still addressed as Slave
			TWCR = RESET; // Switched to the not addressed Slave mode, own SLA will be recognized
			handle_received_messages();
			break;
			
		// Misc. states
		case 0xF8: // No relevant state information available, TWINT = "0"
		case 0x00: // Bus error due to an illegal START or STOP condition
		default:
			break;
			
		// Master receiver mode and slave transmitter mode are not interesting
	}
}


/*
				switch(write_buffer[write_start]) {
					case AUTONOMOUS_MODE:
					case MANUAL_MODE:
					case DRIVE_FORWARD:
					case DRIVE_BACKWARD:
					case DRIVE_FORWARD_RIGHT:
					case DRIVE_FORWARD_LEFT:
					case TURN_RIGHT:
					case TURN_LEFT:
					case TAPE_FOUND:
						helpaddress = CONTROL_UNIT;
						write_msg_end = (write_msg_end + 1) % BUFFER_SIZE;
						break;
					case CALIBRATE_TAPE_SENSOR:
						helpaddress = SENSOR_UNIT;
						write_msg_end = (write_msg_end + 1) % BUFFER_SIZE;
						break;
					case P_PARAMETER:
					case D_PARAMETER:
						helpaddress = CONTROL_UNIT;
						write_msg_end = (write_msg_end + 2) % BUFFER_SIZE;
						break;
					case TAPE_SENSOR_VALUE:
						helpaddress = COMMUNICATION_UNIT;
						write_msg_end = (write_msg_end + 2) % BUFFER_SIZE;
					case ABSOLUTE_X_Y:
					case MAPPED_SQUARE:
					case MAPPED_WALL:
					case MAPPED_GOAL:
						helpaddress = COMMUNICATION_UNIT;
						write_msg_end = (write_msg_end + 3) % BUFFER_SIZE;
						break;
					case MOVED_DISTANCE_AND_ANGLE:
						helpaddress = CONTROL_UNIT;
						write_msg_end = (write_msg_end + 3) % BUFFER_SIZE;
						break;
					case SENSOR_READINGS:
						helpaddress = GENERAL_CALL;
						write_msg_end = (write_msg_end + 11) % BUFFER_SIZE;
						break;
					default:
						// Much is f'ed up
						break;
				}*/