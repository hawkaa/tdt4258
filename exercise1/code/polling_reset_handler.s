.globl  _reset
.type   _reset, %function
.thumb_func
_reset: 
	.include "gpio_setup.s"	
	b withoutinter

withoutinter:
	ldrb r2, [r3, #GPIO_DIN]
	strb r2, [r1, #GPIO_DOUT]
	b withoutinter
