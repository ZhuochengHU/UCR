/*	Author: zhu026
 *  	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Demo: https://drive.google.com/file/d/1bUs6wLZ1QBhyHUbZNHGXucpNU3BZbUiV/view?usp=sharing
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
enum States {start, begin, init, reset, plus, minus, plusOn, minusOn, add, sub} state;
void TimerISR() {
	TimerFlag = 1;
}
unsigned char count = 0x00;

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
			state = begin;
			break;
		case begin:
			state = init;
			break;
		case init:
			count = 0x00;
			if ((~PINA & 0x03) == 0x01) {
				state = add;
			}
			else if ((~PINA & 0x03) == 0x02) {
                                state = sub;
                        }
			else if ((~PINA & 0x03) == 0x03) {
                                state = reset;
                        }
			break;
		case reset:
			if ((~PINA & 0x03) == 0x03) {
				state = reset;
			} else {
				state = init;
			}
			break;
		case plus:
			if ((~PINA & 0x03) == 0x01) {
                                state = plus;
                        } else {
                                state = init;
                        }
                        break;
		case add:
			if (count >= 0x0A) {
                                count = 0x00;
                        }
			state = plusOn;
			break;
		case sub:
			if (temp >= 0x0A) {
                                count = 0x00;
                        }
			state = minusOn;
			break;
		case plusOn:
			if ((~PINA & 0x03) == 0x01) {
				if (count >= 0x0A) {
					state = add;
				}
                                ++count;
				state = plusOn;
                        }
                        else {
                                state = plus;
                        }
			break;
		case minus:
			if ((~PINA & 0x03) == 0x02) {
                                state = minus;
                        } else {
                                state = init;
                        }
                        break;
		case minusOn:
			if ((~PINA & 0x03) == 0x02) {
				if (count >= 0x0A) {
					state = sub;
				}
				++count;
                                state = minusOn;
                        } else {
                                state = minus;
                        }
			break;
		default:
			state = start;
			break;
	}
	switch(state) {
		case start:
			PORTB = 0x07;
			break;
		case begin:
			PORTB = 0x07;
			break;
		case init:
			break;
		case plusOn:
			break;
		case minusOn:
			break;
		case plus:
			break;
		case minus:
			break;
		case add:
			if (PORTB < 0x09) {
                        	PORTB++;
			}
                        break;
		case sub:
			if (PORTB > 0x00) {
                        	PORTB--;
			}
                        break;
		case reset:
			PORTB = 0x00;
			break;
		default:
			PORTB = 0x07;
			break;
	}
}

void main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(100);
    TimerOn();
    while (1) {
	tick();
	while(!TimerFlag) {};
	TimerFlag = 0;
    }
}