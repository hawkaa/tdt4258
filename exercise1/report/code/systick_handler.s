systick_handler:
	// Read button value
	ldr r1, gpio_pa_base
	ldr r5, [r1, #GPIO_DIN]
	// Or with 16 bit number ending with eight 1's
	mov r6, 0x00ff
	orr r5, r5, r6
	
	// Check if we have reached the last LED in the sequence
	mov r7, 0x8000
	and r7, r5, r7
	mov r6, 0x0
	cmp r7, r6 
	// If we have not reached the last LED, 
	// add a zero to the ninth bit, else continue.
	bgt lol
	ldr r6, =0xff7f
	and r5, r5, r6
lol:
	lsl r5, r5, #1
	str r5, [r1, #GPIO_DOUT]
	// Exit SysTick-handler
	bx lr
