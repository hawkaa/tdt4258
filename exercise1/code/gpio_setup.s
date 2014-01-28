	// load CMU base adress
	ldr r1, cmu_base_addr
	
	// Enable GPIO-clk
	// load current value of HFPERCLK ENABLE
	ldr r2, [r1, #CMU_HFPERCLKEN0]

	// set bit for GPIO-clk
	mov r3, #1
	lsl r3, r3 , #CMU_HFPERCLKEN0_GPIO 
	orr r2, r2, r3

	// store new value
	str r2, [r1,  #CMU_HFPERCLKEN0]

	ldr r1, gpio_pa_base

	mov r2, 0x2
	str r2, [r1, #GPIO_CTRL]
	
	mov r2, 0x55555555
	str r2, [r1, #GPIO_MODEH]
	
	ldr r3, gpio_pc_base
	
	mov r2, 0x33333333
	str r2, [r3, #GPIO_MODEL]

	mov r2, 0xff
	str r2, [r3, #GPIO_DOUT]

