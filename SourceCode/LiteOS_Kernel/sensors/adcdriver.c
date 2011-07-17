/** @file adcdriver.c
	@brief The detailed implementation of the adc driver. 

	@author Qing Charles Cao (cao@utk.edu)	
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adcdriver.h"



//the definitions of adc implementations 
#define adc_on() ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADIF)
#define adc_off() ADCSRA &= ~(1 << ADEN)


//The critical ISR function to come back when the interrupt of ADC finishes 
ISR(ADC_vect)
{
    ADCSRA |= _BV(ADIF);
    ADCSRA &= ~_BV(ADEN);       // disable interrupt
    
}

//-------------------------------------------------------------------------
void adcdriver_init_adc()
{
    ADMUX = 0;
    ADCSRA = _BV(ADIE) | 6;
    // does not enable interrupt and set prescaler to 64
}

//poll based approach. After all, requrieing another adc thread is just too expensive!
uint16_t get_adc(int channel)
{
    uint16_t reading;

    ADMUX = 0;
    ADMUX |= (channel & 0x1F);
    ADCSRA &= ~(1 << ADIE);
    ADCSRA |= (1 << ADIF);
    ADCSRA |= (1 << ADEN) | (1 << ADSC);
    // start conversion
    while (ADCSRA & (1 << ADSC))
        ;
    //the interrupt based adc is currently not used. 
    
    reading = ADCL;
    reading |= (ADCH & 3) << 8;
    ADCSRA &= ~_BV(ADEN);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_light()
{
    uint16_t reading;

    PORTE &= ~_BV(6);
    DDRE &= ~_BV(6);
    PORTE |= _BV(5);
    DDRE |= _BV(5);
    reading = get_adc(LIGHT_CHANNEL);
    PORTE &= ~_BV(5);
    DDRE &= ~_BV(5);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_temp()
{
    uint16_t reading;

    PORTE &= ~_BV(5);
    DDRE &= ~_BV(5);
    PORTE |= _BV(6);
    DDRE |= _BV(6);
    reading = get_adc(TEMP_CHANNEL);
    PORTE &= ~_BV(6);
    DDRE &= ~_BV(6);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_magx()
{
    uint16_t reading;

    DDRC |= _BV(5);
    PORTC |= _BV(5);
    reading = get_adc(MAGX_CHANNEL);
    DDRC &= ~_BV(5);
    PORTC &= ~_BV(5);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_magy()
{
    uint16_t reading;

    DDRC |= _BV(5);
    PORTC |= _BV(5);
    reading = get_adc(MAGY_CHANNEL);
    DDRC &= ~_BV(5);
    PORTC &= ~_BV(5);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_accx()
{
    uint16_t reading;

    DDRC |= _BV(4);
    PORTC |= _BV(4);
    reading = get_adc(ACCX_CHANNEL);
    DDRC &= ~_BV(4);
    PORTC &= ~_BV(4);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_accy()
{
    uint16_t reading;

    DDRC |= _BV(4);
    PORTC |= _BV(4);
    reading = get_adc(ACCY_CHANNEL);
    DDRC &= ~_BV(4);
    PORTC &= ~_BV(4);
    return reading;
}

