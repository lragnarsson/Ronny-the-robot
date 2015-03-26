/*
 * sensor_unit.c
 *
 * Created: 2015-03-24 09:43:41
 *  Author: Jesper
 */ 

#define F_CPU 20000000UL

#define TIMER0_B_NON_INV (1<<COM0B1) | (0<<COM0B0)
#define TIMER0_FAST_PWM_L (1<<WGM01) | (1<<WGM00)
#define TIMER0_FAST_PWM_H (0<<WGM02)
#define TIMER0_PRESCALE_1 (0<<CS02) | (0<<CS01) | (1<<CS00)

#define TIMER1_CLEAR_ON_MATCH_H (0<<WGM13) | (1<<WGM12)
#define TIMER1_CLEAR_ON_MATCH_L (0<<WGM11) | (0<<WGM10)
#define TIMER1_PRESCALE_64 (0<<CS12) | (1<<CS11) | (1<<CS10)
#define TIMER1_MATCH_FREQUENCY_25HZ F_CPU / 25 / 64		// = 12500
#define TIMER1_INTERRUPT_ENABLE (1<<OCIE1A)

#define ADC_PRESCALE_128 (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
#define ADC_NOT_COMPLETE !(ADCSRA & (1<<ADIF))

#define INTERRUPT_INT0_INT1 (1<<INT1) | (1<<INT0)
#define INTERRUPT_INT0_INT1_RISING_EDGE (1<<ISC01) | (1<<ISC00) | (1<<ISC11) | (1<<ISC10)

#define AC_INTERRUPT_RISING_EDGE (1<<ACIE) | (1<<ACIS1) | (1<<ACIS0)

#define ENC_L_B PINB0
#define ENC_R_B PINB1

#define REFL_REF PORTB4
#define REFL_ADC PINA5

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint16_t ir_sensors[5];
volatile int8_t encoder_left = 0;
volatile int8_t encoder_right = 0;
volatile uint8_t tape_found = 0;

int main(void)
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
	
	/*
	// set up encoder interrupts
	EIMSK = INTERRUPT_INT0_INT1;
	EICRA = INTERRUPT_INT0_INT1_RISING_EDGE;
	
	// set up encoder inputs
	DDRB &= ~( (1<<ENC_L_B) | (1<<ENC_R_B) );
	
	// set up ac
	ACSR = AC_INTERRUPT_RISING_EDGE;
	DDRB |= (1<<REFL_REF);
	
	// set up pwm
	TCCR0A = TIMER0_B_NON_INV | TIMER0_FAST_PWM_L;
	TCCR0B = TIMER0_FAST_PWM_H | TIMER0_PRESCALE_1;
	*/
	sei();
	
    while(1)
    {
        //TODO:: Please write your application code
    }
}

// IR distance measurement routine
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{
	for (uint8_t i = 0; i < 1; ++i)
	{
		ADMUX = i;
		ADCSRA |= (1<<ADSC);	// start conversion
		
		while (ADC_NOT_COMPLETE) { }
		
		uint8_t adcl = ADCL;
		uint16_t result = ADCH;
		
		PORTC = result & 0x03;
		PORTD = adcl;
		
		result = result << 8;
		result |= adcl;
		
		ir_sensors[i] = result;
	}
}

/*
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
	tape_found = 1;
}

// Reflectance sensor calibration routine
uint8_t calibrate_reflectance_sensor()
{
	cli();
	
	ADMUX = (1<<ADLAR) | (1<<REFL_ADC);
	ADCSRA |= (1<<ADSC);	// start conversion
	
	while (ADC_NOT_COMPLETE) { }
	
	uint8_t reflectance = ADCH;
	uint8_t threshold = 1.20f * reflectance;
	
	OCR0B = threshold; // set pwm
	
	sei();
	
	return reflectance;
}*/