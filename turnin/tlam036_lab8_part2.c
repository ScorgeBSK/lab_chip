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
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
	static double current_frequency;

	if(frequency != current_frequency){
		if(!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {start, init, on, waitOnRelease, waitInput, increment, waitIncRelease, decrement, waitDecRelease, off} state;

const double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

unsigned char i = 0;

void tick() {
	unsigned char button = ~PINA & 0x07;

	switch(state){
		case start:
			state = init;
			break;
		case init:
			state = (button == 0x01) ? on : init;
			break;
		case on:
			state = waitOnRelease;
			break;
		case waitOnRelease:
                        state = (button == 0x01) ? waitOnRelease : waitInput;
                        break;
		case waitInput:
                        if(button == 0x04) {
				if(i < 7) { ++i; }
				state = increment;
			}
			else if(button == 0x02) {
				if (i > 0) { --i; }
				state = decrement;
			}
			else if(button == 0x01) {
				state = off;
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
		case waitIncRelease:
			state = (button == 4) ? waitIncRelease : waitInput;
			break;
		case waitDecRelease:
			state = (button == 2) ? waitDecRelease : waitInput;
			break;
		case off:
                        state = (button == 0x01) ? off : init;
			break;
		default:
			state = start;
			break;
	}

	switch(state){
		case init:
			PWM_off();
			break;
		case on:
			PWM_on();
			break;
		case waitOnRelease:
			break;
		case waitInput:
			break;
		case increment:
			set_PWM(notes[i]);
			break;
		case decrement:
			set_PWM(notes[i]);
			break;
		case waitIncRelease:
			break;
		case waitDecRelease:
			break;
		case off:
			break;
		default:
			break;
	}
}

int main(void) {

    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x40; PORTB = 0x00;
    /* Insert your solution below */	
	
	state = start;

	while(1){
	
		tick();

	}
    		
    return 1;
}
