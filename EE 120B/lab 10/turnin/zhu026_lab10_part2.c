/*	Author: zhu026
 *	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Demo: https://drive.google.com/file/d/1wRfsZd0wPtsm29hcRCTi8jpvYpNT5iD1/view?usp=sharing
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

unsigned char power = 0x01;
unsigned int count;
unsigned int count_corrct;
const unsigned char password[5]={'1','2','3','4','5'};
enum keypadState{start_keypad, init_keypad, enter, standBy, pass};
int tick_keypad(int state){
        unsigned char x;
        x = GetKeypadKey();
        switch(state){
                case start_keypad:
                        state = init_keypad;
                        break;
                case init_keypad:
                        if(x != '#'){
                                state = init_keypad;
                        }
                        else if(x == '#'){
                                count = 0;
                                count_corrct = 0;
                                state = enter;
                        }
                        break;
                case enter:
                        if(x == '#'){
                                state = enter;
                        }
                        else if(x == '\0'){
                                state = standBy;
                        }
                        break;
                case standBy:
                        if(x == '#'){
                                count = 0;
                                count_corrct = 0;
                                state = enter;
                        }
                        else if((x == '\0') && (count < 5)){
                                state = standBy;
                        }
                        else if((x != '\0') && (count < 5)){
                                state = pass;
                                if(x == password[count]) {
                                        count_corrct++;
                                }
                                count++;
                        }
                        else if((x == '\0') && (count >= 5)){
                                state = init_keypad;
                        }
                        break;
                case pass:
                        if(x != '\0'){
                                state = pass;
                        }
                        else if(x == '\0'){
                                state = standBy;
                        }
                        break;
                default:
                        break;
        }
        switch(state){
                case start_keypad:
                        break;
                case init_keypad:
                        if (count_corrct == 5){
                                power = 0x00;
                                count_corrct = 0;
                        }
                        break;
                case enter:
                        count_corrct = 0;
                        count = 0;
                        break;
                case standBy:
                        break;
                case pass:
                        break;
                default:
                        break;

        }
        return state;
}



enum lock_state{start_lock, state_1, state_2};
int tick_lock(int state){
        switch(state){
                case start_lock:
                        state = state_1;
                        break;
                case state_1:
                        if((~PINB & 0x80) == 0x80){
                                power = 0x01;
                                state = state_2;
                        }
                        else{
                                state = state_1;
                        }
                        break;
                case state_2:
                        if((~PINB & 0x80) == 0x80){
                                state = state_2;
                        }
                        else{
                                state = state_1;
                        }
                        break;
                default:
                        break;
        }
        switch(state){
                case start_lock:
                        break;
                case state_1:
                        break;
                case state_2:
                        break;
                default:
                        break;
        }
        return state;
}

enum LED_control{start_LED, light};
int tick_LED(int state){
        switch(state){
                case start_LED:
                        state = light;
                        break;
                case light:
                        state = light;
                        break;
                default:
                        break;
        }
        switch(state){
                case start_LED:
                        break;
                case light:
                        PORTB = power;
                        break;
                default:
                        break;
        }
        return state;
}

task tasks[3];

int main(void) {
        DDRB = 0x7F; PORTB = 0x80;
        DDRC = 0xF0; PORTC = 0x0F;

        tasks[0].state=start_keypad;
        tasks[0].period=100;
        tasks[0].elapsedTime=0;
        tasks[0].TickFct=&tick_keypad;

        tasks[1].state=start_lock;
        tasks[1].period=100;
        tasks[1].elapsedTime=0;
        tasks[1].TickFct=&tick_lock;

        tasks[2].state=start_LED;
        tasks[2].period=100;
        tasks[2].elapsedTime=0;
        tasks[2].TickFct=&tick_LED;

        TimerSet(100);
        TimerOn();
        while(1){
           for (int i = 0; i < 3; i++) {
                        if (tasks[i].elapsedTime == tasks[i].period) {
                                tasks[i].state = tasks[i].TickFct(tasks[i].state);
                                tasks[i].elapsedTime = 0;
                        }
                        tasks[i].elapsedTime += 50;
                }
                while(!TimerFlag);
                TimerFlag = 0;
        }
        return 1;
}