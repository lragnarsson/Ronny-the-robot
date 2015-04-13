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
#define TIMER0_PRESCALE_1024 (1<<CS02) | (0<<CS01) | (1<<CS00)

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
#include <avr/pgmspace.h>

volatile uint16_t ir_sensors[5];
volatile int8_t encoder_left = 0;
volatile int8_t encoder_right = 0;
volatile uint8_t tape_found = 0;

#define IR_SHORT_MIN 190
#define IR_SHORT_MAX 460
const uint8_t ir_short_lut[IR_SHORT_MAX+1] PROGMEM = { 234, 232, 230, 229, 227, 225, 223, 222, 220, 218, 216, 215, 213, 211, 210, 208, 206, 205, 203, 201, 200, 198, 197, 195, 194, 192, 191, 189, 188, 186, 185, 183, 182, 180, 179, 178, 176, 175, 174, 172, 171, 170, 168, 167, 166, 164, 163, 162, 161, 159, 158, 157, 156, 155, 153, 152, 151, 150, 149, 148, 147, 146, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 125, 124, 123, 122, 121, 120, 119, 118, 118, 117, 116, 115, 114, 113, 113, 112, 111, 110, 110, 109, 108, 107, 107, 106, 105, 104, 104, 103, 102, 102, 101, 100, 99, 99, 98, 97, 97, 96, 96, 95, 94, 94, 93, 92, 92, 91, 91, 90, 89, 89, 88, 88, 87, 87, 86, 85, 85, 84, 84, 83, 83, 82, 82, 81, 81, 80, 80, 79, 79, 78, 78, 77, 77, 76, 76, 75, 75, 75, 74, 74, 73, 73, 72, 72, 71, 71, 71, 70, 70, 69, 69, 68, 68, 68, 67, 67, 66, 66, 66, 65, 65, 65, 64, 64, 63, 63, 63, 62, 62, 62, 61, 61, 61, 60, 60, 59, 59, 59, 58, 58, 58, 57, 57, 57, 56, 56, 56, 55, 55, 55, 55, 54, 54, 54, 53, 53, 53, 52, 52, 52, 51, 51, 51, 51, 50, 50, 50, 49, 49, 49, 48, 48, 48, 48, 47, 47, 47, 46, 46, 46, 46, 45, 45, 45, 44, 44, 44, 44, 43, 43, 43, 43, 42, 42, 42, 41, 41, 41, 41, 40, 40, 40, 40, 39, 39, 39, 38, 38, 38, 38, 37, 37, 37, 37, 36, 36, 36, 36, 35, 35, 35, 35, 34, 34, 34, 33, 33, 33, 33, 32, 32, 32, 32, 31, 31, 31, 31, 30, 30, 30, 30, 29, 29, 29, 29, 28, 28, 28, 28, 27, 27, 27, 27, 26, 26, 26, 26, 25, 25, 25, 25, 24, 24, 24, 24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 21, 20, 20, 20, 20, 19, 19, 19, 19, 18, 18, 18, 18, 17, 17, 17, 17, 17, 16, 16, 16, 16, 15, 15, 15, 15, 14, 14, 14, 14, 14, 13, 13, 13, 13, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
const uint8_t ir_short_lut_index_offset = IR_SHORT_MIN;
const uint8_t ir_short_lut_value_offset = 70;

uint8_t calibrate_reflectance_sensor();

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
	*/
	
	/*
	// set up ac
	ACSR = AC_INTERRUPT_RISING_EDGE;
	DDRB |= (1<<REFL_REF);
	
	// set up pwm
	TCCR0A = TIMER0_B_NON_INV | TIMER0_FAST_PWM_L;
	TCCR0B = TIMER0_FAST_PWM_H | TIMER0_PRESCALE_1;
	
	calibrate_reflectance_sensor();
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
			
		uint16_t adc = ADC;
		
		if (adc < IR_SHORT_MIN)
		{
			ir_sensors[i] = 0x00;
			continue;
		}
		else if (adc > IR_SHORT_MAX)
		{
			ir_sensors[i] = 0xFF;
			continue;
		}
		
		uint16_t distance = pgm_read_byte(&ir_short_lut[adc - ir_short_lut_index_offset]) + ir_short_lut_value_offset;
		
		/*
		uint8_t adcl = ADCL;
		uint16_t result = ADCH;
		
		PORTC = result & 0x03;
		PORTD = adcl;
		
		result = result << 8;
		result |= adcl;
		*/
		
		PORTC = (uint8_t)((distance>>8) & 0x03);
		PORTD = (uint8_t)(distance & 0x00ff);
		
		ir_sensors[i] = distance;
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
*/

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