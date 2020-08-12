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

enum States {start, init, waitInput, increment, decrement, reset, waitIncRelease, waitDecRelease, waitResetRelease} state;

void tick(){
	
	unsigned char button = PINA & 0x03;
	signed char output = PINC;

	switch(state){
		case start:
			state = init;
			break;
		case init:
			state = waitInput;
			break;
		case waitInput:
			if(button == 1){
				state = increment;
			}
			else if(button == 2){
				state = decrement;
			}
			else if(button == 3){
				state = reset;
			}
			else{
				state = waitInput;
			}
			break;
		case increment:
			state = waitIncRelease;
			break;
		case decrement:
			state = waitDecRelease;
			break;
		case reset:
			state = waitResetRelease;
			break;
		case waitIncRelease:
			state = button ? waitIncRelease : waitInput;
			break;
		case waitDecRelease:
			state = button ? waitDecRelease : waitInput;
			break;
		case waitResetRelease:
			state = button ? waitResetRelease : waitInput;
			break;
		default:
			state = start;
			break;

	}

	switch(state){
		case init:
			output = 7;
			break;
		case waitInput:
			break;
		case increment:
			++output;
			if(output > 9){
				output = 9;
			}	
			break;
		case decrement:
			--output;
			if(output < 0){
				output = 0;
			}
			break;
		case reset:
			output = 0;
			break;
		case waitIncRelease:
			break;
		case waitDecRelease:
			break;
		case waitResetRelease:
			break;
		default:
			break;
	}

	PORTC = output;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; 
	DDRC = 0xFF; PORTC = 0x07;
    /* Insert your solution below */

	state = start;

	while(1){
		tick();
	}
    return 1;
}
