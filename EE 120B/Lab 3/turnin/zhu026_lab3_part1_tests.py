test "(1)PINA: 0x01, (0)PINB: 0x00 => (1)PORTC: 0x01"
setPINA 0x01
setPINB 0x00
continue 5
expectPORTC 0x01
checkResult

test "(2)PINA: 0x03, (0)PINB: 0x00 => (2)PORTC: 0x02"
setPINA 0x03
setPINB 0x00
continue 5
expectPORTC 0x02
checkResult

test "(0)PINA: 0x00, (1)PINB: 0x01 => (1)PORTC: 0x01"
setPINA 0x00
setPINB 0x01
continue 5
expectPORTC 0x01
checkResult

test "(4)PINA: 0xF0, (4)PINB: 0x0F => (8)PORTC: 0x08"
setPINA 0xF0
setPINB 0x0F
continue 5
expectPORTC 0x08
checkResult

test "(2)PINA: 0x03, (3)PINB: 0x70 => (5)PORTC: 0x05"
setPINA 0x03
setPINB 0x70
continue 5
expectPORTC 0x05
checkResult

test "(8)PINA: 0xFF, (8)PINB: 0xFF => (16)PORTC: 0x10"
setPINA 0xFF
setPINB 0xFF
continue 5
expectPORTC 0x10
checkResult