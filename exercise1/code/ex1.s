.syntax unified
.include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  	// Exception vector table
  	// This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
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
	      .long   systick_handler         /* SysTick Handler              */

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
	// Load CMU base address
	ldr r1, cmu_base_addr
	
	// Enable GPIO-clk
	// Load current value of HFPERCLK ENABLE
	ldr r2, [r1, #CMU_HFPERCLKEN0]

	// Set bit for GPIO-clk
	mov r3, #1
	lsl r3, r3 , #CMU_HFPERCLKEN0_GPIO 
	orr r2, r2, r3

	// Store new value
	str r2, [r1,  #CMU_HFPERCLKEN0]

	// EM3 enable (requires EM2 code enabled)
	//mov r2, 0x00000000
	//str r2, [r1, #CMU_LFCLKSEL]	

	ldr r1, gpio_pa_base
	
	// Set high drive strength
	mov r2, 0x2
	str r2, [r1, #GPIO_CTRL]
	
	// Set pins 8-15 to output
	mov r2, 0x55555555
	str r2, [r1, #GPIO_MODEH]
	
	ldr r3, gpio_pc_base
	
	// Set pins 0-7 to input
	mov r2, 0x33333333
	str r2, [r3, #GPIO_MODEL]

	//Enable internal pull-up
	mov r2, 0xff
	str r2, [r3, #GPIO_DOUT]

	//
	// INTERRUPT
	//

	// Save the GPIO C Address to register r1
	ldr r1, gpio_base

	// Write 0x22222222 to GPIO_EXTIPSELL
	mov r2, 0x22222222
	str r2, [r1, #GPIO_EXTIPSELL]

	// We want interrupt on button down
	mov r2, 0xff
	str r2, [r1, #GPIO_EXTIFALL]
	//str r2, [r1, #GPIO_EXTIRISE]

	// Enable interrupt generation
	mov r2, 0xff
	str r2, [r1, #GPIO_IEN]
	str r2, [r1, #GPIO_IFC]

	// Enable interrupt-handling
        ldr r1, iser0
        ldr r2, =0x802
        str r2, [r1]

	//
	// SysTick
	//


	// Enable SysTick
	mov r2, 0x7 
	ldr r1, st_ctrl       // Enable timer
	strb r2, [r1] 	      // Configure systick
	
	// Set count-down timer
	ldr r2, =0x00ffffff  // Longest possible interval
	ldr r1, st_load      //	Set interval-time
	str r2, [r1] 

	// Enable deep sleep (EM2) and automatic return from interrupt
        //ldr r1, scr
        //mov r2, #6
        //str r2, [r1]         
	
	// Sleep
	//wfi

	// Jump to the sleeping loop.
	b sleep_loop

	// EM4 init. WFI is no longer required. Write 2, 3, 2, 3, 2, 3, 2, 3, 2 to EMU-
	// control to enable EM4 (no easy way to wake up found).
	//ldr r1, emu_base
	//mov r2, #2
	//lsl r2, r2, #2
	//mov r3, #3
	//lsl r3, r3, #2
	//str r2, [r1, #EMU_CTRL]
	//str r3, [r1, #EMU_CTRL]
	//str r2, [r1, #EMU_CTRL]
	//str r3, [r1, #EMU_CTRL]
	//str r2, [r1, #EMU_CTRL]
	//str r3, [r1, #EMU_CTRL]
	//str r2, [r1, #EMU_CTRL]
	//str r3, [r1, #EMU_CTRL]
	//str r2, [r1, #EMU_CTRL]


// Goes to sleep. On wakeup it will return to sleep. Required for EM1. 
sleep_loop:
	wfi
	b sleep_loop
	    
	
	/////////////////////////////////////////////////////////////////////////////
	//
  	// GPIO handler
  	// The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////

// Simply read the copy values from buttons to LED's
.thumb_func
gpio_handler:  
	ldr r1, gpio_pa_base
	ldr r2, gpio_pc_base
	ldr r4, gpio_base

	ldrb r3, [r2, #GPIO_DIN]
	strb r3, [r1, #GPIO_DOUT]
	
	// reset the interrupt
	mov r3, 0xff
	str r3, [r4, #GPIO_IFC]

	// Exit interrupt-handler
	bx lr 
	
	/////////////////////////////////////////////////////////////////////////////
	//
	// SysTick Handler
	// The CPU will jump here when there is a SysTick interrupt
	//
	////////////////////////////////////////////////////////////////////////////


// Called when SysTick-interrupt occurs (timer tick). Shift lights right.
.thumb_func
systick_handler:
	// Read button value
	ldr r1, gpio_pa_base
	ldr r5, [r1, #GPIO_DIN]
	// Or with 16 bit number ending with eight 1's
	mov r6, 0x00ff
	orr r5, r5, r6
	
	// Check if we have reached the last LED in the sequence
	mov r7, 0x8000
	and r7, r5, r7
	mov r6, 0x0
	cmp r7, r6 
	// If we have not reached the last LED, add a zero to the ninth bit, else continue.
	bgt lol
	ldr r6, =0xff7f
	and r5, r5, r6
lol:
	lsl r5, r5, #1
	str r5, [r1, #GPIO_DOUT]
	// Exit SysTick-handler
	bx lr

.thumb_func
dummy_handler:  
        b .  // do nothing

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

emu_base:
	.long EMU_BASE

st_ctrl:
	.long 0xe000e010

st_load:
	.long 0xe000e014

