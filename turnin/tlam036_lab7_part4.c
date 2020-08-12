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
    /* Insert your solution below */
	unsigned char led = 0;
	unsigned short max = 0x00C9;
	while(1){
		unsigned short x = ADC;

		if(x >= 200){
			led = 0xFF;
		}
		else if(x < 200 && x >= 175){
			led = 0x7F;
		}
		else if(x < 175 && x >= 150){
			led = 0x3F;
		}
		else if(x < 150 && x >= 125){
			led = 0x1F;
		}
		else if(x < 125 && x >= 100){
			led = 0x0F;
		}
		else if(x < 100 && x >= 75){
			led = 0x07;
		}
		else if(x < 75 && x >= 50){
			led = 0x03;
		}
		else if(x < 50 && x >= 25){
			led = 0x01;
		}
		else{
			led = 0;
		}

		PORTB = led;


	}

    return 1;
}
