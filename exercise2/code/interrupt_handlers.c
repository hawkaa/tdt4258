#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
void gpioh();
/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
	TIMER1_IF = 1;
	*GPIO_PA_DOUT  = 0xfafa;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
	//*GPIO_PA_DOUT = 0xfafa;
	gpioh();
}


/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
	gpioh();
}

void gpioh()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = *GPIO_PC_DIN;
		
//	*GPIO_IEN = 0xff;
}
