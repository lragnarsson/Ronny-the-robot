/*
 * I2C.h
 *
 * Created: 2015-03-31
 *  Author: Filip
 */ 
#ifndef I2C_H_
#define I2C_H_

#define atmega20br 25 // scl freq 92593Hz
#define atmega18br 91 // scl freq 93091Hz
#define atmega20pc 1 // prescaler atmega 20kHz clock
#define atmega18pc 0 // prescaler atmega 18.432kHz clock
#define general_call 0x00
#define control_unit 0x12
#define sensor_unit 0x14
#define communication_unit 0x16
//Init
void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t adress);

//START condition
void i2c_start();

//STOP condition
void i2c_stop();

//Getter for bussbuffer
uint8_t i2c_get_buffer(void);

void i2c_clear_buffer(void);

//Write to I2C
uint8_t i2c_write(uint8_t adress, uint8_t data);

//Send by bus
uint8_t i2c_send(uint8_t adress, uint8_t data);
#endif /* I2C_H_ */