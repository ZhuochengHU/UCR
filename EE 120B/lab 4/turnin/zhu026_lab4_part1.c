/*	Author: zhu026
 *  Partner(s) Name: Jo Smith
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, pressOn, releaseOn, pressOff, releaseOff} state;
void t_tick() {
	switch(state) {
		case start:
			state = pressOn;
			break;
		case pressOn:
			if ((PINA & 0x01) == 0x01) {
				state = pressOn;
			}
			else {
				state = releaseOn;
			}
			break;
		case releaseOn:
			if ((PINA & 0x01) == 0x01) {
                                state = pressOff;
                        }
                        else {
                                state = releaseOn;
                        }
                        break;
		case pressOff:
			if ((PINA & 0x01) == 0x01) {
				state = pressOff;
			}
			else {
				state = releaseOff;
			}
			break;
		case releaseOff:
			if ((PINA & 0x01) == 0x01) {
				state = pressOn;
			}
			else {
				state = releaseOff;
			}
			break;
		default:
			state = start;
			break;
	}
	switch(state) {
		case start:
			PORTB = 0x01;
			break;
		case pressOn:
                        PORTB = 0x01;
                        break;
		case releaseOn:
			break;
		case pressOff:
                        PORTB = 0x02;
                        break;
		case releaseOff:
			break;
		default:
			PORTB = 0x01;
			break;
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    while (1) {
	t_tick();
    }
    return 0;
}