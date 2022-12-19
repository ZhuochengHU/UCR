// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
// Keypad arrangement
//        Px4 Px5 Px6 Px7
//        col 1   2   3   4
//  row  ______________
//Px0 1 | 1 | 2 | 3 | A
//Px1 2 | 4 | 5 | 6 | B
//Px2 3 | 7 | 8 | 9 | C
//Px3 4 | * | 0 | # | D

#ifndef KEYPAD_H
#define KEYPAD_H

#include <bit.h>

// Keypad Setup Values
#define KEYPADPORT PORTC
#define KEYPADPIN  PINC
#define ROW1 0
#define ROW2 1
#define ROW3 2
#define ROW4 3
#define COL1 4
#define COL2 5
#define COL3 6
#define COL4 7

////////////////////////////////////////////////////////////////////////////////
//Functionality - Gets input from a keypad via time-multiplexing
//Parameter: None
//Returns: A keypad button press else '\0'
unsigned char GetKeypadKey() {

        PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
        asm("nop"); // add a delay to allow PORTC to stabilize before checking
        if (GetBit(PINC,0)==0) { return('1'); }
        if (GetBit(PINC,1)==0) { return('4'); }
        if (GetBit(PINC,2)==0) { return('7'); }
        if (GetBit(PINC,3)==0) { return('*'); }

        // Check keys in col 2
        PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
        asm("nop"); // add a delay to allow PORTC to stabilize before checking
        if (GetBit(PINC,0)==0) { return('2'); }
        if (GetBit(PINC,1)==0) { return('5'); }
        if (GetBit(PINC,2)==0) { return('8'); }
        if (GetBit(PINC,3)==0) { return('0'); }

        // Check keys in col 3
        PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
        asm("nop"); // add a delay to allow PORTC to stabilize before checking
        if (GetBit(PINC,0)==0) { return('3'); }
        if (GetBit(PINC,1)==0) { return('6'); }
        if (GetBit(PINC,2)==0) { return('9'); }
        if (GetBit(PINC,3)==0) { return('#'); }

        // Check keys in col 4  
        PORTC = 0x7F;
        asm("nop"); // add a delay to allow PORTC to stabilize before checking
        if (GetBit(PINC,0)==0) { return('A'); }
        if (GetBit(PINC,1)==0) { return('B'); }
        if (GetBit(PINC,2)==0) { return('C'); }
        if (GetBit(PINC,3)==0) { return('D'); }

        return('\0'); // default value
}

#endif //KEYPAD_H

