/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"


static void
gpio_handler(void)
{
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	*GPIO_IFC = 0xff;
}

static int i = 0;
void __attribute__ ((interrupt))
TIMER1_IRQHandler(void)
{
	/*
	 * TODO
	 * Feed new samples to the DAC. Remember to clear the pending
	 * interrupt by writing 1 to TIMER1_IFC
	 */

	++i;
	if(i % 100 == 0) {
		*GPIO_PA_DOUT = ~(*GPIO_PA_DOUT);
	}
//	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;

	//*GPIO_PA_DOUT = 0xaaff;

	*TIMER1_IFC = 1;
	//TIMER1_IF = 1;
	//*GPIO_PA_DOUT  = 0xfafa;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt))
GPIO_EVEN_IRQHandler() 
{
	gpio_handler();
}


/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt))
GPIO_ODD_IRQHandler() 
{
	gpio_handler();
}

