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

enum States {start, init, unlockPound, unlockY, unlockYRelease, locked} state;
void tick() {
	switch(state) {
		case start:
			state = init;
			break;
		case init:
			if ((PINA & 0x07) == 0x04) {
				state = unlockPound;
			}
			else if ((PINA & 0x87) == 0x80) {
				state = locked;
			}
			else {
				state = init;
			}
			break;
		case unlockPound:
			if ((PINA & 0x07) == 0x04) {
                                state = unlockPound;
                        }
                        else {
                                state = unlockY;
                        }
                        break;
		case unlockY:
			if (((PINA & 0x07) == 0x02) && ((PORTB & 0x01) == 0x01)) {
                                state = locked;
                        }
			else if ((PINA & 0x07) == 0x02) {
				state = unlockYRelease;
			}
			else if (PINA == 0x00) {
				state = unlockY;
			}
                        else {
                                state = init;
                        }
                        break;
		case unlockYRelease:
			if ((PINA & 0x07) == 0x02) {
                                state = unlockYRelease;
                        }
                        else {
                                state = init;
                        }
                        break;
		case locked:
			if ((PINA & 0x87) == 0x80) {
                                state = locked;
                        }
			else if ((PINA & 0x07) == 0x02) {
				state = locked;
			}
                        else {
                                state = init;
                        }
                        break;
	}
	switch(state) {
		case start:
			PORTB = 0x00;
			break;
		case init:
			break;
		case unlockPound:
			break;
		case unlockY:
			break;
		case unlockYRelease:
			PORTB = 0x01;
			break;
		case locked:
			PORTB = 0x00;
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