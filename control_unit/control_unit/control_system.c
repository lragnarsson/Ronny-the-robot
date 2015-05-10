/*
 * control_system.c
 *
 */
#include "control_system.h"
#include "mult16x16.h"

const int16_t ANGLE_CHANGE_THRESHOLD = 800;
//volatile uint8_t P_COEFFICIENT = 20;
//volatile uint8_t D_COEFFICIENT = 20;

volatile uint8_t desired_speed = 0;
volatile uint8_t current_speed = 0;
volatile uint8_t control_speed_left = 0;
volatile uint8_t control_speed_right = 0;
volatile uint8_t control_speed_max = 0;


/* Initialize hardware ports */
void init_control_system() {
	TCCR0A = TIMER0_CLEAR_ON_MATCH_H; // PWM for left and right engine
	TCCR0B = TIMER0_CLEAR_ON_MATCH_L; // PWM for left and right engine
	DDRB = ENGINE_OUTPUT; // Left and right engine speed and direction as output
	
	TCCR1A = TIMER1_CLEAR_ON_MATCH_L;
	TCCR1B = TIMER1_CLEAR_ON_MATCH_H | TIMER1_PRESCALE_64;
	TIMSK1 = TIMER1_INTERRUPT_ENABLE;
	OCR1A = TIMER1_MATCH_FREQUENCY_100HZ;
	
	TCCR2A = TIMER2_CLEAR_ON_MATCH_H;
	TCCR2B = TIMER2_CLEAR_ON_MATCH_L;

	DDRD = (0<<DDD0) | (1<<DDD7); // Button input and claw output
	//CLAW_POSITION = CLAW_CLOSED;
	
	P_COEFFICIENT = 0x0A00;
	D_COEFFICIENT = 0x0F00;
}

/* Service routine for handling acceleration */
ISR(TIMER1_COMPA_vect) {
	if(abs(angle_remaining) < 20 && distance_remaining < 100)
		desired_speed = 20; //Speed from witch instant stop is OK
	if(desired_speed > current_speed)
		++current_speed;
	else if(desired_speed < current_speed)
		--current_speed;
	control_speed_max = current_speed;
	
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
	PORTB = (1<<ENGINE_LEFT_DIRECTION)|(1<<ENGINE_RIGHT_DIRECTION);
}

void set_same_engine_speed() {
	ENGINE_LEFT_SPEED = current_speed + 3;
	ENGINE_RIGHT_SPEED = current_speed;
}

/* Calculates the control_speed based on current angle and error in distance to the middle of the corridor */
void calculate_control_speed() {
	int32_t p;
	MultiSU16X16to32(p, current_distance_error, P_COEFFICIENT);
	int32_t d;
	MultiSU16X16to32(d, current_derivative_error, D_COEFFICIENT);
	int16_t control_speed_raw = (p + d)>>16;
	
	if (control_speed_raw > 0)
	{
		control_speed_left = (uint8_t)abs(control_speed_raw);
		if (control_speed_left > control_speed_max)
		{
			control_speed_left = control_speed_max;
		}
		control_speed_right = 0;
	}
	else 
	{
		control_speed_right = (uint8_t)abs(control_speed_raw);
		if (control_speed_right > control_speed_max)
		{
			control_speed_right = control_speed_max;
		}
		control_speed_left = 0;
	}
}

void set_controlled_engine_speed() {
	calculate_control_speed();
	ENGINE_LEFT_SPEED = current_speed - control_speed_left + 3;
	ENGINE_RIGHT_SPEED = current_speed - control_speed_right;
}

void set_manual_forward_left_engine_speed() {
	ENGINE_LEFT_SPEED = current_speed - (current_speed >> 3);
	ENGINE_RIGHT_SPEED = current_speed +  (current_speed >> 3);
}

void set_manual_forward_right_engine_speed() {
	ENGINE_LEFT_SPEED = current_speed + (current_speed >> 3);
	ENGINE_RIGHT_SPEED = current_speed - (current_speed >> 3) ;
}

void open_claw() {
	CLAW_POSITION = CLAW_OPEN;
}

void close_claw() {
	CLAW_POSITION = CLAW_CLOSED;
}