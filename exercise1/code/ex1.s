	.syntax unified
	.include "efm32gg.s"
	.section .vectors
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler           /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler           /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  	// The CPU will start executing here after a reset	//
	/////////////////////////////////////////////////////////////////////////////

	//.include "polling_reset_handler.s"
	//.include "interrupt_reset_handler.s"
	
.globl _reset
.type _reset, %function
.thumb_func
_reset:

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
	
	// Set one light
	mov r3, 0x22
	strb r3, [r1, #GPIO_DOUT]

	
	//
	// INTERRUPT
	//

	// Save the GPIO C Address to register r1
	ldr r1, gpio_base

	// Write 0x22222222 to GPIO_EXTIPSELL
	mov r2, 0x22222222
	str r2, [r1, #GPIO_EXTIPSELL]

	// We want on button down
	mov r2, 0xff
	str r2, [r1, #GPIO_EXTIFALL]
	str r2, [r1, #GPIO_EXTIRISE]

	// Enable interrupt generation
	mov r2, 0xff
	str r2, [r1, #GPIO_IEN]
	str r2, [r1, #GPIO_IFC]


	//
	ldr r1, iser0
	ldr r2, =0x802
	str r2, [r1]

	b .

main_loop:
	//ldrb r2, [r3, #GPIO_DIN]
	//strb r2, [r1, #GPIO_DOUT]	
	b main_loop

.thumb_func
gpio_handler:
	ldr r1, gpio_pa_base
	ldr r2, gpio_pc_base
	ldr r4, gpio_base
	
	// change some led lights
	//mov r3, 0x33
	//strb r3, [r1, #GPIO_DOUT]
	ldrb r3, [r2, #GPIO_DIN]
	strb r3, [r1, #GPIO_DOUT]	
	
	// reset the interrupt
	mov r3, 0xff
	str r3, [r4, #GPIO_IFC]


	bx lr 

	
.thumb_func
dummy_handler:
	b .
//.include "base_addresses.s"
cmu_base_addr:
	.long CMU_BASE

gpio_pa_base:
	.long GPIO_PA_BASE

gpio_pc_base:
	.long GPIO_PC_BASE

gpio_base:
	.long GPIO_BASE

iser0:
	.long ISER0

scr:
	.long SCR



