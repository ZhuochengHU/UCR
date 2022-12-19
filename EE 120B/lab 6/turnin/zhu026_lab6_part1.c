/*	Author: zhu026
 *  	Partner(s) Name: 
 *	Lab Section: 
 *	Assignment: 
 *	Exercise Description: [optional - include for your own benefit]
 *	Demo: https://drive.google.com/file/d/12bOEW9zWoznBHg5KlOzZsgBNMLgXtI-V/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
enum States {start, init, nextLight} state;
void TimerISR() { 
	TimerFlag = 1;
}

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void tick() {
	switch(state) {
		case start:	
			state = init;
			break;
		case init:	
			state = nextLight; 
			break;
		case nextLight:	
			state = nextLight; 
			break;
		default:	
			state = start; 
			break;
	}
	switch (state) {
		case start:
			PORTB = 0x01;
			break;
                case init:
			PORTB = 0x01; 
			break;
		case nextLight:
			if ( PORTB == 0x04) {
				PORTB = 0x01;
			} else {
				PORTB = PORTB << 1;
			} 
			break;
                default:        
			break;
	}
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000);
    TimerOn();
    while (1) {
	tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}