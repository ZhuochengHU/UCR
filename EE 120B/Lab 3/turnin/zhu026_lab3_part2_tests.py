test "(1)PINA: 0x01 => (0 1 100000)PORTC: 0x60"
setPINA 0x01
continue 5
expectPORTC 0x60
checkResult

test "(3)PINA: 0x03 => (0 1 110000)PORTC: 0x70"
setPINA 0x03
continue 5
expectPORTC 0x70
checkResult

test "(6)PINA: 0x06 => (0 0 111000)PORTC: 0x38"
setPINA 0x06
continue 5
expectPORTC 0x38
checkResult

test "(7)PINA: 0x07 => (0 0 111100)PORTC: 0x3C"
setPINA 0x07
continue 5
expectPORTC 0x3C
checkResult

test "(B)PINA: 0x0B => (0 0 111110)PORTC: 0x3E"
setPINA 0x0B
continue 5
expectPORTC 0x3E
checkResult

test "(F)PINA: 0x0F => (0 0 111111)PORTC: 0x3F"
setPINA 0x0F
continue 5
expectPORTC 0x3F
checkResult

test "(0)PINA: 0x00 => (0 1 000000)PORTC: 0x40"
setPINA 0x00
continue 5
expectPORTC 0x40
checkResult