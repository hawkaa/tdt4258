/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"


static void
gpio_handler(void)
{
//	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
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
	i = i % 291;



	*DAC0_CH0DATA = *DAC0_CH1DATA = (i * 1024) / 291;

	*TIMER1_IFC = 1;
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

