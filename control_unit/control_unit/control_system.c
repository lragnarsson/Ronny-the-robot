/* 
 * control_system.c
 * Ronny-the-robot/control_unit
 * ------------------------------
 * This file contains the functions for controlling the motors.
 * ------------------------------ 
 * Author: L. Ragnarsson, E. Sköld
 */ 

#include "control_system.h"
#include "mult16x16.h"

volatile uint8_t desired_engine_speed = 0;
volatile uint8_t current_engine_speed = 0;
volatile engine_direction desired_engine_direction = ENGINE_DIRECTION_FORWARD;

/*
 * Initializes hardware ports and sets control parameters.
 */
void init_control_system()
{
	TCCR0A = TIMER0_CLEAR_ON_MATCH_H; // PWM for left and right engine
	TCCR0B = TIMER0_CLEAR_ON_MATCH_L; // PWM for left and right engine
	DDRB = ENGINE_OUTPUT; // Left and right engine speed and direction as output
	
	TCCR1A = TIMER1_CLEAR_ON_MATCH_L;
	TCCR1B = TIMER1_CLEAR_ON_MATCH_H | TIMER1_PRESCALE_64;
	TIMSK1 = TIMER1_INTERRUPT_ENABLE;
	OCR1A = TIMER1_MATCH_FREQUENCY_500HZ;

	DDRD = (0<<DDD0)|(1<<DDD7);
	
	TCCR2A = (1<<COM2A1) | (0<<COM2A0) | (0<<WGM21) | (1<<WGM20);
	TCCR2B = (0<<WGM22) | (1<<CS22) | (0<<CS21) | (1<<CS20);
	
	/* Control parameters */
	P_COEFFICIENT = 0x0C80;
	D_COEFFICIENT = 0x1220;
	
	motor_trim = -5;
}

/* 
 * Service routine for handling acceleration and setting motor speeds fron control signals.
 */
ISR(TIMER1_COMPA_vect)
{
	if (current_mode == MANUAL)
		return;
		
	/* Acceleration */
	if(desired_engine_speed > current_engine_speed)
	{
		if (desired_engine_speed - current_engine_speed < 5)
			current_engine_speed = desired_engine_speed;
		else
			current_engine_speed += 5;
	}
	else if(desired_engine_speed < current_engine_speed)
	{
		if (current_engine_speed < 5)
			current_engine_speed = 0;
		else
			current_engine_speed -= 5;
	}
	
	/* Automatic control */
	uint8_t max_control_speed = current_engine_speed / 2;
	
	int32_t p;
	MultiSU16X16to32(p, current_distance_error, P_COEFFICIENT);
	int32_t d;
	MultiSU16X16to32(d, current_derivative_error, D_COEFFICIENT);
	int16_t control_speed = (p + d)>>16;
	
	if (desired_engine_direction != ENGINE_DIRECTION_FORWARD)
		control_speed = 0;
	
	int16_t trim_speed = (int8_t)current_engine_speed * motor_trim;
	
	if (control_speed > 0)
	{
		uint8_t left_control_speed = (uint8_t)control_speed;
		if (left_control_speed > max_control_speed)
			ENGINE_LEFT_SPEED = current_engine_speed - max_control_speed + (trim_speed >> 8);
		else
			ENGINE_LEFT_SPEED = current_engine_speed - left_control_speed + (trim_speed >> 8);
			
		ENGINE_RIGHT_SPEED = current_engine_speed - (trim_speed >> 8);
	}
	else 
	{
		uint8_t right_control_speed = (uint8_t)abs(control_speed);
		if (right_control_speed > max_control_speed)
			ENGINE_RIGHT_SPEED = current_engine_speed - max_control_speed - (trim_speed >> 8);
		else
			ENGINE_RIGHT_SPEED = current_engine_speed - right_control_speed - (trim_speed >> 8);
		
		ENGINE_LEFT_SPEED = current_engine_speed + (trim_speed >> 8);
	}
	
	/* Engine direction */
	if (is_stationary())
		PORTB = desired_engine_direction;
}

/* 
 * Sets the desired engine speed.
 */
void set_desired_engine_speed(uint8_t speed)
{
	desired_engine_speed = speed;
}

/*
 * Forcefully sets the engine speed, bypassing the acceleration service routine.
 */
void force_engine_speed(uint8_t speed)
{
	desired_engine_speed = speed;
	current_engine_speed = speed;
	ENGINE_LEFT_SPEED = current_engine_speed;
	ENGINE_RIGHT_SPEED = current_engine_speed;
}

/*
 * Sets the desired engine direction.
 */
void set_desired_engine_direction(engine_direction direction)
{
	desired_engine_direction = direction;
}

/* 
 * Forcefully sets the engine direction, bypassing the acceleration service routine
 */
void force_engine_direction(engine_direction direction)
{
	desired_engine_direction = direction;
	PORTB = direction;
}

/*
 * Returns true if current engine speed is zero.
 */
uint8_t is_stationary()
{
	return !(current_engine_speed);
}

/*
 * Sets the motor speed for moving forward in manual mode.
 */
void set_manual_forward_engine_speed() {
	ENGINE_LEFT_SPEED = 100;
	ENGINE_RIGHT_SPEED = 100;
}

/*
 * Sets the motor speed for turning in manual mode.
 */
void set_manual_turn_engine_speed() {
	ENGINE_LEFT_SPEED = TURN_SPEED;
	ENGINE_RIGHT_SPEED = TURN_SPEED;
}

/*
 * Sets the motor speed for moving forward and turning left in manual mode.
 */
void set_manual_forward_left_engine_speed() {
	ENGINE_LEFT_SPEED = 80;
	ENGINE_RIGHT_SPEED = 120;
}

/*
 * Sets the motor speed for moving forward and turning right in manual mode.
 */
void set_manual_forward_right_engine_speed() {
	ENGINE_LEFT_SPEED = 120;
	ENGINE_RIGHT_SPEED = 80;
}

/*
 * Sets the PWM signal for ópening the claw
 */
void open_claw() {
	CLAW_POSITION = CLAW_OPEN;
}
/*
 * Sets the PWM signal for closing the claw
 */
void close_claw() {
	CLAW_POSITION = CLAW_CLOSED;
}