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

static int count = 0;
static int direction = 1;
static int digital = 0;
void __attribute__ ((interrupt))
TIMER1_IRQHandler(void)
{
	/*
	 * TODO
	 * Feed new samples to the DAC. Remember to clear the pending
	 * interrupt by writing 1 to TIMER1_IFC
	 */
	if(count % 298)
	{
		*DAC0_CH0CTRL = digital;
		*DAC0_CH1CTRL = digital;
	}

	count++;
	digital += direction;
	
//	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	
//	*TIMER1_IFC = 1;
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

