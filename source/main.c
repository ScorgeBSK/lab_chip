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

enum States {start, init, playC, playD, playE} state;

void tick() {
	unsigned char button = ~PINA & 0x07;

	switch(state){
		case start:
			state = init;
			break;
		case init:
			if(button == 0x01){
				state = playC;
			}
			else if(button == 0x02){
				state = playD;
			}
			else if(button == 0x04){
				state = playE;
			}
			else{
				state = init;
			}
			break;
		case playC:
			state = (button == 0x01) ? playC : init;
			break;
		case playD:
                        state = (button == 0x02) ? playD : init;
                        break;
		case playE:
                        state = (button == 0x04) ? playE : init;
                        break;
		default:
			state = start;
			break;
	}

	switch(state){
		case init:
			set_PWM(0);
			break;
		case playC:
			set_PWM(261.63);
			break;
		case playD:
			set_PWM(293.66);
			break;
		case playE:
			set_PWM(329.63);
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
	
	PWM_on();	
	
	state = start;

	while(1){
	
		tick();

	}
    		
    return 1;
}
