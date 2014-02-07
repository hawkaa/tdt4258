// load CMU base address
ldr r1, cmu_base_addr

// Write 0 to the CMU_LFCLKSEL
mov r2, 0x00000000
str r2, [r1, #CMU_LFCLKSEL]

