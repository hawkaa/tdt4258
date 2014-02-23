/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"


static void
gpio_handler(void)
{
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
}


/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
	//TIMER1_IF = 1;
	//*GPIO_PA_DOUT  = 0xfafa;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt))
GPIO_EVEN_IRQHandler() 
{
	/* 
	 * TODO
	 * Handle button pressed event, remember to clear pending interrupt
	 */
	gpio_handler();
}


/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt))
GPIO_ODD_IRQHandler() 
{
	/* 
	 * TODO
	 * Handle button pressed event, remember to clear pending interrupt
	 */
	gpio_handler();
}

