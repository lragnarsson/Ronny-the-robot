/*
 * I2C.c
 *
 * Created: 2015-03-26 19:16:02
 *  Author: Filip
 */ 

#define atmega20br 25 // scl freq 92593Hz
#define atmega18br 91 // scl freq 93091Hz
#define atmega20pc 1 // prescaler atmega 20kHz clock
#define atmega18pc 0 // prescaler atmega 18.432kHz clock
#define general_call 0x00
#define control_unit 0x12
#define sensor_unit 0x14
#define communication_unit 0x16
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "I2C.h"
volatile uint8_t busbuffer;
volatile uint8_t helpadress = communication_unit;
volatile uint8_t helpdata = 0x12;

//TWCR BITS(TWxx):   INT EA STA STO WC EN Res IE
#define SEND 0xc5 // 1   1  0   0   0  1  0   1
#define STOP 0xd5 // 1   1  0   1   0  1  0   1
#define START 0xe5// 1   1  1   0   0  1  0   1
#define ACK 0xc5  // 1   1  0   0   0  1  0   1
#define NACK 0x85 // 1   0  0   0   0  1  0   1
#define RESET 0xc5// 1   1  0   0   0  1  0   1

//Init
void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t adress)
{
	//SCL 
	cli();
	TWBR = bitrate;
	TWSR |= prescaler;
	TWAR = adress | (1<<TWGCE); //Set adress and general call enable
	TWCR = RESET;	//enable TWI, TWI interrupts, TWI enable ack
	sei();
}

uint8_t i2c_get_buffer() {
	return busbuffer;
}

void i2c_clear_buffer() {
	busbuffer = 0x00;
}

//START condition
/*
uint8_t i2c_start(){
	uint8_t status_code;
	cli();
	TWCR = (1<<TWINT)|(1<<TWSTA)|(0<<TWSTO)|(1<<TWEN);

	//Wait for flag to be set
	while (!(TWCR & (1<<TWINT)));
	sei();
	status_code = TWSR & 0xf8; //Check status on bus
	
	
	//atmega1284p p.220
	switch (status_code) {
		case 0x08: // Start
		case 0x10: // Repeated start
			return 0;
		//Something went wrong
		case 0x00: //Buss error p.230
			cli();
			
			//TWCR &= 0xff ^ (1 << TWSTA);
			//TWCR |= (1 << TWINT) | (1 << TWSTO);
			
			TWCR = (1<<TWINT) | (0<<TWSTA) | (1<<TWSTO);
			while (!(TWCR & (1 << TWINT)));
			sei();
			return 1;
		case 0xf8: // No relevant state information, try again
		default:
			return i2c_start();
	}
}
*/
void i2c_start() {
	TWCR = START;
	//while (!(TWCR & (1<<TWINT)));
}

//STOP condition
void i2c_stop(){
	TWCR = STOP;
	//while (!(TWCR & (1<<TWINT)));
}

//Write to I2C

uint8_t i2c_write(uint8_t adress, uint8_t data) {
	helpadress = adress | 0;
	helpdata = data;
	i2c_start();
	return 0;
}
/*
uint8_t i2c_write(uint8_t adress, uint8_t data){
	uint8_t status_code;
	cli();
	TWDR = adress | 0; //Send SLA+W
	TWCR = (1<<TWINT)|(1<<TWEN); //Set when all data on TWDR
	while (!(TWCR & (1<<TWINT))); //Make sure TWINT is set
	sei(); // Set enable interrupts
	status_code = TWSR & 0xf8; //Check bus status
	
	
	switch(status_code) {
		case 0x18: // SLA+W has been transmitted, ACK has been received
		case 0x28:
			cli();
			TWDR = data;
			TWCR = (0<<TWSTA)|(0<<TWSTO)|(1<<TWINT);
			while (!(TWCR & (1<<TWINT))); //Make sure TWINT is set
			sei();
			status_code = TWSR & 0xf8; //Check bus status
			break;
		case 0x20: // SLA+W has been transmitted, NOT ACK has been received
			i2c_stop();
			return 1;
		case 0x38: // Arbitration lost in SLA+W or data bytes
			cli();
			TWCR = (1<<TWSTA)|(0<<TWSTO)|(1<<TWINT);
			while (!(TWCR & (1<<TWINT)));
			sei();
			return 2;
		default:
			return i2c_start();
	}
	
	switch(status_code) {
		case 0x18:
		case 0x28: // Data byte has been transmitted, ACK has been received
			cli();
			TWCR = (0<<TWSTA)|(1<<TWSTO)|(1<<TWINT);
			while (!(TWCR & (1<<TWINT)));
			sei();
			return 0;
		case 0x30: // Data byte has been transmitted, NOT ACK has been received
			i2c_stop();
			return 3;
		default:
			return 4;
	}
}
*/
/*
uint8_t i2c_send(uint8_t adress, uint8_t data) {
	if(i2c_start() == 0) {
		switch(i2c_write(adress, data)) {
			case 0:
				return 0;
			default:
				return 1;
		}
	}
	return 2;
}
*/
ISR(TWI_vect) {
	switch (TWSR & 0xf8) {
		//status codes for master transmitter mode
		case 0x08: // Start condition has been transmitted
		case 0x10: // Repeted start condition has been transmitted
			TWDR = helpadress;
			TWCR = SEND;
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x18: // SLA+W has been transmitted, ACK has been received
		case 0x28: // Data byte has been transmitted, ACK has been received
			TWDR = helpdata;
			TWCR = SEND;
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x20: // SLA+W has been transmitted, NOT ACK has been received
			TWDR = helpdata;
			TWCR = SEND;
			//while (!(TWCR & (1<<TWINT)));
			break;

		case 0x30: // Data byte has been transmitted, NOT ACK has been received
			TWCR = START;
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x38: //  Arbitration lost in SLA+W or data bytes
			TWCR = START;
			//while (!(TWCR & (1<<TWINT)));
			break;
			
			
		// Status codes for slave receiver mode
		case 0x60: // Own SLA+W has been received, ACK has been returned
			TWCR = ACK; //Send ack
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x68: // Arbitration lost in SLA+R/W as Master, own SLA+W has been received; ACK has been returned
			TWCR = ACK; //Send ack
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x70: // General call address has been received, ACK has been returned
			TWCR = ACK; //Send ack
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x78: //  Arbitration lost in SLA+R/W as Master, General call address has been received, ACK has been returned
			TWCR = NACK; //Send nack
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x80: // Previously addressed with own SLA+W, data has been received, ACK has been returned
			busbuffer = TWDR;
			TWCR = NACK; //Send ack
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x90: // Previously addressed with general call, data has been received, ACK has been returned
			busbuffer = TWDR;
			TWCR = ACK; //Send ack
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0x88: // Previously addressed with own SLA+W, data has been received, NOT ACK has been returned
		case 0x98: // Previously addressed with general call, data has been received, NOT ACK has been returned
			TWCR = NACK; // Switched to not adressed slave mode
			//while (!(TWCR & (1<<TWINT)));
			break;
		case 0xA0: // A STOP condition or repeated START condition has been received while still addressed as Slave
			TWCR = RESET; // Switched to the not addressed Slave mode, own SLA will be recognized
			//while (!(TWCR & (1<<TWINT)));
			break;
		// Misc. states
		case 0xF8: // No relevant state information available, TWINT = "0"
		case 0x00: // Bus error due to an illegal START or STOP condition
		default:
			break;
		// Master receiver mode and slave transmitter mode are not interesting
	}
}
