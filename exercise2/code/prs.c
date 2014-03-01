/* system headers */
#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* This method sets up the PRS with producer timer 1
   and consumer DAC */
void 
setupPRS(void)
{	
	// Setup the PRS-clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_PRS;
	
	// Setup trigger Timer1 to chanel 0 in PRS
	*PRS_CH0_CTRL |=  PRS_SOURCE_TIMER1;

	// Select overflow signal from Timer1
	*PRS_CH0_CTRL |= PRS_SIGSEL_TIMER1_OF;

	/* In DAC, write to DAC0_CH(0-1)CTRL to PRSEN (set bit 2 to 1) and		PRSSEL (set bit 4-7 to value 0x1)  */
	*DAC0_CH0CTRL |= DAC_CH_PRSEN;

	*DAC0_CH1CTRL |= DAC_CH_PRSEN;

	*DAC0_CH0CTRL |= DAC_CH_PRSSEL;

	*DAC0_CH1CTRL |= DAC_CH_PRSSEL; 
}
