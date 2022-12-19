/*	Author: zhu026
 *  	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Demo: https://drive.google.com/file/d/1PsrxsfNMWWxP2FN40kK6b2H8bAosyrGh/view?usp=sharing
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    ADC_init();
    while (1) {
	unsigned short x = ADC;
	unsigned short max = 0xFF;
	unsigned char temp = (char)x;
	unsigned light = max/8;

	if (temp <= light) {
		PORTB = 0x01;
	}
	else if (temp <= (light * 2)) {
                PORTB = 0x03;
        }
	else if (temp <= (light * 3)) {
		PORTB = 0x07;
	}
	else if (temp <= (light * 4)) {
                PORTB = 0x0F;
        }
	else if (temp <= (light * 5)) {
                PORTB = 0x1F;
        }
	else if (temp <= (light * 6)) {
                PORTB = 0x3F;
        }
	else if (temp <= (light * 7)) {
                PORTB = 0x7F;
        }
	else if (temp <= (light * 8)) {
		PORTB = 0xFF;
	}
    }
    return 0;
}