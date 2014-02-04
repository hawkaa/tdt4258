  	.syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
 	// Exception vector table
	// This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   0x1000               /* Top of Stack                 */
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
	      .long   0x44           /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   0x6c           /* GPIO odd handler */
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

	mov r2, 0xff
	str r2, [r1, #GPIO_DOUT]

	
	ldr r3, gpio_pc_base
	
	mov r2, 0x33333333
	str r2, [r3, #GPIO_MODEL]

	mov r2, 0xffffffff
	str r2, [r1, #GPIO_DOUT]

	// r6 reserved for GPIO_BASE
	ldr r6, gpio_base 

	mov r2, 0x22222222
	str r2, [r6, #GPIO_EXTIPSELL]

	mov r5, 0xff
	str r5, [r6, #GPIO_EXTIFALL]
	str r5, [r6, #GPIO_EXTIRISE]
	str r5, [r6, #GPIO_IFC]
	str r5, [r6, #GPIO_IEN]

	// r4 reserved for ISER0
	ldr r4, iser0
	ldr r2,=0x802

	str r2, [r4]
	
	mov r2, #6
	ldr r7, scr
	str r2, [r7]
	push {lr}
	b dummy_handler
	

	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
	//.include "gpio_interrupt_handler.s"
.thumb_func
gpio_handler:
	//ldr r3, gpio_pc_base
	//ldr r1, gpio_pa_base
	//ldr r5, gpio_base
	mov r5, 0xff
	ldr r2, [r3, #GPIO_DIN]
	str r2, [r1, #GPIO_DOUT]
	str r5, [r6, #GPIO_IFC]
	pop {pc}
	bx lr

	
	/////////////////////////////////////////////////////////////////////////////

.thumb_func
dummy_handler:  
        push {lr}
	wfi
	pop {pc}
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
