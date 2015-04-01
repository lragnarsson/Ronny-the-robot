#ifndef F_CPU
#define F_CPU 20000000UL
#endif
/*
 * Port names, inclusions and 
 * constants.
 */
#define ENGINE_LEFT_SPEED OCR0A
#define ENGINE_RIGHT_SPEED OCR0B
#define ENGINE_LEFT_DIRECTION PB0
#define ENGINE_RIGHT_DIRECTION PB1
#define LEFT 1
#define BACKWARD 2
#define RIGHT 3
#define ROUTE_END 10
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static const uint8_t MAPPING_SPEED = 100;
static const uint8_t SUPER_SPEED = 200;
static const uint8_t TURN_SPEED = 100;
static const uint8_t ANGLE_CHANGE_THRESHOLD = 20;
static const uint8_t P_COEFFICIENT = 1;
static const uint8_t D_COEFFICIENT = 1;

/*
 * Typedef's
 */
typedef void (*state_function)();

typedef enum { MANUAL, AUTONOMOUS } mode;
typedef enum { SEARCH, RETRIEVE, DELIVER, RETURN, IDLE } task;
typedef enum { NORTH, EAST, SOUTH, WEST } direction;


/*
 * Global variables
 */

/* State machine related */
mode current_mode;
task current_task;

/* Navigation related */
direction current_direction;
direction current_route[64]  = { NORTH, NORTH, NORTH, EAST, EAST, WEST, 10 }; // init with test route
uint8_t route_index = 0;
uint8_t know_shortest_path = 0;
uint16_t distance_remaining = 0;
uint8_t angle_remaining  = 0;

/* Control system related */
uint8_t current_speed = 0;
uint8_t control_speed = 0;
uint8_t control_speed_max = 0;
uint8_t current_angle = 0;
uint8_t current_angle_left = 0;
uint8_t current_angle_right = 0;
uint8_t current_distance_error = 0;

/* Sensor related */
uint16_t left_wall_distance = 0;
uint16_t right_wall_distance = 0;
uint8_t left_wall_angle = 0;
uint8_t right_wall_angle =  0;
uint8_t last_tick_angle_left = 0;
uint8_t last_tick_angle_right = 0;
uint8_t inverse_sampling_speed = 1;