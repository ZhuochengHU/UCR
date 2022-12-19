/*      Author: zhu026
 *      Partner(s) Name:
 *      Lab Section:
 *      Assignment:
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      Demo: https://drive.google.com/file/d/1VYMN7iQ2dAkN1NjE7tAdI3RUX-nlK66a/view?usp=sharing
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include <keypad.h>
#include <timer.h>
#include <scheduler.h>
#include "simAVRHeader.h"
#endif

unsigned char playerPad_row = 0xC7;
unsigned char playerPad_col = 0x01;
enum playerPad_States {start_playerPad, move_playerPad, standBy};
int tick_playerPad(int state)
{
        switch (state)
        {
                case start_playerPad:
                        state = move_playerPad;
                        break;
                case move_playerPad:
                        if (((~PINA & 0x03) & 0x01) == 0x01) {
                                if (playerPad_row != 0xF8) {
                                        playerPad_row = (playerPad_row >> 1) | 0x80;
                                }
                                state = standBy;
                        } else if (((~PINA & 0x03) & 0x02) == 0x02) {
                                if (playerPad_row != 0x1F) {
                                        playerPad_row = (playerPad_row << 1) | 0x01;
                                }
                                state = standBy;
                        } else {
                                state = move_playerPad;
                        }
                        break;
                case standBy:
                        if ((~PINA & 0x03) == 0x00) {
                                state = move_playerPad;
                        } else {
                                state = standBy;
                        }
                        break;
                default:
                        state = start_playerPad;
                        break;
        }

        return state;
}


unsigned char ball_row = 0xEF;
unsigned char ball_col = 0x10;

unsigned char AIPad_row = 0xC7;
unsigned char AIPad_center = 0xEF;
unsigned char AIPad_col = 0x80;
unsigned char option;
enum AIPad_States {start_AIPad, init_AIPad, move_AIPad};
int tick_AIPad(int state)
{
        switch (state)
        {
                case start_AIPad:
                        option = 0;
                        state = init_AIPad;
                        break;
                case init_AIPad:
                        option = rand() % 4 + 1;
                        state = move_AIPad;
                        break;
                case move_AIPad:
                        if (option == 1) {
                                if (AIPad_row != 0xF8) {
                                        AIPad_row = (AIPad_row >> 1) | 0x80;
                                        AIPad_center = (AIPad_center >> 1) | 0x80;
                                }
                        }
                        else if (option == 2)
                        {
                                if (AIPad_row != 0x1F) {
                                        AIPad_row = (AIPad_row << 1) | 0x01;
                                        AIPad_center = (AIPad_center << 1) | 0x01;
                                }
                        }
                        else if (option == 3 || option == 4)
                        {
                                if (AIPad_center < ball_row) {
                                        if (AIPad_row != 0xF8) {
                                                AIPad_row = (AIPad_row >> 1) | 0x80;
                                                AIPad_center = (AIPad_center >> 1) | 0x80;
                                        }
                                }
                                else if (AIPad_center > ball_row)
                                {
                                        if (AIPad_row != 0x1F) {
                                                AIPad_row = (AIPad_row << 1) | 0x01;
                                                AIPad_center = (AIPad_center << 1) | 0x01;
                                        }
                                }
                        }
                        state = init_AIPad;
                        break;
                default:
                        state = start_AIPad;
                        break;
        }
        return state;
}

unsigned int count = 0;
unsigned int speed = 40;
unsigned char leftOrRight;
unsigned char moveStyle_ball;
unsigned char position_one;
unsigned char position_two;
unsigned char deviation;
enum ball_States {start_ball, speed_system, move_ball, ball_end};
int tick_Ball(int state)
{
        switch (state)
        {
                case start_ball:
                        ball_row = 0xEF;
                        ball_col = 0x10;
                        leftOrRight = 1;
                        moveStyle_ball = 2;
                        state = move_ball;
                        break;
                case speed_system:
                        if (count < speed) {
                                count++;
                                state = speed_system;
                        } else {
                                count = 0;
                                state = move_ball;
                        }
                        break;
                case move_ball:
                        if (moveStyle_ball == 2 && leftOrRight == 1)
                        {
                                if (ball_col != 0x02) {
                                        ball_col >>= 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerPad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerPad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerPad_row | position_one) == position_one) {
                                                moveStyle_ball = 2;
                                                speed = 40;
                                        }
                                        else if ((playerPad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        leftOrRight = 2;
                                        state = speed_system;
                                } else {
                                        ball_col >>= 1;
                                        state = ball_end;
                                }
                        }
                        else if (moveStyle_ball == 2 && leftOrRight == 2)
                        {
                                if (ball_col != 0x40) {
                                        ball_col <<= 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x40 && ((~AIPad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((AIPad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((AIPad_row | position_one) == position_one) {
                                                moveStyle_ball = 2;
                                                speed = 40;
                                        }
                                        else if ((AIPad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        leftOrRight = 1;
                                        state = speed_system;
                                } else {
                                        ball_col <<= 1;
                                        state = ball_end;
                                }
                        }
                        else if (moveStyle_ball == 1 && leftOrRight == 1)
                        {
                                deviation = ~ball_row >> 1;
                                if (ball_col != 0x02 && ball_row != 0xFE) {
                                        ball_col >>= 1;
                                        ball_row = (ball_row >> 1) | 0x80;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerPad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerPad_row | position_two) == position_two) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        else if ((playerPad_row | position_one) == position_one) {
                                                moveStyle_ball = 1;
                                                speed = 40;
                                        }
                                        else if ((playerPad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        leftOrRight = 2;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerPad_row & deviation) == deviation))
                                {
                                        moveStyle_ball = 3;
                                        leftOrRight = 2;
                                        speed = 30;
                                        state = speed_system;
                                }
                                else if (ball_row == 0xFE && ball_col != 0x02)
                                {
                                        moveStyle_ball = 3;
                                        speed = 30;
                                        state = speed_system;
                                } else {
                                        ball_col >>= 1;
                                        if (ball_row != 0xFE) {
                                                ball_row = (ball_row >> 1) | 0x80;
                                        }
                                        state = ball_end;
                                }
                        }
                        else if (moveStyle_ball == 1 && leftOrRight == 2)
                        {
                                deviation = ~ball_row >> 1;
                                if (ball_col != 0x40 && ball_row != 0xFE) {
                                        ball_col <<= 1;
                                        ball_row = (ball_row >> 1) | 0x80;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x40 && ((~AIPad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((AIPad_row | position_two) == position_two) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        else if ((AIPad_row | position_one) == position_one) {
                                                moveStyle_ball = 1;
                                                speed = 40;
                                        }
                                        else if ((AIPad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        leftOrRight = 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x40 && ((~AIPad_row & deviation) == deviation))
                                {
                                        moveStyle_ball = 3;
                                        leftOrRight = 1;
                                        speed = 30;
                                        state = speed_system;
                                }
                                else if (ball_row == 0xFE && ball_col != 0x40)
                                {
                                        moveStyle_ball = 3;
                                        speed = 30;
                                        state = speed_system;
                                } else {
                                        ball_col <<= 1;
                                        if (ball_row != 0xFE) {
                                                ball_row = (ball_row >> 1) | 0x80;
                                        }
                                        state = ball_end;
                                }
                        }
                        else if (moveStyle_ball == 3 && leftOrRight == 1)
                        {
                                deviation = ~ball_row << 1;
                                if (ball_col != 0x02 && ball_row != 0x7F) {
                                        ball_col >>= 1;
                                        ball_row = (ball_row << 1) | 0x01;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerPad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerPad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerPad_row | position_one) == position_one) {
                                                moveStyle_ball = 3;
                                                speed = 40;
                                        }
                                        else if ((playerPad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        leftOrRight = 2;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerPad_row & deviation) == deviation))
                                {
                                        moveStyle_ball = 1;
                                        leftOrRight = 2;
                                        speed = 30;
                                        state = speed_system;
                                }
                                else if (ball_row == 0x7F && ball_col != 0x02)
                                {
                                        moveStyle_ball = 1;
                                        speed = 30;
                                        state = speed_system;
                                } else {
                                        ball_col >>= 1;
                                        if (ball_row != 0x7F) {
                                                ball_row = (ball_row << 1) | 0x01;
                                        }
                                        state = ball_end;
                                }
                        }
                        else if (moveStyle_ball == 3 && leftOrRight == 2)
                        {
                                deviation = ~ball_row << 1;
                                if (ball_col != 0x40 && ball_row != 0x7F) {
                                        ball_col <<= 1;
                                        ball_row = (ball_row << 1) | 0x01;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x40 && ((~AIPad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((AIPad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((AIPad_row | position_one) == position_one) {
                                                moveStyle_ball = 3;
                                                speed = 40;
                                        }
                                        else if ((AIPad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        leftOrRight = 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~AIPad_row & deviation) == deviation))
                                {
                                        moveStyle_ball = 1;
                                        leftOrRight = 1;
                                        speed = 30;
                                        state = speed_system;
                                }
                                else if (ball_row == 0x7F && ball_col != 0x40)
                                {
                                        moveStyle_ball = 1;
                                        speed = 30;
                                        state = speed_system;
                                } else {
                                        ball_col <<= 1;
                                        if (ball_row != 0x7F) {
                                                ball_row = (ball_row << 1) | 0x01;
                                        }
                                        state = ball_end;
                                }
                        }
                        break;
                case ball_end:
                        if (leftOrRight == 1) {
                                ball_col = 0x10;
                                ball_row = 0xEF;
                                moveStyle_ball = 2;
                                speed = 40;
                                leftOrRight = 2;
                        }
                        else if (leftOrRight == 2)
                        {
                                ball_col = 0x08;
                                ball_row = 0xEF;
                                moveStyle_ball = 2;
                                speed = 40;
                                leftOrRight = 1;
                        }
                        state = move_ball;
                        break;
                default:
                        state = start_ball;
                        break;
        }
        return state;
}


enum reset_States {start_reset, init_reset, reset};
int tick_reset(int state)
{
        unsigned char button = ~PINA & 0x04;
        switch (state)
        {
                case start_reset:
                        state = init_reset;
                        break;
                case init_reset:
                        if ((button & 0x04) == 0x04) {
                                state = reset;
                        } else {
                                state = init_reset;
                        }
                        break;
                case reset:
                        playerPad_row = 0xC7;
                        playerPad_col = 0x01;
                        AIPad_row = 0xC7;
                        AIPad_center = 0xEF;
                        AIPad_col = 0x80;
                        ball_row = 0xEF;
                        ball_col = 0x10;
                        leftOrRight = 1;
                        moveStyle_ball = 2;
                        state = init_reset;
                        break;
                default:
                        state = start_reset;
                        break;
        }
        return state;
}

enum display_States {start_display, one_display, two_display, three_display};
int tick_display(int state)
{
        switch (state)
        {
                case start_display:
                        state = one_display;
                        break;
                case one_display:
                        PORTD = playerPad_row;
                        PORTC = playerPad_col;
                        state = two_display;
                        break;
                case two_display:
                        PORTD = AIPad_row;
                        PORTC = AIPad_col;
                        state = three_display;
                        break;
                case three_display:
                        PORTD = ball_row;
                        PORTC = ball_col;
                        state = one_display;
                        break;
                default:
                        state = start_display;
                        break;
        }
        return state;
}

task tasks[5];

int main(void) {
        DDRA = 0x00; PORTA = 0xFF;
        DDRB = 0xFF; PORTB = 0x00;
        DDRC = 0xFF; PORTC = 0x00;
        DDRD = 0xFF; PORTD = 0x00;

        tasks[0].state = start_playerPad;
        tasks[0].period = 1000;
        tasks[0].elapsedTime = 0;
        tasks[0].TickFct = &tick_playerPad;

        tasks[1].state = start_AIPad;
        tasks[1].period = 120000;
        tasks[1].elapsedTime = 0;
        tasks[1].TickFct = &tick_AIPad;

        tasks[2].state = start_ball;
        tasks[2].period = 5000;
        tasks[2].elapsedTime = 0;
        tasks[2].TickFct = &tick_Ball;

        tasks[3].state = start_reset;
        tasks[3].period = 1000;
        tasks[3].elapsedTime = 0;
        tasks[3].TickFct = &tick_reset;

        tasks[4].state = start_display;
        tasks[4].period = 1000;
        tasks[4].elapsedTime = 0;
        tasks[4].TickFct = &tick_display;

        TimerSet(100);
        TimerOn();

    while (1) {
                for (int i = 0; i < 5; i++)
                {
                        if (tasks[i].elapsedTime == tasks[i].period)
                        {
                                tasks[i].state = tasks[i].TickFct(tasks[i].state);
                                tasks[i].elapsedTime = 0;
                        }
                        tasks[i].elapsedTime += 100;
                }
                while (!TimerFlag)
                        TimerFlag = 0;
    }
    return 1;
}