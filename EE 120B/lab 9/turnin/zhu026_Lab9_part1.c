/*	Author: zhu026
 *  	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Demo: https://drive.google.com/file/d/1YKA3bT6ag_-vHR9LECxZ6sv05Uxj0sfw/view?usp=sharing
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States_ThreeLEDs {start_threeLEDs, step1, step2, step3} state_ThreeLEDs;
enum States_BlinkingLED {start_blink, init_blink, blink} state_BlinkingLED;
enum States_CombineLED {start_combine, init_combine} state_CombineLED;
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;

volatile unsigned char TimerFlag = 0;
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

void tick_ThreeLEDsSM() {
	switch(state_ThreeLEDs) {
		case start_threeLEDs:
			state_ThreeLEDs = step1;
			break;
		case step1:
			state_ThreeLEDs = step2;
			break;
		case step2:
			state_ThreeLEDs = step3;
			break;
		case step3:
			state_ThreeLEDs = step1;
			break;
		default:
			state_ThreeLEDs = start_threeLEDs;
			break;
	}

	switch (state_ThreeLEDs) {
		case start_threeLEDs:
		        break;
		case step1:
			threeLEDs = 0x01;
			break;
		case step2:
			threeLEDs = 0x02;
			break;
		case step3:
			threeLEDs = 0x04;
			break;
                default:
	        	break;
	}
}

void tick_BlinkingLEDSM() {
	switch (state_BlinkingLED) {
                case start_blink:
			state_BlinkingLED = init_blink;
			break;
                case init_blink:
			state_BlinkingLED = blink;
			break;
                case blink:
			state_BlinkingLED = init_blink;
			break;
                default:
			state_BlinkingLED = start_blink;
			break;
        }
	switch (state_BlinkingLED) {
                case start_blink:
			break;
                case init_blink:
			blinkingLED = 0x00;
			break;
                case blink:
			blinkingLED = 0x01;
			break;
                default:
			break;
        }
}

void tick_CombineLEDsSM() {
        switch (state_CombineLED) {
                case start_combine:
			state_CombineLED = init_combine;
			break;
                case init_combine:
			state_CombineLED = init_combine;
			break;
                default:
			state_CombineLED = start_combine;
			break;
        }
        switch (state_CombineLED) {
                case start_combine:
			break;
                case init_combine:
			PORTB = (threeLEDs) | (blinkingLED << 3);
			break;
                default:
			break;
        }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(1000);
    TimerOn();

    while (1) {
	tick_ThreeLEDsSM();
	tick_BlinkingLEDSM();
	tick_CombineLEDsSM();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}