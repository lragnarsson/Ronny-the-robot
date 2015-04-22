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
#define CLAW_CLOSED 75
#define CLAW_OPEN 150
#define TIMER0_CLEAR_ON_MATCH_H (0<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0)
#define TIMER0_CLEAR_ON_MATCH_L (0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00)

#define ENGINE_OUTPUT (1<<DDB0)|(1<<DDB1)|(1<<DDB3)|(1<<DDB4)

#define TIMER1_CLEAR_ON_MATCH_H (0<<WGM13) | (1<<WGM12)
#define TIMER1_CLEAR_ON_MATCH_L (0<<WGM11) | (0<<WGM10)
#define TIMER1_PRESCALE_64 (0<<CS12) | (1<<CS11) | (1<<CS10)
#define TIMER1_MATCH_FREQUENCY_100HZ F_CPU / 500 / 64	// = 625
#define TIMER1_INTERRUPT_ENABLE (1<<OCIE1A)

#define TIMER2_CLEAR_ON_MATCH_H (0<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)
#define TIMER2_CLEAR_ON_MATCH_L (0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00)

void init_control_system(void);
void set_desired_speed(uint8_t speed);
void stop_engines(void);
void set_same_engine_speed(void);
void set_controlled_engine_speed(void);
uint8_t corner_detected_left(void);
uint8_t corner_detected_right(void);
void set_manual_forward_left_engine_speed();
void set_manual_forward_right_engine_speed();

void open_claw();
void close_claw();

#endif /* CONTROL_SYSTEM_H_ */