// Load the EMU_BASE
ldr r1, emu_base

// Loading the correct bytes into r2 and r3
mov r2, #2
lsl r2, r2, #2
mov r3, #3
lsl r3, r3, #2

// Byte sequence
str r2, [r1, #EMU_CTRL]
str r3, [r1, #EMU_CTRL]
... // Repeat so the sequence ends up with 232323232

