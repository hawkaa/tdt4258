gpio_handler:
	... 
	// Reset interrupt
	mov r3, 0xff
	str r3, [r4, #GPIO_IFC]

	// Jump back, and let the code continue where it was
	bx lr 
