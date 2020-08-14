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
#include "pwm.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;

enum TL_States {threeLED_start, seq1, seq2, seq3} TL_state;
void threeLEDTick(){


	switch(TL_state){
		case threeLED_start:
			TL_state = seq1;
			break;
		case seq1:
			TL_state = seq2;
			break;
		case seq2:
			TL_state = seq3;
			break;
		case seq3:
			TL_state = seq1;
			break;
		default:
			TL_state = threeLED_start;
			break;
	}

	switch(TL_state){
		case threeLED_start:
			break;
		case seq1:
			threeLEDs = 0x01;
			break;
		case seq2:
			threeLEDs = 0x02;
			break;
		case seq3:
			threeLEDs = 0x04;
			break;
		default:
			break;

	}

}

enum BL_States {blinkLED_start, init, on, off} BL_state;
void blinkLEDTick(){

	switch(BL_state){
		case blinkLED_start:
			BL_state = init;
			break;
		case init:
			BL_state = on;
			break;
		case off:
			BL_state = on;
			break;
		case on:
			BL_state = off;
			break;
		default:
			BL_state = blinkLED_start;
			break;
	}

	switch(BL_state){
		case blinkLED_start:
			break;
		case init:
			blinkingLED = 0x00;
			break;
		case off:
			blinkingLED = 0x00;
			break;
		case on:
			blinkingLED = 0x08;
			break;
		default:
			break;
	}	
}

enum CL_States {combinedLED_start, output} CL_state;
void CombineLEDTick(){

	switch(CL_state){
		case combinedLED_start:
			CL_state = output;
			break;
		case output:
			CL_state = output;
			break;
		default:
			CL_state = combinedLED_start;
			break;
	}

	switch(CL_state){
		case combinedLED_start:
			break;
		case output:
			PORTB = (blinkingLED | threeLEDs);		
			break;
		default:
			break;
	}
		
}

int main(void) {

    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */

	BL_state = blinkLED_start;
	TL_state = threeLED_start;
        CL_state = combinedLED_start;

	TimerSet(1000);
	TimerOn();

	while(1){
		threeLEDTick();
		blinkLEDTick();
		CombineLEDTick();

		while(!TimerFlag) {};
		TimerFlag = 0;

	}
    		
    return 1;
}
