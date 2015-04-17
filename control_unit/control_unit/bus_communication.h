/*
 * bus_communication.h
 *
 */ 

#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#ifndef BUS_COMMUNICATION_H_
#define BUS_COMMUNICATION_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "I2C.h"

typedef enum { M_FORWARD, M_LEFT, M_RIGHT, M_FORWARD_LEFT, M_FORWARD_RIGHT, M_BACKWARD, DO_NOTHING } manual_command;

volatile manual_command last_manual_command;

volatile uint8_t goal_found;

volatile uint16_t left_wall_distance;
volatile uint16_t right_wall_distance;
volatile uint16_t front_wall_distance;
volatile uint8_t current_angle_error;
volatile uint8_t current_angle_left;
volatile uint8_t current_angle_right;
volatile uint8_t current_distance_error;
volatile uint8_t last_tick_angle_left;
volatile uint8_t last_tick_angle_right;
volatile uint16_t distance_remaining;
volatile uint16_t square_distance_remaining;
volatile uint8_t angle_remaining;

uint8_t inverse_sampling_speed;

void init_bus_communication();

void handle_received_message();
void update_sensor_readings(uint8_t rear_left_h, uint8_t rear_left_l, uint8_t front_left_h, uint8_t front_left_l,
							uint8_t rear_right_h, uint8_t rear_right_l, uint8_t front_right_h, uint8_t front_right_l, uint8_t front_h, uint8_t front_l);
void update_distance_and_angle(uint8_t distance, uint8_t angle);
void tape_found();
void manual_drive_forward();
void manual_turn_right();
void manual_turn_left();
void manual_drive_forward_right();
void manual_drive_forward_left();
void manual_drive_backward();
void handle_recieved_message();


#endif /* BUS_COMMUNICATION_H_ */