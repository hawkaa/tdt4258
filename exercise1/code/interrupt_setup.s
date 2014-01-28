mov r2, 0x22222222
str r2, [r3, #GPIO_EXTISELL]

mov r2, 0xff
str r2, [r3, #GPIO_EXTIFALL]
str r2, [r3, #GPIO_EXTIRISE]
str r2, [r3, #GPIO_EN]

str r4, 
