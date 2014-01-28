// r6 reserved for GPIO_BASE
ldr r6, gpio_base 

mov r2, 0x22222222
str r2, [r6, #GPIO_EXTIPSELL]

mov r5, 0xff
str r5, [r6, #GPIO_EXTIFALL]
str r5, [r6, #GPIO_EXTIRISE]
str r5, [r6, #GPIO_IEN]

// r4 reserved for ISER0
ldr r4, iser0
ldr r2,=0x802

str r2, [r4]



 
