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

enum States {start, init, seq1, seq2, seq3, seq2R, waitRelease1, waitPush1, waitRelease2, waitPush2, waitRelease3, waitPush3, waitRelease2R, waitPush2R} state;

void tick(){
	
	unsigned char button = ~PINA & 0x01;
	signed char led = PINB;

	switch(state){
		case start:
			state = init;
			break;
		case init:
			state = button ? seq1 : init;
			break;
		case seq1:
			state = waitRelease1;
			break;
		case seq2:
			state = waitRelease2;
			break;
		case seq3:
			state = waitRelease3;
			break;
		case seq2R:
			state = waitRelease2R;
			break;
		case waitRelease1:
			state = button ? waitRelease1 : waitPush1;
			break;
		case waitPush1:
			state = button ? seq2 : waitPush1;
			break;
		case waitRelease2:
                        state = button ? waitRelease2 : waitPush2;
                        break;
                case waitPush2:
                        state = button ? seq3 : waitPush2;
                        break;
		case waitRelease3:
                        state = button ? waitRelease3 : waitPush3;
                        break;
                case waitPush3:
                        state = button ? seq2R : waitPush3;
                        break;
		case waitRelease2R:
			state = button ? waitRelease2R : waitPush2R;
			break;
		case waitPush2R:
			state = button ? seq1 : waitPush2R;
			break;
		default:
			state = start;
			break;

	}

	switch(state){
		case init:
			led = 0;
			break;
		case seq1:
			led = 0x0C;
			break;
		case seq2:
			led = 0x12;	
			break;
		case seq3:
			led = 0x21;
			break;
		case seq2R:
			led = 0x12;
			break;
		default:
			break;
	}

	PORTB = led;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */

	state = start;

	while(1){
		tick();
	}
    return 1;
}
