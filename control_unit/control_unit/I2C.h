/*
 * I2C.h
 *
 * Created: 2015-03-31
 *  Author: Filip
 */ 
#ifndef I2C_H_
#define I2C_H_

// Set SCL speed around 100kHz
#define BITRATE_20MHZ 20 // scl freq 92593Hz 25
#define BITRATE_18MHZ 18 // scl freq 93091Hz 91
#define PRESCALER_20MHZ 0 // prescaler atmega 20kHz clock 1
#define PRESCALER_18MHZ 0 // prescaler atmega 18.432kHz clock


#define GENERAL_CALL 0x00
#define CONTROL_UNIT 0x12
#define SENSOR_UNIT 0x14
#define COMMUNICATION_UNIT 0x16

/* Headers */
#define ABSOLUTE_X_Y 0x30				// 3 bytes
#define AUTONOMOUS_MODE 0x31
#define MANUAL_MODE 0x32
#define MAPPED_SQUARE 0x33				// 3 bytes
#define MAPPED_WALL 0x34				// 3 bytes
#define MAPPED_GOAL 0x35				// 3 bytes
#define SENSOR_READINGS 0x40			// 11 bytes
#define MOVED_DISTANCE_AND_ANGLE 0x41	// 3 bytes
#define TAPE_FOUND 0x42
#define TAPE_SENSOR_VALUE 0x70			// 2 bytes
#define DRIVE_FORWARD 0xC1
#define TURN_RIGHT 0xC2
#define TURN_LEFT 0xC3
#define DRIVE_BACKWARD 0xC4
#define DRIVE_FORWARD_RIGHT 0xC5
#define DRIVE_FORWARD_LEFT 0xC6
#define P_PARAMETER 0xC7				// 2 bytes
#define D_PARAMETER 0xC8				// 2 bytes
#define MOTOR_TRIM 0xC9					// 2 bytes
#define CALIBRATE_TAPE_SENSOR 0xD0



volatile uint8_t busbuffer[16];
volatile uint8_t receiverstart;
volatile uint8_t receiverstop;

volatile uint8_t is_sending;

// Init
void i2c_init(uint8_t bitrate, uint8_t prescaler, uint8_t address);

// START condition
void i2c_start();

void i2c_clear_buffer(void);

// Write to I2C
uint8_t i2c_write(uint8_t address, uint8_t* data, uint8_t length);
uint8_t i2c_write_byte(uint8_t address, uint8_t data);

void handle_received_message();

#endif /* I2C_H_ */