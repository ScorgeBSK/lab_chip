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

struct Task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct) (int);
};

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char speakerOutput = 0x00;
unsigned char combinedLED = 0x00;

enum TL_States {threeLED_start, seq1, seq2, seq3} TL_state;
int threeLEDTick(){


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
	return TL_state;
}

enum BL_States {blinkLED_start, init, on, off} BL_state;
int blinkLEDTick(){

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
	return BL_state;
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
			combinedLED = (speakerOutput | blinkingLED | threeLEDs);		
			break;
		default:
			break;
	}
}

enum S_States {speaker_start, speaker_init, speaker_off, speaker_on} S_state;
int speakerTick(){

	unsigned char button = ~PINA & 0x01;

	switch(S_state){
		case speaker_start:
			S_state = init;
			break;
		case speaker_init:
			S_state = button ? speaker_on : speaker_init;
			break;
		case speaker_off:
			S_state = button ? speaker_on : speaker_off;
			break;
		case speaker_on:
			S_state = button ? speaker_on : speaker_off;
			break;
		default:
			S_state = speaker_start;
			break;
	}

	switch(S_state){
		case speaker_start:
			break;
		case speaker_init:
			speakerOutput = 0x00;
			break;
		case speaker_off:
			speakerOutput = 0x00;
			break;
		case speaker_on:
			speakerOutput = 0x10;
			break;
		default:
			break;
	}

	return S_state;
}

int main(void) {

    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */

	const unsigned char SIZE = 3;
	struct Task tasks[SIZE];

	tasks[0].state = threeLED_start;
	tasks[0].period = 300;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &threeLEDTick;

	tasks[1].state = blinkLED_start;
	tasks[1].period = 1000;
	tasks[1].elapsedTime = tasks[1].period;
	tasks[1].TickFct = &blinkLEDTick;

	tasks[2].state = speaker_start;
	tasks[2].period = 2;
	tasks[2].elapsedTime = tasks[2].period;
	tasks[2].TickFct = &speakerTick;

	unsigned long PERIOD = 2;
	TimerSet(PERIOD);
	TimerOn();
	
	unsigned char i;
	while(1){
		
		for(i = 0; i < SIZE; ++i){
			if(tasks[i].elapsedTime >= tasks[i].period){
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += PERIOD;
		}
		CombineLEDTick();
		PORTB = combinedLED;
		while(!TimerFlag) {};
		TimerFlag = 0;

	}
    		
    return 1;
}
