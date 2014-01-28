.thumb_func
gpio_handler:
	ldrb r2, [r3, #GPIO_DIN]
	strb r2, [r1, #GPIO_DOUT]
	str r5, [r6, #GPIO_IFC]
	bx LR
