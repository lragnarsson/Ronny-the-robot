/*
 * bus_communication.h
 *
 */ 

#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#ifndef BUS_COMMUNICATION_H_
#define BUS_COMMUNICATION_H_

#define SAMPLE_FREQUENCY 25

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "I2C.h"

typedef enum { M_FORWARD, M_LEFT, M_RIGHT, M_FORWARD_LEFT, M_FORWARD_RIGHT, M_BACKWARD, DO_NOTHING } manual_command;

volatile manual_command last_manual_command;

volatile uint8_t tape_square;

volatile uint16_t left_wall_distance;
volatile uint16_t right_wall_distance;
volatile uint16_t front_wall_distance;

volatile int16_t current_distance_error;
volatile int16_t current_derivative_error;

volatile int16_t distance_travelled;
volatile int16_t absolute_rotation;

void init_bus_communication();
void handle_received_message();
void update_sensor_readings(uint8_t rear_left_h, uint8_t rear_left_l, uint8_t front_left_h, uint8_t front_left_l,
							uint8_t rear_right_h, uint8_t rear_right_l, uint8_t front_right_h, uint8_t front_right_l, uint8_t front_h, uint8_t front_l);

#endif /* BUS_COMMUNICATION_H_ */