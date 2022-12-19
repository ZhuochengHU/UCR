/*	Author: zhu026
 *	Partner(s) Name:
 *	Lab Section:
 *	Assignment:
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Demo: https://drive.google.com/file/d/18iRP8x9wI2EmE5j1EQZOmsVWH09gQiFs/view?usp=sharing
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
unsigned int changed = 0;
const unsigned char password[5]={'1','2','3','4','5'};
const unsigned char newPassword[4]={'E','E','E','E'};
enum keypadState{start_keypad, init_keypad, enter, standBy, standBy_changed, pass};
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
                        else if((x == '\0') && (changed == 0)){
                                state = standBy;
                        }
			else if((x == '\0') && (changed == 1)){
				state = standBy_changed;
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
		case standBy_changed:
                        if(x == '#'){
                                count = 0;
                                count_corrct = 0;
                                state = enter;
                        }
                        else if((x == '\0') && (count < 4)){
                                state = standBy_changed;
                        }
                        else if((x != '\0') && (count < 4)){
                                state = pass;
                                if(x == newPassword[count]) {
                                        count_corrct++;
                                }
                                count++;
                        }
                        else if((x == '\0') && (count >= 4)){
                                state = init_keypad;
                        }
                        break;
                case pass:
                        if(x != '\0'){
                                state = pass;
                        }
                        else if((x == '\0') && (changed == 0)){
                                state = standBy;
                        }
			else if((x == '\0') && (changed == 1)){
				state = standBy_changed;
			}
                        break;
                default:
                        break;
        }
        switch(state){
                case start_keypad:
                        break;
                case init_keypad:
                        if (((count_corrct == 5) && (changed == 0)) || ((count_corrct == 4) && (changed == 1))) {
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



void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


double melody[8] = {261.63, 293.66, 329.63, 261.63, 261.63, 293.66, 329.63, 261.63};
int time_hold[8] = {1, 1, 1, 1, 1, 1, 1, 1};
unsigned int x=0, i=0;
enum doorDell_states {start_doorDell, init_doorDell, power_doorDell, standBy_doorDell};
int tick_sound(int state) {
	switch(state) {
		case start_doorDell:
			state = init_doorDell;
			break;
		case init_doorDell:
			if ((~PINA & 0x80) == 0x80) {
				state = power_doorDell;
			} else {
				state = init_doorDell;
			}
			break;
		case power_doorDell:
			if (x >= 8) {
				state = standBy_doorDell;
			} else {
				state = power_doorDell;
			}
			break;
		case standBy:
			if ((~PINA & 0x80) == 0x00) {
				state = init_doorDell;
			} else {
				state = standBy_doorDell;
			}
			break;
		default:
			state = start_doorDell;
			break;
	}
	switch (state) {
		case start_doorDell:     
			break;
		case init_doorDell:	
			set_PWM(0);
			x = 0;
			break;
		case power_doorDell:	
			if (i < time_hold[x]) {
				set_PWM(melody[x]);
				i++;
			} else if( i == time_hold[x]) {
				i = 0;
				x++;
			}
			break;
		case standBy_doorDell:	
			set_PWM(0); 
			x = 0; 
			break;
                default:
		        break;
	}
	return state;
}



enum changePassword_states {start_changePassword, init_changePassword, changing, done};
unsigned int count_newPassword;
int tick_changePassword(int state){
	unsigned char x;
	x = GetKeypadKey();
	switch(state) {
		case start_changePassword:
			state = init_changePassword;
			break;
		case init_changePassword:
			if ((x == '*') && ((~PINB & 0x80) == 0x80)) {
				count_newPassword = 0;
				state = changing;
			} else {
				state = init_changePassword;
			}
			break;
		case changing:
			if (((x == '*') && (~PINB & 0x80) == 0x80) || (x == '\0')) {
				state = changing;
			}
			else if (count_newPassword == 4) {
				changed = 1;
				state = done;
			}
			else if (x != '\0') {
				newPassword[count_newPassword] = x;
				count_newPassword++;
				state = changing;
			}
			break;
		case done:
			state=done;
			break;
		default:
			break;
	}
	switch(state) {
		case start_changePassword:
			break;
		case init_changePassword:
			break;
		case changing:
			break;
		case done:
			break;
		default:
			break;
	}
	return state;
}

task tasks[5];

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
        DDRB = 0x7F; PORTB = 0x80;
        DDRC = 0xF0; PORTC = 0x0F;

	PWM_on();

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

        tasks[3].state=start_doorDell;
        tasks[3].period=200;
        tasks[3].elapsedTime=0;
        tasks[3].TickFct=&tick_sound;

        tasks[4].state=start_changePassword;
        tasks[4].period=100;
        tasks[4].elapsedTime=0;
        tasks[4].TickFct=&tick_changePassword;

        TimerSet(100);
        TimerOn();
        while(1){
           for (int i = 0; i < 5; i++) {
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