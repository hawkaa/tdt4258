// Enable SysTick
mov r2, 0x7 
ldr r1, st_ctrl       // Enable timer
strb r2, [r1] 	      // Configure systick
	
// Set count-down timer
ldr r2, =0x00ffffff  // Longest possible interval
ldr r1, st_load      //	Set interval-time
str r2, [r1] 
