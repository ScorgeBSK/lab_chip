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

enum States {start, init, waitRelease, waitPlay, play, off} state;

const double notes[17] = {440.00, 0.00, 440.00, 0.00, 440.00, 0.00, 349.23, 0.00, 523.25, 0.00, 440.00, 0.00, 349.23, 0.00, 523.25, 0.00, 440.00};
const unsigned long times[17] = {500, 50, 500, 50, 500, 50, 250, 50, 150, 50, 500, 50, 250, 50, 150, 50, 750};
unsigned char i = 0;
unsigned counter = 0;
unsigned long PERIOD = 50;


void tick() {
	unsigned char button = ~PINA & 0x01;

	switch(state){
		case start:
			state = init;
			break;
		case init:
			if(button){
				state = play;
			}
			else{
				state = init;
			}
			break;
		case play:
			state = play;
			if(counter < times[i]){
				state = play;
			}
			else{
				++i;
				counter = 0;
			}

			if(i > 16){
				state = off;
			}
			break;
		case off:
			state = button ? off : init;
			break;
		default:
			state = start;
			break;
	}

	switch(state){
		case init:
			PWM_on();
			i = 0;
			counter = 0;
			break;
		case play:
			set_PWM(notes[i]);
			counter += PERIOD;
			break;
		case off:
			PWM_off();
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
	TimerSet(PERIOD);
	TimerOn();
	PWM_on();
	state = start;

	while(1){
	
		tick();
		while(!TimerFlag) {};
		TimerFlag = 0;

	}
    		
    return 1;
}
