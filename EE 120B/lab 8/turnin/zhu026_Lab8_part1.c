/*	Author: zhu026
 *  	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	Demo: https://drive.google.com/file/d/1nniSr-9wzTB0iA0M9CVStKIo2mwZW7JY/view?usp=sharing
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
enum States {start, init, cFour, dFour, eFour} state;
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
				state = cFour;
			}
			else if ((~PINA & 0x07) == 0x02) {
				state = dFour;
			}
			else if ((~PINA & 0x07) == 0x04) {
				state = eFour;
			} else {
				state = init;
			}
			break;
		case cFour:
			if ((~PINA & 0x07) == 0x01) {
				state = cFour;
			} else {
				state = init;
			}
			break;
		case dFour:
			if ((~PINA & 0x07) == 0x02) {
				state = dFour;
			} else {
				state = init;
			}
                        break;
		case eFour:
			if ((~PINA & 0x07) == 0x04) {
				state = eFour;
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
		  	break;
		case init:
			set_PWM(0);
			break;
		case cFour:
			set_PWM(261.63);
			break;
		case dFour:
			set_PWM(293.66);
			break;
		case eFour:
			set_PWM(329.63);
			break;
                default:
		        break;
	}
}

void main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(300);
    TimerOn();
    PWM_on();
    while (1) {
	tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
}