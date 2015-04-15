/*
 * sensor_unit.c
 *
 * Created: 2015-03-24 09:43:41
 *  Author: Jesper
 */ 

#include "sensor_unit.h"

int main(void)
{
	init_ir();
	init_reflectance();
	init_wheel_encoder();
	
	sei();
	
    while(1)
    {
        //TODO:: Please write your application code
    }
}


// IR distance measurement routine
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{
	static uint8_t sample_index = 0;
	
	// Short distance
	for (uint8_t i = 0; i < 4; ++i)
	{
		ADMUX = i;
		ADCSRA |= (1<<ADSC);	// start conversion
		
		while (ADC_NOT_COMPLETE) { }
			
		uint16_t adc = ADC;
		
		if (adc < IR_SHORT_MIN)
		{
			ir_sensors[i][sample_index] = 0xFFFF;
		}
		else if (adc > IR_SHORT_MAX)
		{
			ir_sensors[i][sample_index] = 0x0000;
		}
		else
		{
			ir_sensors[i][sample_index] = pgm_read_byte(&ir_short_lut[adc - ir_short_lut_index_offset]) + ir_short_lut_value_offset;
		}
	}
	
	// Long distance
	ADMUX = 4;
	ADCSRA |= (1<<ADSC);	// start conversion
	
	while (ADC_NOT_COMPLETE) { }
	
	uint16_t adc = ADC;
	
	if (adc < IR_LONG_MIN)
	{
		ir_sensors[4][sample_index] = 0xFFFF;
	}
	else if (adc > IR_LONG_MAX)
	{
		ir_sensors[4][sample_index] = 0x0000;
	}
	else
	{
		ir_sensors[4][sample_index] = pgm_read_word(&ir_long_lut[adc - ir_long_lut_index_offset]);
	}
	
	if (++sample_index == IR_NUM_SAMPLES)
		sample_index = 0;
		
	uint16_t distance = 0;
	for (uint8_t i = 0; i < IR_NUM_SAMPLES; ++i)
	{
		distance += ir_sensors[3][i];
	}
	distance = distance / IR_NUM_SAMPLES;
	
	PORTC = (uint8_t)((distance>>8) & 0x01);
	PORTD = (uint8_t)(distance & 0x00ff);
}

// Wheel encoder counting routines
ISR(INT0_vect)
{
	if (PINB & (1<<ENC_L_B))
		++encoder_left;
	else
		--encoder_left;
}

ISR(INT1_vect)
{	
	if (PINB & (1<<ENC_R_B))
		++encoder_right;
	else
		--encoder_right;
}

// Reflectance sensor routine
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
		PORTC |= (1<<PORTC1);
		_delay_us(500);
		PORTC &= ~(1<<PORTC1);
		ACSR |= (1<<ACI); // Avoid double interrupt, requires delay	
	}
}

// Reflectance sensor calibration routine
uint8_t calibrate_reflectance_sensor()
{
	cli();
	
	ADMUX = (1<<ADLAR) | REFL_ADC;
	ADCSRA |= (1<<ADSC);	// start conversion
	
	while (ADC_NOT_COMPLETE) { }
	
	uint8_t inv_reflectance = ADCH;
	uint8_t threshold = ((0xFF - inv_reflectance) >> 1) + inv_reflectance;
	
	OCR0B = threshold; // set pwm
	
	sei();
	
	return inv_reflectance;
}

// Initialize IR sensors
void init_ir()
{
	// set up adc
	ADCSRA = ADC_PRESCALE_128;	// 20 MHz / 128 = 156.25 kHz
	
	// set up timed interrupt at 25 hz
	TCCR1A = TIMER1_CLEAR_ON_MATCH_L;
	TCCR1B = TIMER1_CLEAR_ON_MATCH_H | TIMER1_PRESCALE_64;
	TIMSK1 = TIMER1_INTERRUPT_ENABLE;
	OCR1A = TIMER1_MATCH_FREQUENCY_25HZ;
	
	DDRD = 0xFF;
	DDRC = 0x03;
}

// Initialize reflectance sensor
void init_reflectance()
{
	// set up ac
	ACSR = AC_INTERRUPT_RISING_EDGE;
	DDRB |= (1<<REFL_REF);
	
	// set up pwm
	TCCR0A = TIMER0_B_NON_INV | TIMER0_FAST_PWM_L;
	TCCR0B = TIMER0_FAST_PWM_H | TIMER0_PRESCALE_1;
	
	calibrate_reflectance_sensor();
}

// Initialize wheel encoders
void init_wheel_encoder()
{
	// set up encoder interrupts
	EIMSK = INTERRUPT_INT0_INT1;
	EICRA = INTERRUPT_INT0_INT1_RISING_EDGE;
	
	// set up encoder inputs
	DDRB &= ~( (1<<ENC_L_B) | (1<<ENC_R_B) );
}