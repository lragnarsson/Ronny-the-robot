/* 
 * sensor_unit.c
 * Ronny-the-robot/sensor_unit
 * ------------------------------
 * This file contains all functionality available in the sensor unit, 
 * including reading sensor values as well as sending and 
 * receiving messages to and from other modules.
 * ------------------------------ 
 * Author: J. Otterholm
 */ 

#include "sensor_unit.h"
#include "I2C.h"

int main(void)
{
	init_ir();
	init_reflectance();
	init_wheel_encoder();
	i2c_init(BITRATE_20MHZ, PRESCALER_20MHZ, SENSOR_UNIT);
	DDRD = (0<<DDD0);
	
	sei();
	
    while(1) { }
}


/*
 * IR distance measurement routine.
 */
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{
	static uint8_t sample_index = 0;
	
	/* Short distance sensors */
	for (uint8_t i = 0; i < 4; ++i)
	{
		ADMUX = i;
		ADCSRA |= (1<<ADSC);	// Start conversion
		
		while (ADC_NOT_COMPLETE) { }
			
		uint16_t adc = ADC;
		
		if (adc < IR_SHORT_MIN)
			ir_sensors[i][sample_index] = 0xFFFF;
		else if (adc > IR_SHORT_MAX)
			ir_sensors[i][sample_index] = 0x0000;
		else
			ir_sensors[i][sample_index] = pgm_read_byte(&ir_short_lut[adc - ir_short_lut_index_offset]) + ir_short_lut_value_offset;
	}
	
	/* Long distance sensor */
	ADMUX = 4;
	ADCSRA |= (1<<ADSC);	// Start conversion
	
	while (ADC_NOT_COMPLETE) { }
	
	uint16_t adc = ADC;
	
	if (adc < IR_LONG_MIN)
		ir_sensors[4][sample_index] = 0xFFFF;
	else if (adc > IR_LONG_MAX)
		ir_sensors[4][sample_index] = 0x0000;
	else
		ir_sensors[4][sample_index] = pgm_read_word(&ir_long_lut[adc - ir_long_lut_index_offset]);
	
	if (++sample_index == IR_NUM_SAMPLES)
		sample_index = 0;
		
	send_distance_readings();
}

/*
 * Left wheel encoder counting routine.
 */
ISR(INT0_vect)
{
	if (PINB & (1<<ENC_L_B))
	{
		if (--encoder_left < -5)
		{
			if (send_odometry_readings())
				encoder_left = encoder_right = 0;
		}
	}
	else
	{
		if (++encoder_left > 5)
		{
			if (send_odometry_readings())
				encoder_left = encoder_right = 0;
		}
	}
}

/*
 * Right wheel encoder counting routine.
 */
ISR(INT1_vect)
{	
	if (PINB & (1<<ENC_R_B))
	{
		if (++encoder_right > 5)
		{
			if (send_odometry_readings())
				encoder_left = encoder_right = 0;
		}
	}
	else
	{
		if (--encoder_right < -5)
		{
			if (send_odometry_readings())
				encoder_left = encoder_right = 0;
		}
	}
}

/* 
 * Reflectance sensor routine.
 */
ISR(ANALOG_COMP_vect)
{
	if (tape_found)
	{
		tape_found = 0;
		_delay_us(500);
		ACSR |= (1<<ACI); // Avoid double interrupt, requires delay	
	}
	else 
	{
		tape_found = 1;

		for (uint8_t i = 0; i < 3; ++i)
		{
			if (i2c_write_byte(GENERAL_CALL, TAPE_FOUND))
				break;
			_delay_ms(1);
		}

		ACSR |= (1<<ACI); // Avoid double interrupt, requires delay	
	}
}

/* 
 * Reflectance sensor calibration routine.
 */
uint8_t calibrate_reflectance_sensor()
{
	uint8_t inv_reflectance;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ADMUX = (1<<ADLAR) | REFL_ADC;
		ADCSRA |= (1<<ADSC);	// Start conversion
		
		while (ADC_NOT_COMPLETE) { }
		
		inv_reflectance = ADCH;
		uint8_t threshold = ((0xFF - inv_reflectance) >> 1) + inv_reflectance;
		
		OCR0B = threshold;		// Set PWM 
	}
	
	return inv_reflectance;
}

/*
 * Initialize IR sensors.
 */
void init_ir()
{
	ADCSRA = ADC_PRESCALE_128;
	
	/* Timed interrupt at 25 hz */
	TCCR1A = TIMER1_CLEAR_ON_MATCH_L;
	TCCR1B = TIMER1_CLEAR_ON_MATCH_H | TIMER1_PRESCALE_64;
	TIMSK1 = TIMER1_INTERRUPT_ENABLE;
	OCR1A = TIMER1_MATCH_FREQUENCY_25HZ;
}

/*
 * Initialize reflectance sensor.
 */
void init_reflectance()
{
	ACSR = AC_INTERRUPT_RISING_EDGE;
	DDRB |= (1<<REFL_REF);
	
	/* Reference value PWM */
	TCCR0A = TIMER0_B_NON_INV | TIMER0_FAST_PWM_L;
	TCCR0B = TIMER0_FAST_PWM_H | TIMER0_PRESCALE_1;
	
	calibrate_reflectance_sensor();
}

/*
 * Initialize wheel encoders.
 */
void init_wheel_encoder()
{
	EIMSK = INTERRUPT_INT0_INT1;
	EICRA = INTERRUPT_INT0_INT1_RISING_EDGE;
	
	DDRB &= ~( (1<<ENC_L_B) | (1<<ENC_R_B) );
}

/*
 * Decides what to do with a received message from the I2C-bus.
 */
void handle_received_message()
{
	switch (busbuffer[0])
	{
		case CALIBRATE_TAPE_SENSOR:
		{
			uint8_t val = calibrate_reflectance_sensor();
			uint8_t msg[] = {TAPE_SENSOR_VALUE, val};
			i2c_write(COMMUNICATION_UNIT, msg, sizeof(msg));
			break;
		}
	}
}

/*
 * Send average of current and latest sensor reading for each sensor on the I2C-bus. 
 */
uint8_t send_distance_readings()
{
	uint16_t distances[] = {0, 0, 0, 0, 0};
		
	for (uint8_t sensor = 0; sensor < 5; ++sensor)
	{
		for (uint8_t sample = 0; sample < IR_NUM_SAMPLES; ++sample)
		{
			distances[sensor] += ir_sensors[sensor][sample];
		}
		distances[sensor] = distances[sensor] / IR_NUM_SAMPLES;
	}
	
	uint8_t msg[] = { SENSOR_READINGS,
		(uint8_t)(distances[0]>>8),
		(uint8_t) distances[0],
		(uint8_t)(distances[1]>>8),
		(uint8_t) distances[1],
		(uint8_t)(distances[2]>>8),
		(uint8_t) distances[2],
		(uint8_t)(distances[3]>>8),
		(uint8_t) distances[3],
		(uint8_t)(distances[4]>>8),
		(uint8_t) distances[4] };

	return i2c_write(GENERAL_CALL, msg, sizeof(msg));
}

/*
 * Send distance traveled since last time this message was sent.
 */
uint8_t send_odometry_readings()
{
	static uint8_t distance_trunc = 0;
	int16_t scaled_distance = (encoder_left + encoder_right) * ENCODER_DISTANCE_SCALE + distance_trunc;
	distance_trunc = (uint8_t)scaled_distance;
	int8_t distance = (int8_t)(scaled_distance>>8);
	
	static uint8_t rotation_trunc = 0;
	int16_t scaled_rotation = (encoder_left - encoder_right) * ENCODER_ROTATION_SCALE + rotation_trunc;
	rotation_trunc = (uint8_t)scaled_rotation;
	int8_t rotation = (int8_t)(scaled_rotation>>8);
	
	int8_t msg[] = { MOVED_DISTANCE_AND_ANGLE, distance, rotation };
	return i2c_write(GENERAL_CALL, msg, sizeof(msg));
}
