/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {

	ADC_init();

    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00; 
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */

	while(1){
		unsigned short x = ADC;
		
		PORTB = x & 0xFF;
		PORTD = ( (x >> 2)  & 0xC0 ) >> 6 ;

	}

    return 1;
}
