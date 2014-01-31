.globl _reset
.type _reset, %function
.thumb_func
_reset:
	.include "gpio_setup.s"
	.include "interrupt_setup.s"
