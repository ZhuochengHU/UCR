echo ======================================================\n
echo Running all tests..."\n\n

test "Test add 1"
set state = Start
setPINA ~0x01
continue 5
expectPORTC 0x08
expect state Plus
checkResult

test "Test minus 1"
set state = Start
setPINA ~0x02
continue 5
expectPORTC 0x06
expect state Minus
checkResult

test "Test Reset"
set state = Start
setPINA ~0x03
continue 5
expectPORTC 0x00
expect state Reset
checkResult