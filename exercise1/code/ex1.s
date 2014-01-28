  	.syntax unified
	
	      .include "efm32gg.s"

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
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
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
  	// The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
		.thumb_func
_reset: 
	// load CMU base adress
	ldr r1, cmu_base_addr

	// load current value of HFPERCLK ENABLE
	ldr r2, [r1, #CMU_HFPERCLKEN0]

	// set bit for GPIO clk
	mov r3, #1
	lsl r3, r3 , #CMU_HFPERCLKEN0_GPIO 
	orr r2, r2, r3

	// store new value
	str r2, [r1,  #CMU_HFPERCLKEN0]

	ldr r1, gpio_pa_base_addr

	mov r2, 0x2
	str r2, [r1, #GPIO_CTRL]
	
	mov r2, 0x55555555
	str r2, [r1, #GPIO_MODEH]

	// So this was output, lets configure input

	ldr r4, gpio_pc_base_addr

	mov r2, 0x33333333
	str r2, [r4, #GPIO_MODEL]
	
	mov r2, 0xff
	str r2, [r4, #GPIO_DOUT]
	b event_loop


event_loop:

	
	// Load input register value
	ldrb r2, [r4, #GPIO_DIN] 

	// See if we can turn on some lights!!

	strb r2, [r1, #GPIO_DOUT]

	b event_loop



///////////////////////////////////////////////////////////
//
// Base addresses
// This section contains the base addresses of various
// units, like the GPIO ports and CMU
//
///////////////////////////////////////////////////////////


gpio_pa_base_addr:
	.long GPIO_PA_BASE

gpio_pc_base_addr:
	.long GPIO_PC_BASE

cmu_base_addr:
	.long CMU_BASE

	/////////////////////////////////////////////////////////////////////////////
	//
  	// GPIO handler
  	// The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	

.thumb_func
gpio_handler:  
	b .  // do nothing
		
        
.thumb_func
dummy_handler:  
        b .  // do nothing

