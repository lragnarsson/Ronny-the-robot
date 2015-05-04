/*
 * I2C.h
 *
 * Created: 2015-03-31
 *  Author: Filip
 */ 
#ifndef I2C_H_
#define I2C_H_

// Set SCL speed around 100kHz
#define atmega20br 20 // scl freq 92593Hz 25
#define atmega18br 18 // scl freq 93091Hz 91
#define atmega20pc 0 // prescaler atmega 20kHz clock 1
#define atmega18pc 0 // prescaler atmega 18.432kHz clock

/* I2C adresses */
#define GENERAL_CALL 0x00
#define CONTROL_UNIT 0x12
#define SENSOR_UNIT 0x14
#define COMMUNICATION_UNIT 0x16

/* Headers */
#define ABSOLUTE_X_Y 0x30	// 3 bytes
#define AUTONOMOUS_MODE 0x31
#define MANUAL_MODE 0x32
#define MAPPED_SQUARE 0x33 // 3 bytes
#define MAPPED_WALL 0x34 // 3 bytes
#define MAPPED_GOAL 0x35 // 3 bytes
#define SENSOR_READINGS 0x40 // 11 bytes
#define MOVED_DISTANCE_AND_ANGLE 0x41 // 3 bytes
#define TAPE_FOUND 0x42
#define TAPE_SENSOR_VALUE 0x70 // 2 bytes
#define DRIVE_FORWARD 0xC1
#define TURN_RIGHT 0xC2
#define TURN_LEFT 0xC3
#define DRIVE_BACKWARD 0xC4
#define DRIVE_FORWARD_RIGHT 0xC5
#define DRIVE_FORWARD_LEFT 0xC6
#define P_PARAMETER 0xC7 // 2 bytes
#define D_PARAMETER 0xC8 // 2 bytes
#define CALIBRATE_TAPE_SENSOR 0xD0

/* TWCR definitions */
#define SEND 0xc5 // 1   1  0   0   0  1  0   1
#define STOP 0xd5 // 1   1  0   1   0  1  0   1
#define START 0xe5// 1   1  1   0   0  1  0   1
#define ACK 0xc5  // 1   1  0   0   0  1  0   1
#define NACK 0x85 // 1   0  0   0   0  1  0   1
#define RESET 0xc5// 1   1  0   0   0  1  0   1

/* Read and write buffers */
#define BUFFER_SIZE 64

volatile uint8_t write_buffer[BUFFER_SIZE];
volatile uint8_t write_start;
volatile uint8_t write_end;
volatile uint8_t write_address[BUFFER_SIZE];

volatile uint8_t read_buffer[BUFFER_SIZE];
volatile uint8_t read_start;
volatile uint8_t read_end;

void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t address);
void i2c_start();
void i2c_stop();
void i2c_write(uint8_t address, uint8_t* data, uint8_t length);
void i2c_write_byte(uint8_t adress, uint8_t data);

void handle_received_messages();

#endif /* I2C_H_ */