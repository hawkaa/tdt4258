// Loading the address of SCR (0xe000ed10) into r1
ldr r1, scr

// Setting the value of SCR to the constant 6
mov r2, #6
str r2, [r1]

// Going to sleep
wfi

