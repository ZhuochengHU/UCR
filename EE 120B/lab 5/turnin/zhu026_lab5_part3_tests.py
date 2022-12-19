echo ======================================================\n
echo Running all tests..."\n\n

test "Test add 1"
set state = Start
continue 5
setPINA ~0x01
continue 5
expectPORTC 0x01
expect state Plus
checkResult

test "Test add 8"
set state = Start
continue 5
setPINA ~0x01
continue 5
setPINA ~0x00
continue 5
setPINA ~0x01
continue 5
setPINA ~0x00
continue 5
setPINA ~0x01
continue 5
setPINA ~0x00
continue 5
setPINA ~0x01
continue 5
setPINA ~0x00
continue 5
setPINA ~0x01
continue 5
setPINA ~0x00
continue 5
setPINA ~0x01
continue 5
expectPORTC 0x7F
expect state Plus
checkResult