/*
 * control_system.h
 *
 */ 

#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bus_communication.h"

#define ENGINE_LEFT_SPEED OCR0A
#define ENGINE_RIGHT_SPEED OCR0B
#define ENGINE_LEFT_DIRECTION PB0
#define ENGINE_RIGHT_DIRECTION PB1

#define CLAW_POSITION OCR2A
#define CLAW_CLOSED 130
#define CLAW_OPEN 75
#define TIMER0_CLEAR_ON_MATCH_H (0<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0)
#define TIMER0_CLEAR_ON_MATCH_L (0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00)

#define ENGINE_OUTPUT (1<<DDB0)|(1<<DDB1)|(1<<DDB3)|(1<<DDB4)

#define TIMER1_CLEAR_ON_MATCH_H (0<<WGM13) | (1<<WGM12)
#define TIMER1_CLEAR_ON_MATCH_L (0<<WGM11) | (0<<WGM10)
#define TIMER1_PRESCALE_64 (0<<CS12) | (1<<CS11) | (1<<CS10)
#define TIMER1_MATCH_FREQUENCY_500HZ F_CPU / 500 / 64	// = 625
#define TIMER1_INTERRUPT_ENABLE (1<<OCIE1A)

volatile uint16_t P_COEFFICIENT;
volatile uint16_t D_COEFFICIENT;

static const uint8_t MAPPING_SPEED = 135;
static const uint8_t SUPER_SPEED = 135;
static const uint8_t TURN_SPEED = 75;
static const uint8_t INTERSECTION_SPEED = 135;

volatile int8_t motor_trim;

typedef enum { MANUAL, AUTONOMOUS, TEST } mode;
mode current_mode;

typedef enum {
	ENGINE_DIRECTION_FORWARD = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION),
	ENGINE_DIRECTION_LEFT = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION),
	ENGINE_DIRECTION_RIGHT = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION),
	ENGINE_DIRECTION_BACKWARD = (0<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION)
} engine_direction;

void init_control_system();
void set_desired_engine_speed(uint8_t speed);
void force_engine_speed(uint8_t speed);
void set_desired_engine_direction(engine_direction direction);
void force_engine_direction(engine_direction direction);
uint8_t is_stationary();
void set_manual_forward_engine_speed();
void set_manual_turn_engine_speed();
void set_manual_forward_left_engine_speed();
void set_manual_forward_right_engine_speed();

void open_claw();
void close_claw();

#endif /* CONTROL_SYSTEM_H_ */