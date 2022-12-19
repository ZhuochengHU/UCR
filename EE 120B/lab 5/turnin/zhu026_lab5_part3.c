/*	Author: zhu026
 *  Partner(s) Name: Jo Smith
 *	Lab Section:
 *	Assignment: Lab 5
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Video (demo):https://drive.google.com/file/d/1rbNDCqpIZRpl5PZA7AbccIh-uih0cT90/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned array[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
unsigned char i = 0x00;
unsigned char invert = 0x00;
enum States {start, begin, init, reset, resetOn, plus, minus, plusOn, minusOn} state;
void tick() {
	switch(state) {
		case start:
			state = begin;
			break;
		case begin:
			state = init;
			break;
		case init:
			if ((i > 0x08) || (i < 0x00)) {
				state = resetOn;
			}
			else if ((~PINA & 0x01) == 0x01) {
				if ((invert & 0x01) == 0x01) {
                                	state = minusOn;
                        	}
				else {
					state = plusOn;
				}
			}
			else {
                        	state = init;
                        }
			break;
		case reset:
			if ((~PINA & 0x01) == 0x01) {
                                state = reset;
                        }
                        else {
                                state = begin;
                        }
                        break;
		case resetOn:
			state = reset;
			break;
		case plus:
			if ((~PINA & 0x01) == 0x01) {
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
                        if ((~PINA & 0x01) == 0x01) {
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
			state = start;
			break;
	}
	switch(state) {
		case start:
			PORTC = 0x00;
			break;
		case begin:
			PORTC = 0x00;
			break;
		case init:
			break;
		case plus:
			break;
		case minus:
			break;
		case plusOn:
			++i;
			PORTC = array[i];
                        break;
		case minusOn:
			--i;
			PORTC = array[i];
                        break;
		case reset:
			break;
		case resetOn:
			PORTC = 0x00;
			if ((invert & 0x01) == 0x01) {
				invert = 0x00;
				i = 0x00;
			}
			else {
				invert = 0x01;
				i = 0x08;
			}
			break;
		default:
			PORTC = 0x00;
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