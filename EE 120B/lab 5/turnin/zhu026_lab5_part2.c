/*	Author: zhu026
 *  Partner(s) Name: Jo Smith
 *	Lab Section:
 *	Assignment: Lab 5 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, begin, init, reset, plus, minus, plusOn, minusOn} state;
void tick() {
	switch(state) {
		case start:
			state = begin;
			break;
		case begin:
			state = init;
			break;
		case init:
			if ((~PINA & 0x03) == 0x03) {
                                state = reset;
                        }
			else if ((~PINA & 0x01) == 0x01) {
				state = plusOn;
			}
			else if ((~PINA & 0x02) == 0x02) {
                                state = minusOn;
                        }
			break;
		case reset:
			if ((~PINA & 0x03) == 0x03) {
                                state = reset;
                        }
                        else {
                                state = init;
                        }
                        break;
		case plus:
			if ((~PINA & 0x03) == 0x01) {
                                state = plus;
                        }
                        else {
                                state = init;
                        }
                        break;
		case plusOn:
			state = plus;
			break;
			
		case minus:
                        if ((~PINA & 0x03) == 0x02) {
                                state = minus;
                        }
                        else {
                                state = init;
                        }
                        break;
		case minusOn:
			state = minus;
			break;
			
		default:
			state = init;
			break;
	}
	switch(state) {
		case start:
			PORTC = 0x07;
			break;
		case begin:
			PORTC = 0x07;
			break;
		case init:
			break;
		case plus:
			break;
		case minus:
			break;
		case plusOn:
			if (PORTC < 0x09) {
                      		PORTC = PORTC + 1;
			}
                        break;
		case minusOn:
			if (PORTC > 0x00) { 
                        	PORTC = PORTC - 1;
			}
                        break;
		case reset:
			PORTC = 0x00;
			break;
		default:
			PORTC = 0x07;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    /* Insert your solution below */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    while (1) {
	tick();
    }
    return 0;
}