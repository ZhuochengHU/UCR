test "(500)PIND: 0xFA, PINB: 0x00, (SEATBELT ENABLE)PORTB: 0x02"
setPIND 0xFA
setPINB 0x00
continue 5
expectPORTB 0x02
checkResult

test "(69)PIND: 0x22, PINB: 0x01, (SEATBELT DISABLE)PORTB: 0x04"
setPIND 0x22
setPINB 0x01
continue 5
expectPORTB 0x04
checkResult

test "(1)PIND: 0x00, PINB: 0x01, (NONE)PORTB: 0x00"
setPIND 0x00
setPINB 0x01
continue 5
expectPORTB 0x00
checkResult

test "EDGE (5)PIND: 0x02, PINB: 0x01, (NONE)PORTB: 0x00"
setPIND 0x00
setPINB 0x01
continue 5
expectPORTB 0x00
checkResult

test "EDGE (70)PIND: 0x23, PINB: 0x00, (SEATBELT ENABLE)PORTB: 0x02"
setPIND 0x23
setPINB 0x00
continue 5
expectPORTB 0x02
checkResult