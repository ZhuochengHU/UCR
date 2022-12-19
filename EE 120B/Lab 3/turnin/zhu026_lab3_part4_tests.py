test "PINA: 0xFF => (0000 1111)PORTB: 0x0F, (1111 0000)PORTC: 0xF0"
setPINA 0xFF
continue 5
expectPORTB 0x0F
expectPORTC 0xF0
checkResult

test "PINA: 0x69 => (0000 0110)PORTB: 0x06, (1001 0000)PORTC: 0x90"
setPINA 0x69
continue 5
expectPORTB 0x06
expectPORTC 0x90
checkResult

test "PINA: 0xAB => (0000 1010)PORTB: 0x0A, (1011 0000)PORTC: 0xB0"
setPINA 0xAB
continue 5
expectPORTB 0x0A
expectPORTC 0xB0
checkResult