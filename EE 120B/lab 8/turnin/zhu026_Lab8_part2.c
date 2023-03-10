/*	Author: zhu026
 *  	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Demo: https://drive.google.com/file/d/16rlI2ByobBvodu3HP7NBe6TVJcJsxBJa/view?usp=sharing
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
enum States {start, init, up, down, power, standBy} state;
double d_levels[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned int power_situation = 0;
unsigned int x = 0;

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

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }
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

void tick() {
	switch(state) {
		case start:
			state = init;
			break;
		case init:
			if ((~PINA & 0x07) == 0x01) {
				state = up;
			}
			else if ((~PINA & 0x07) == 0x02) {
				state = down;
			}
			else if ((~PINA & 0x07) == 0x04) {
				state = power;
			} else {
				state = init;
			}
			break;
		case up:
			state = standBy;
			break;
		case down:
			state = standBy;
                        break;
		case power:
			state = standBy;
			break;
		case standBy:
			if ((~PINA & 0x07) == 0x00) {
				state = init;
			} else {
				state = standBy;
			}
                        break;
		default:
			state = start;
			break;
	}
	switch (state) {
		case start:
		    	break;
		case init:
			break;
		case up:
			if (power_situation == 1??? {
				if (x < 7) {
					x++;
				}
				set_PWM(d_levels[x]);
			}
			break;
		case down:
			if (power_situation == 1??? {
				if (x > 0) {
					x--;
				}
				set_PWM(d_levels[x]);
			}
			break;
		case power:
			if (power_situation == 0) {
				power_situation = 1;
				set_PWM(d_levels[0]);
			}
			if (power_situation == 1) {
				power_situation = 0;
				set_PWM(0);
			}
			break;
		case standBy:
			break;
                default:
		        break;
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(100);
    TimerOn();
    PWM_on();
    while (1) {
	tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}