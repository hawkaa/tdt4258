.globl _reset
.type _reset, %function
.thumb_func
_reset:
	.include "interrupt_setup.s"

