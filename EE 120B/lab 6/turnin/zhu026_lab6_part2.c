/*	Author: zhu026
 *  	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *	Demo: https://drive.google.com/file/d/1kMwjRRoYCtkWdL6SQHUFrjG3zh7-r05S/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
enum States {start, init, nextLight, pause, reSume_1, reSume_2} state;
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
			if ((~PINA & 0x01) == 0x01) {
				state = pause;
			} else {
				state = nextLight;
			}
			break;
		case pause:	
			if ((~PINA & 0x01) == 0x01) {
				state = pause;
			} else {
				state = reSume_1;
			}
			break;
		case reSume_1:	
			if ((~PINA & 0x01) == 0x01) {
				state = reSume_2;
			} else {
				state = reSume_1;
			}
			break;
		case reSume_1:	
			if ((~PINA & 0x01) == 0x01) {
				state = reSume_2;
			} else {
				state = init;
			}
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
			if (PORTB == 0x04) {
				PORTB = PORTB >> 1;
			} else {
				PORTB = PORTB << 1;
			}
			if (PORTB == 0x01) {
                                PORTB = PORTB << 1;
                        } else {
                                PORTB = PORTB >> 1;
                        }
			break;
		case pause:
		 	break;
		case reSume_1:
			break;
		case reSume_2:
		 	break;
                default:
		        break;
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(300);
    TimerOn();
    while (1) {
	tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}