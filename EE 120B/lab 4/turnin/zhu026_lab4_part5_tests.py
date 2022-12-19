echo ======================================================\n
echo Running all tests..."\n\n

test "Test Lock"
set state = Init
setPINA 0x80
continue 5
expectPORTB 0x00
expect state Locked
checkResult

test "Test #XYX Unlock"
set state = Init
setPINA 0x80
continue 5
setPINA 0x04
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x02
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTB 0x01
expect state Init
checkResult

test "Test Wrong Input: #XY#"
set state = Init
setPINA 0x80
continue 5
setPINA 0x00
continue 5
setPINA 0x04
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x02
continue 5
setPINA 0x00
continue 5
setPINA 0x04
continue 5
setPINA 0x00
continue 5
expectPORTB 0x00
expect state Init
checkResult
