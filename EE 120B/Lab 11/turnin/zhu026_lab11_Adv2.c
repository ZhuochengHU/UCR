/*      Author: zhu026
 *      Partner(s) Name:
 *      Lab Section:
 *      Assignment:
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      Demo: https://drive.google.com/file/d/1ACq-Da06CMCyhjru5dRAohOE_7TRy7xk/view?usp=sharing
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

unsigned int gameType = 0;
unsigned int score_one = 0;
unsigned int score_two = 0;
unsigned int gameEnd = 0;

unsigned char playerOne_Pad_row = 0xC7;
unsigned char playerOne_Pad_col = 0x01;
enum playerOne_Pad_States {start_playerOne_Pad, move_playerOne_Pad, standBy_one};
int tick_playerOne_Pad(int state)
{
        switch (state)
        {
                case start_playerOne_Pad:
                        state = move_playerOne_Pad;
                        break;
                case move_playerOne_Pad:
                        if (((~PINA & 0x03) & 0x01) == 0x01) {
                                if (playerOne_Pad_row != 0xF8) {
                                        playerOne_Pad_row = (playerOne_Pad_row >> 1) | 0x80;
                                }
                                state = standBy_one;
                        } else if (((~PINA & 0x03) & 0x02) == 0x02) {
                                if (playerOne_Pad_row != 0x1F) {
                                        playerOne_Pad_row = (playerOne_Pad_row << 1) | 0x01;
                                }
                                state = standBy_one;
                       } else {
                                state = move_playerOne_Pad;
                        }
                        break;
                case standBy_one:
                        if ((~PINA & 0x03) == 0x00) {
                                state = move_playerOne_Pad;
                        } else {
                                state = standBy_one;
                        }
                        break;
                default:
                        state = start_playerOne_Pad;
                        break;
        }

        return state;
}


unsigned char playerTwo_Pad_row = 0xC7;
unsigned char playerTwo_Pad_col = 0x80;
enum playerTwo_Pad_States {start_playerTwo_Pad, move_playerTwo_Pad, standBy_two};
int tick_playerTwo_Pad(int state)
{
        unsigned char x;
        x = GetKeypadKey();
        switch (state)
        {
                case start_playerTwo_Pad:
                        state = move_playerTwo_Pad;
                        break;
                case move_playerTwo_Pad:
                        if (gameType == 2) {
                        if (x == '1') {
                                if (playerTwo_Pad_row != 0xF8) {
                                        playerTwo_Pad_row = (playerTwo_Pad_row >> 1) | 0x80;
                                }
                                state = standBy_two;
                        } else if (x == '4') {
                                if (playerTwo_Pad_row != 0x1F) {
                                        playerTwo_Pad_row = (playerTwo_Pad_row << 1) | 0x01;
                                }
                                state = standBy_two;
                        } else {
                                state = move_playerTwo_Pad;
                        }
                        } else {
                                state = move_playerTwo_Pad;
                        }
                        break;
                case standBy_two:
                        if (x == '\0') {
                                state = move_playerTwo_Pad;
                        } else {
                                state = standBy_two;
                        }
                        break;
                default:
                        state = move_playerTwo_Pad;
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
                        if (gameType == 1) {
                                state = move_AIPad;
                        } else {
                                state = init_AIPad;
                        }
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
enum ball_States {start_ball, speed_system, move_ball_one, move_ball_two, ball_end};
int tick_Ball(int state)
{
        switch (state)
        {
                case start_ball:
                        ball_row = 0xEF;
                        ball_col = 0x10;
                        leftOrRight = 1;
                        moveStyle_ball = 2;
                        if (gameType == 1) {
                                score_one = score_two = 0;
                                state = move_ball_one;
                        }
                        else if (gameType == 2) {
                                score_one = score_two = 0;
                                state = move_ball_two;
                        }
                        break;
                case speed_system:
                        if (count < speed) {
                                count++;
                                state = speed_system;
                        } else {
                                count = 0;
                                if (gameType == 1) {
                                        state = move_ball_one;
                                }
                                else if (gameType == 2) {
                                        state = move_ball_two;
                                }
                        }
                        break;
                case move_ball_one:
                        if (moveStyle_ball == 2 && leftOrRight == 1)
                        {
                                if (ball_col != 0x02) {
                                        ball_col >>= 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerOne_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerOne_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 2;
                                                speed = 40;
                                        }
                                        else if ((playerOne_Pad_row | ball_row) == ball_row) {
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
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerOne_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        else if ((playerOne_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 1;
                                                speed = 40;
                                        }
                                        else if ((playerOne_Pad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        leftOrRight = 2;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & deviation) == deviation))
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
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerOne_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerOne_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 3;
                                                speed = 40;
                                        }
                                        else if ((playerOne_Pad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        leftOrRight = 2;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & deviation) == deviation))
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
                case move_ball_two:
                        if (moveStyle_ball == 2 && leftOrRight == 1)
                        {
                                if (ball_col != 0x02) {
                                        ball_col >>= 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerOne_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerOne_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 2;
                                                speed = 40;
                                        }
                                        else if ((playerOne_Pad_row | ball_row) == ball_row) {
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
                                else if (ball_col == 0x40 && ((~playerTwo_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerTwo_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerTwo_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 2;
                                                speed = 40;
                                        }
                                        else if ((playerTwo_Pad_row | ball_row) == ball_row) {
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
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerOne_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        else if ((playerOne_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 1;
                                                speed = 40;
                                        }
                                        else if ((playerOne_Pad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        leftOrRight = 2;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & deviation) == deviation))
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
                                else if (ball_col == 0x40 && ((~playerTwo_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerTwo_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        else if ((playerTwo_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 1;
                                                speed = 40;
                                        }
                                        else if ((playerTwo_Pad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 3;
                                                speed = 30;
                                        }
                                        leftOrRight = 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x40 && ((~playerTwo_Pad_row & deviation) == deviation))
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
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerOne_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerOne_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 3;
                                                speed = 40;
                                        }
                                        else if ((playerOne_Pad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        leftOrRight = 2;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerOne_Pad_row & deviation) == deviation))
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
                                else if (ball_col == 0x40 && ((~playerTwo_Pad_row & ~ball_row) == ~ball_row))
                                {
                                        position_one = (ball_row << 1) | 0x01;
                                        position_two = (ball_row << 2) | 0x03;
                                        if ((playerTwo_Pad_row | position_two) == position_two) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        else if ((playerTwo_Pad_row | position_one) == position_one) {
                                                moveStyle_ball = 3;
                                                speed = 40;
                                        }
                                        else if ((playerTwo_Pad_row | ball_row) == ball_row) {
                                                moveStyle_ball = 1;
                                                speed = 30;
                                        }
                                        leftOrRight = 1;
                                        state = speed_system;
                                }
                                else if (ball_col == 0x02 && ((~playerTwo_Pad_row & deviation) == deviation))
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
                                score_one++;
                        }
                        else if (leftOrRight == 2)
                        {
                                ball_col = 0x08;
                                ball_row = 0xEF;
                                moveStyle_ball = 2;
                                speed = 40;
                                leftOrRight = 1;
                                score_two++;
                        }
                        if ((score_one == 2) || (score_two == 2)) {
                                gameType = 0;
                                gameEnd = 1;
                                //score_one = score_two = 0;
                                state = start_ball;
                        }
                        if (gameType == 1) {
                                state = move_ball_one;
                        }
                        else if (gameType == 2) {
                                state = move_ball_two;
                        }
                        break;
                default:
                        state = start_ball;
                        break;
        }
        return state;
}


enum startNewGame_States {start_newgame, init_newgame, onePlayer, twoPlayer, reset};
int tick_reset(int state)
{
        unsigned char x;
        x = GetKeypadKey();
        switch (state)
        {
                case start_newgame:
                        state = init_newgame;
                        break;
                case init_newgame:
                        if (x == 'A') {
                                state = onePlayer;
                        }
                        else if (x == 'B') {
                                state = twoPlayer;
                        }
                        else {
                                state = init_newgame;
                        }
                        break;
                case onePlayer:
                        gameType = 1;
                        if (gameEnd == 1) {
                                state = reset;
                        } else {
                                state = onePlayer;
                        }
                        break;
                case twoPlayer:
                        gameType = 2;
                        if (gameEnd == 1) {
                                state = reset;
                        } else {
                                state = twoPlayer;
                        }
                        break;
                case reset:
                        playerOne_Pad_row = 0xC7;
                        playerOne_Pad_col = 0x01;
                        playerTwo_Pad_row = 0xC7;
                        playerTwo_Pad_col = 0x80;
                        AIPad_row = 0xC7;
                        AIPad_center = 0xEF;
                        AIPad_col = 0x80;
                        ball_row = 0xEF;
                        ball_col = 0x10;
                        leftOrRight = 1;
                        moveStyle_ball = 2;
                        //gameType = 0;
                        state = init_newgame;
                        break;
                default:
                        state = start_newgame;
                        break;
        }
        return state;
}

enum display_States {start_display, one_display, two_displayOne, two_displayTwo, three_display};
int tick_display(int state)
{
        switch (state)
        {
                case start_display:
                        state = one_display;
                        break;
                case one_display:
                        if (gameType == 1 || gameType == 2) {
                                PORTD = playerOne_Pad_row;
                                PORTC = playerOne_Pad_col;
                                state = two_displayOne;
                        } else {
                                state = one_display;
                        }
                        break;
                case two_displayOne:
                        if (gameType == 2) {
                                PORTD = playerTwo_Pad_row;
                                PORTC = playerTwo_Pad_col;
                        }
                        state = two_displayTwo;
                        break;
                case two_displayTwo:
                        if (gameType == 1) {
                                PORTD = AIPad_row;
                                PORTC = AIPad_col;
                        }
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


unsigned char blueLight = 0x00;
unsigned char oneLEDs = 0x00;
unsigned char twoLEDs = 0x00;
enum LED_States {start_LEDdisplay, one_LEDdisplay, two_LEDdisplay};
int tick_LEDdisplay(int state)
{
        switch (state)
        {
                case start_LEDdisplay:
                        state = one_LEDdisplay;
                        break;
                case one_LEDdisplay:
                        if (gameType == 1) {
                                blueLight = 0x01;
                        }
                        else if (gameType == 2) {
                                blueLight = 0x03;
                        }
                        if (score_one == 0) {
                                oneLEDs = 0x00;
                        }
                        if (score_one == 1) {
                                oneLEDs = 0x01;
                        }
                        if (score_one == 2) {
                                oneLEDs = 0x03;
                        }
                        if (score_two == 0) {
                                twoLEDs = 0x00;
                        }
                        if (score_two == 1) {
                                twoLEDs = 0x01;
                        }
                        if (score_two == 2) {
                                twoLEDs = 0x03;
                        }
                        state = two_LEDdisplay;
                        break;
                case two_LEDdisplay:
                        state = one_LEDdisplay;
                        break;
        }
        switch (state)
        {
                case two_LEDdisplay:
                        PORTA = 0x03 | (blueLight << 2) | (oneLEDs << 4) | (twoLEDs << 6);
                        break;
        }
        return state;
}

task tasks[7];

int main(void) {

        DDRA = 0xFC; PORTA = 0x03;
        DDRB = 0xF0; PORTB = 0x0F;
        DDRC = 0xFF; PORTC = 0x00;
        DDRD = 0xFF; PORTD = 0x00;

        tasks[0].state = start_playerOne_Pad;
        tasks[0].period = 1000;
        tasks[0].elapsedTime = 0;
        tasks[0].TickFct = &tick_playerOne_Pad;

        tasks[1].state = start_playerTwo_Pad;
        tasks[1].period = 1000;
        tasks[1].elapsedTime = 0;
        tasks[1].TickFct = &tick_playerTwo_Pad;

        tasks[2].state = start_AIPad;
        tasks[2].period = 120000;
        tasks[2].elapsedTime = 0;
        tasks[2].TickFct = &tick_AIPad;

        tasks[3].state = start_ball;
        tasks[3].period = 5000;
        tasks[3].elapsedTime = 0;
        tasks[3].TickFct = &tick_Ball;

        tasks[4].state = start_newgame;
        tasks[4].period = 1000;
        tasks[4].elapsedTime = 0;
        tasks[4].TickFct = &tick_reset;

        tasks[5].state = start_display;
        tasks[5].period = 1000;
        tasks[5].elapsedTime = 0;
        tasks[5].TickFct = &tick_display;

        tasks[6].state = start_LEDdisplay;
        tasks[6].period = 100;
        tasks[6].elapsedTime = 0;
        tasks[6].TickFct = &tick_LEDdisplay;


        TimerSet(100);
        TimerOn();

    while (1) {
                for (int i = 0; i < 7; i++)
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
