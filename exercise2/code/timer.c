/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"

/*
 * Function to setup the high frequency timer.
 * This function is not used in this system
 */
void
setupTimer(uint16_t period)
{
	/*
	 * Enable and set up the timer
	 * 1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
	 * 2. Write the period to register TIMER1_TOP
	 * 3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
	 * 4. Start the timer by writing 1 to TIMER1_CMD
	 *
	 * This will cause a timer interrupt to be generated every (period)
	 * cycles. Remember to configure the NVIC as well, otherwise the
	 * interrupt handler will not be invoked.
	 */

	/* enable clock for high frequency timer */
	*CMU_HFPERCLKEN0 |= 1 << 6;

	/* set period */
	*TIMER1_TOP = period;

	/* enable interrupts */
	*TIMER1_IEN = 1;

	/* start timer */
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

void
startLETimer(void)
{
	/* enable low frequency timers */
	*CMU_LFCLKSEL = 1;
}

void
stopLETimer(void)
{
	/* disable low frequency timers */
	*CMU_LFCLKSEL = 0;
}
