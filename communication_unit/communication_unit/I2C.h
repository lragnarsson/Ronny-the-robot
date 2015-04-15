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

#define ABSOLUTE_X_Y 0x30
#define AUTONOMOUS_MODE 0x31
#define MANUAL_MODE 0x32
#define MAPPED_SQUARE 0x33 // 3 bytes
#define MAPPED_WALL 0x34 // 3 bytes
#define MAPPED_GOAL 0x35 // 3 bytes
#define SENSOR_READINGS 0x40 // 6 bytes
#define MOVED_DISTANCE_AND_ANGLE 0x41 // 3 bytes
#define TAPE_FOUND 0x42
#define DRIVE_FORWARD 0xC1
#define TURN_RIGHT 0xC2
#define TURN_LEFT 0xC3
#define DRIVE_BACKWARD 0xC4
#define DRIVE_FORWARD_RIGHT 0xC5
#define DRIVE_FORWARD_LEFT 0xC6
#define CALIBRATE_TAPE_SENSOR 0xD0



volatile uint8_t busbuffer[16];
volatile uint8_t receiverstart;
volatile uint8_t receiverstop;

//Init
void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t adress);

//START condition
void i2c_start();

//STOP condition
void i2c_stop();

//Getter for bussbuffer
uint8_t i2c_get_buffer(uint8_t*);

void i2c_clear_buffer(void);

//Write to I2C
uint8_t i2c_write(uint8_t adress, uint8_t* data, uint8_t length);

void handle_recieved_message();

//Send by bus
//uint8_t i2c_send(uint8_t adress, uint8_t data);
#endif /* I2C_H_ */