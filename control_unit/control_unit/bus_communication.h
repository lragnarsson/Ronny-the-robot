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

volatile uint8_t current_angle;
volatile uint8_t current_angle_left;
volatile uint8_t current_angle_right;
volatile uint8_t current_distance_error;
volatile uint8_t last_tick_angle_left;
volatile uint8_t last_tick_angle_right;
uint8_t inverse_sampling_speed;

void init_bus_communication();

#endif /* BUS_COMMUNICATION_H_ */