/*
 * control_unit.h
 */
#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#ifndef CONTROL_UNIT_H_
#define CONTROL_UNIT_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "bus_communication.h"
#include "control_system.h"
#include "map.h"

static const uint8_t MAPPING_SPEED = 50;
static const uint8_t SUPER_SPEED = 255;
static const uint8_t TURN_SPEED = 50;
uint8_t state_speed;
typedef void (*state_function)();

typedef enum { MANUAL, AUTONOMOUS, TEST } mode;
typedef enum { SEARCH, RETRIEVE, DELIVER, RETURN, IDLE } task;

mode current_mode;
task current_task;


#endif