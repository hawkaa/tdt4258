/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"

/* function to setup the timer */
void
setupTimer(uint16_t period)
{
  /*
    TODO enable and set up the timer
    
    1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
    2. Write the period to register TIMER1_TOP
    3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
    4. Start the timer by writing 1 to TIMER1_CMD
    
    This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.
  */ 
	//*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1 //set bit 6	
	//*TIMER1_TOP = 100;//period;
	//*TIMER1_IEN = 1;
	//*ISER0 |= (1 << 12);	

	*CMU_HFPERCLKEN0 |= 1 << 6;
	*TIMER1_TOP = period;
	*TIMER1_IEN = 1;
	*TIMER1_CMD = 1;

}

void setupLETimer(uint16_t period)
{
	/* enable clock for low energy timers */
	*CMU_HFCORECLKEN0 |= 1 << 4; 

	/* enable low energy oscilator */
	*CMU_OSCENCMD |= 1 << 6;

	/* enable low energy timer */
	*CMU_LFACLKEN0 |= CMU_LETIMER0_EN;
	
	/* use free mode */
	*LETIMER0_CTRL |= 1 << 9; 

	/* set period */
	*LETIMER0_TOP = period;

	/* enable interrupts */
	*LETIMER0_IEN = 1;

	/* start timer */
	*LETIMER0_CMD = 1;
}
