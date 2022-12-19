echo ======================================================\n
echo Running all tests..."\n\n

test "Test no button press"
set state = OffRelease
setPINA 0x00
continue 5
expect state OffRelease
checkResult