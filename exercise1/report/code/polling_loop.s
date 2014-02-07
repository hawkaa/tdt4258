main_loop:
	// Base addresses
	ldr r1, gpio_pa_base
	ldr r2, gpio_pc_base
	
	// Load input register into output register
	ldrb r3, [r2, #GPIO_DIN]
	strb r3, [r1, #GPIO_DOUT]

	// Restart loop
	b main_loop