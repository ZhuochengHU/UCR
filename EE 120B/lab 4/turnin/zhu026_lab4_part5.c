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

enum States {start, init, correct, wrong, unlock, locked} state;
unsigned char index = 0x00;
unsigned char array[4] = {0x04, 0x01, 0x02, 0x01};
void tick() {
	switch(state) {
		case start:
			state = init;
			break;
		case init:
			if ((PINA & 0x87) == array[index]) {
				state = correct;
			}
			else if ((PINA & 0x87) == 0x80) {
				state = locked;
			}
			else if (PINA == 0x00) {
				state = init;
			}
			else {
				state = wrong;
			}
			break;
		case correct:
			if ((index == 0x03) && ((PORTB & 0x01) == 0x01)) {
				state = locked;
			}
			else if (index == 0x03) {
				state = unlock;
			}
			else if ((PINA & 0x07) == 0x00) {
                                ++index;
				state = init;
                        }
			else {
				state = correct;
			}
                        break;
		case wrong:
			if ((PINA & 0x07) == 0x00) {
				index = 0x00;
				state = init;
			}
			else {
			}
			break;
		case unlock:
                        if ((PINA & 0x07) == 0x00) {
				state = init;
				index = 0x00;
			}
			else {
				state = unlock;
			}
			break;
		case locked:
			if ((PINA & 0x87) == 0x80) {
                                state = locked;
                        }
                        else {
                                state = init;
				index = 0x00;
                        }
                        break;
	}
	switch(state) {
		case start:
			PORTB = 0x00;
			break;
		case init:
			break;
		case correct:
			break;
		case wrong:
			break;
		case unlock:
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