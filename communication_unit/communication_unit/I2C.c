/*
 * I2C.c
 *
 * Created: 2015-03-26 19:16:02
 *  Author: Filip och Emma
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
uint16_t busbuffer;
//Init
void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t adress)
{
	//SCL 
	TWBR = bitrate;
	TWSR |= prescaler;
	TWAR = adress; //Set adress and general call enable
	TWAR |= (1<<TWGCE);
	TWCR = (1<<TWEN);	//enable TWI
	sei();
}

//START condition
uint8_t i2c_start(){
	uint8_t status_code;
	cli();
	TWCR = (1<<TWINT)|(1<<TWSTA)|(0<<TWSTO)|(1<<TWEN);

	//Wait for flag to be set
	while (!(TWCR & (1<<TWINT)));
	status_code = TWSR & 0xf8; //Check status on bus
	sei();
	
	//atmega1284p p.220
	switch (status_code) {
		case 0x08: // Start
		case 0x10: // Repeated start
		return 0;
		//Something went wrong
		/*case 0x00: //Buss error p.230
		cli();
		TWCR &= 0xff ^ (1 << TWSTA);
		TWCR |= (1 << TWINT) | (1 << TWSTO);
		while (!(TWCR & (1 << TWINT)));
		sei();*/
		//case 0xf8:
		default:
		// Catastrophic failure, retry
		return i2c_start();
	}
}

//STOP condition
void i2c_stop(){
	TWCR |= (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

//Write to I2C
uint8_t i2c_write(uint8_t data){
	uint8_t status_code;
	cli();
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN); //Set when all data on TWDR
	while (!(TWCR & (1<<TWINT))); //Make sure TWINT is set
	status_code = TWSR & 0xf8; //Check bus status
	sei();
	return status_code;
}

//Read ACK
uint8_t i2c_RACK(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}
ISR(TWI_vect) {
	switch (TWSR & 0xf8) {
		//status codes for master transmitter mode
		case 0x08: // Start condition has been transmitted
		case 0x10: // Repeted start condition has been transmitted
		case 0x18: // SLA+W has been transmitted, ACK has been received
		case 0x20: // SLA+W has been transmitted, NOT ACK has been received
		case 0x28: // Data byte has been transmitted, ACK has been received
		case 0x30: // Data byte has been transmitted, NOT ACK has been received
		case 0x38: //  Arbitration lost in SLA+W or data bytes
		
		// Status codes for slave receiver mode
		case 0x60: // Own SLA+W has been received, ACK has been returned
		case 0x68: // Arbitration lost in SLA+R/W as Master, own SLA+W has been received; ACK has been returned
		case 0x70: // General call address has been received, ACK has been returned
		case 0x78: //  Arbitration lost in SLA+R/W as Master, General call address has been received, ACK has been returned
		case 0x80: // Previously addressed with own SLA+W, data has been received, ACK has been returned
		case 0x88: // Previously addressed with own SLA+W, data has been received, NOT ACK has been returned
		case 0x90: // Previously addressed with general call, data has been received, ACK has been returned
		case 0x98: // Previously addressed with general call, data has been received, NOT ACK has been returned
		case 0xA0: // A STOP condition or repeated START condition has been received while still addressed as Slave
		
		// Misc. states
		case 0xF8: // No relevant state information available, TWINT = "0"
		case 0x00: // Bus error due to an illegal START or STOP condition
		
		// Master receiver mode and slave transmitter mode are not interesting
	}
}
