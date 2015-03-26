/*
 * control_unit.c
 *
 * Created: 3/24/2015 9:06:14 AM
 *  Author: lagra033
 */
 
#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#define ENGINE_LEFT_SPEED OCR0A
#define ENGINE_RIGHT_SPEED OCR0B
#define ENGINE_LEFT_DIRECTION PB0
#define ENGINE_RIGHT_DIRECTION PB1
#define LEFT 1
#define BACKWARD 2
#define RIGHT 3

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const uint8_t MAPPING_SPEED = 200;
const uint8_t SUPER_SPEED = 150;
const uint8_t TURN_SPEED = 150;

typedef enum { NORTH, EAST, SOUTH, WEST } direction;
static const uint8_t ROUTE_END = 10;

direction current_direction;
direction current_route[64]  = { NORTH, NORTH, NORTH, NORTH, EAST, EAST, SOUTH, WEST, WEST, WEST, EAST, 10 };

uint8_t route_index = 0;
uint16_t distance_remaining = 0;
uint8_t angle_remaining  = 0;

uint8_t current_speed;

void init_ports() {
	TCCR0A = (0<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0);
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00);
	DDRB = (1<<DDB0)|(1<<DDB1)|(1<<DDB3)|(1<<DDB4);	
	DDRD = 0<<DDD1;
}

void init_state() {
	current_direction = NORTH;
	current_speed = MAPPING_SPEED;
}

void stop_engines() {
	ENGINE_LEFT_SPEED =	ENGINE_RIGHT_SPEED = 0;
}

void step_forward() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = current_speed;
	while(distance_remaining != 0) {
		_delay_ms(5);
		--distance_remaining;
	}
	stop_engines();	
}

void drive_forward() {
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = current_speed;
	while(distance_remaining != 0) {
		_delay_ms(5);
		--distance_remaining;
	}
	stop_engines();	
}

void rotate_left_90() {
	angle_remaining = 90;
	PORTB = (0<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = TURN_SPEED;
	while(angle_remaining != 0) {
		_delay_ms(5);
		-- angle_remaining; // simulering
	}
	--current_direction;
	current_direction &= 3;
	stop_engines();	
}

void rotate_right_90() {
	angle_remaining = 90;
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = TURN_SPEED;
	while(angle_remaining != 0) {
		_delay_ms(5);
		-- angle_remaining; // simulering
	}
	++current_direction;
	current_direction &= 3;
	stop_engines();	
}

void rotate_180() {
	angle_remaining = 180;
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(0<<ENGINE_RIGHT_DIRECTION);
	ENGINE_LEFT_SPEED = ENGINE_RIGHT_SPEED = TURN_SPEED;
	while(angle_remaining != 0) {
		_delay_ms(5);
		-- angle_remaining; // simulering
	}
	current_direction += 2;
	current_direction &= 3;
	stop_engines();
}

void create_route() {
	
	
}

uint8_t navigate() {
	direction next_direction = current_route[route_index];
	if (next_direction == ROUTE_END)
		return 0;
	distance_remaining = 0;
	do {
		distance_remaining += 100;
		++route_index;
	} while (next_direction == current_route[route_index]);

	uint8_t next_turn = (current_direction - next_direction) & 3;
	switch (next_turn) {
		case LEFT:
			rotate_left_90();
			step_forward();
			break;
		case RIGHT:
			rotate_right_90();
			step_forward();
			break;
		case BACKWARD:
			rotate_180();
			break;
		default:
			break;
	}
	
	drive_forward();
	return 1;
}

int main(void) {
	init_ports();
	init_state();

	uint8_t status = 0;
	do {
		status = navigate();
	} while (status == 1);
	stop_engines();
}