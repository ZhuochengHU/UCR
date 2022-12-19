/*	Author: zhu026
 *  Partner(s) Name: Jo Smith
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
     DDRA = 0x00; PORTA = 0xFF;
     DDRB = 0x00; PORTB = 0xFF;
     DDRC = 0x00; PORTC = 0xFF;
     DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    unsigned char tmpB = 0x00;
    unsigned char tmpA = 0x00;
    unsigned char tmpC = 0x00;
    unsigned char tmpD = 0x00;
    unsigned char tmpD_1 = 0x00;
    unsigned char tmpD_f = 0x00;
    unsigned char totalWeight = 0x0000;

    while (1) {
	tmpA = PINA;
	tmpB = PINB;
	tmpC = PINC;
	tmpD = 0x00;
	tmpD_1 = 0x00;
	totalWeight = tmpA + tmpB + tmpC;
	if (totalWeight > 0x008C) {
		tmpD = 0x01;
	} else {
		tmpD = 0x00;
	}
	if ((tmpA - tmpC) > 0x50) {
		tmpD_1 = 0x02;
	}
	if ((tmpC - tmpA) > 0x50) {
		tmpD_1 = 0x02;
	}
	
	tmpD_f = (((totalWeight & 0x00FC)>>2) & 0x3C) | tmpD | tmpD_1;
	PORTD = tmpD_f;
    }
    return 0;
}
