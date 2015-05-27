/*
 * control_unit.h
 */

#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#ifndef CONTROL_UNIT_H_
#define CONTROL_UNIT_H_

#define DEBUG_LED_GREEN PORTD1
#define DEBUG_LED_RED PORTD2

#define CLAMP(x, l, h) (((x) > (h)) ? (h) : (((x) < (l)) ? (l) : (x)))

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "bus_communication.h"
#include "control_system.h"
#include "map.h"

typedef void (*state_function)();

state_function next_state;
state_function follow_up_state;

void autonomous_mode();
void test_mode();
void manual_mode();
void enter_map_state();
void search_state();
void navigate_state();
void return_state();
void grab_package_state();
void drop_package_state();
void end_state();
void rotate_left_90();
void rotate_right_90();
void rotate_180();

#endif /* CONTROL_UNIT_H */