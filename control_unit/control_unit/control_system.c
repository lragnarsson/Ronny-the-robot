﻿/*
 * control_system.c
 *
 */
#include "control_system.h"

const uint8_t ANGLE_CHANGE_THRESHOLD = 20;
volatile uint8_t P_COEFFICIENT = 1;
volatile uint8_t D_COEFFICIENT = 1;

uint8_t desired_speed = 0;
volatile uint8_t current_speed = 0;
uint8_t control_speed = 0;
uint8_t control_speed_max = 0;

/* Initialize hardware ports */
void init_control_system() {
	TCCR0A = (0<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0); // PWM for left and right engine
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00); // PWM for left and right engine
	DDRB = (1<<DDB0)|(1<<DDB1)|(1<<DDB3)|(1<<DDB4);	// Left and right engine speed and direction as output
	DDRD = 0<<DDD1; // Input, reset button
	TCCR1A = TIMER1_CLEAR_ON_MATCH_L;
	TCCR1B = TIMER1_CLEAR_ON_MATCH_H | TIMER1_PRESCALE_64;
	TIMSK1 = TIMER1_INTERRUPT_ENABLE;
	OCR1A = TIMER1_MATCH_FREQUENCY_100HZ;
	sei();
}

/* Service routine for handling acceleration */
ISR(TIMER1_COMPA_vect) {
	if(desired_speed > current_speed)
		++current_speed;
	else if(desired_speed < current_speed)
		--current_speed;
	control_speed_max = 0.49 * current_speed;
}

/* Set desired_speed */
void set_desired_speed(uint8_t speed) {
	desired_speed = speed;
}

/* Stop engines by settings PWM duty cycles to 0. Direction unaffected */
void stop_engines() {
	set_desired_speed(0);
	current_speed = 0;
	ENGINE_LEFT_SPEED =	ENGINE_RIGHT_SPEED = 0;
}

void set_same_engine_speed() {
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = current_speed;
}

/* Calculates the control_speed based on current angle and error in distance to the middle of the corridor */
void calculate_control_speed() {
	uint16_t control_speed_16 = P_COEFFICIENT * current_distance_error + D_COEFFICIENT * current_angle_error;
	if (control_speed_16 > control_speed_max)
	control_speed = control_speed_max;
	else
	control_speed = (uint8_t) control_speed_16;
}

void set_controlled_engine_speed() {
	calculate_control_speed();
	ENGINE_LEFT_SPEED = current_speed - control_speed;
	ENGINE_RIGHT_SPEED = current_speed + control_speed;
}

void set_manual_forward_left_engine_speed() {
	ENGINE_LEFT_SPEED = current_speed - 0.1 * current_speed;
	ENGINE_RIGHT_SPEED = current_speed +  0.1 * current_speed;
}

void set_manual_forward_right_engine_speed() {
	ENGINE_LEFT_SPEED = current_speed + 0.1 * current_speed;
	ENGINE_RIGHT_SPEED = current_speed - 0.1 * current_speed;
}

/* Absolute value of linear approximation of left "angle" derivative */
uint8_t get_angle_change_left() {
	if (current_angle_left > last_tick_angle_left)
		return inverse_sampling_speed * (current_angle_left - last_tick_angle_left);
	return inverse_sampling_speed * (last_tick_angle_left - current_angle_left);
}

/* Absolute value of linear approximation of right "angle" derivative */
uint8_t get_angle_change_right() {
	if (current_angle_right > last_tick_angle_right)
		return inverse_sampling_speed * (current_angle_right - last_tick_angle_right);
	return inverse_sampling_speed * (last_tick_angle_right - current_angle_right);
}

uint8_t corner_detected_left() {
	return (uint8_t)(get_angle_change_left() > ANGLE_CHANGE_THRESHOLD);
}

uint8_t corner_detected_right() {
	return (uint8_t)(get_angle_change_right() > ANGLE_CHANGE_THRESHOLD);	
}