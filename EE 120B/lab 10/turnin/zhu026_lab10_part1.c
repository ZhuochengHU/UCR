/*	Author: zhu026
 *	Partner(s) Name:
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
#include <keypad.h>
#include <timer.h>
#include <scheduler.h>
#endif

unsigned char x = 0x00;
enum KeypadStates {start, init};

int tick_keypad(int state) {
        x = GetKeypadKey();
        switch(state) {
                case start:
                        state = init;
                        break;
                case init:
                        state = init;
                        break;
                default:
                        state = start;
                        break;
        }
        switch(state) {
                case start:
                        break;
                case init:
                        switch (x) {
                                case '\0': PORTB = 0x00; break;
                                case '1': PORTB = 0x80; break;
                                case '2': PORTB = 0x80; break;
                                case '3': PORTB = 0x80; break;
                                case '4': PORTB = 0x80; break;
                                case '5': PORTB = 0x80; break;
                                case '6': PORTB = 0x80; break;
                                case '7': PORTB = 0x80; break;
                                case '8': PORTB = 0x80; break;
                                case '9': PORTB = 0x80; break;
                                case 'A': PORTB = 0x80; break;
                                case 'B': PORTB = 0x80; break;
                                case 'C': PORTB = 0x80; break;
                                case 'D': PORTB = 0x80; break;
                                case '*': PORTB = 0x80; break;
                                case '0': PORTB = 0x80; break;
                                case '#': PORTB = 0x80; break;
                                default: PORTB = 0x00; break;
                        }
                default:
                        break;
        }
        return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    /* Insert your solution below */
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;

    static task task_test;
    task *tasks[] = { &task_test };

    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = -1;

    task_test.state = start;
    task_test.period = 50;
    task_test.elapsedTime = task_test.period;
    task_test.TickFct = &tick_keypad;

    TimerSet(50);
    TimerOn();

    unsigned short i;
    while (1) {
        for (i = 0; i < numTasks; i++) {
                if (tasks[i]->elapsedTime == tasks[i]->period) {
                        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                        tasks[i]->elapsedTime = 0;
                }
                tasks[i]->elapsedTime += 50;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}