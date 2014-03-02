/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"
#include "timer.h"
#include "gpio.h"
#include "dac.h"
#include "sampler.h"

/* 
 * Calculate the appropriate sample period for the sound wave(s) 
 * you want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits.
 */

/* The period between sound samples, in clock cycles. */
#ifndef   SAMPLE_PERIOD
#define   SAMPLE_PERIOD  0
#endif

/*
 * Sets up NVIC
 */
static void
setupNVIC(void)
{
	/*
	 * Use the NVIC ISERx registers to enable handling of interrupt(s)
	 * remember two things are necessary for interrupt handling:
	 * - the peripheral must generate an interrupt signal
	 * - the NVIC must be configured to make the CPU handle the signal
	 * You will need TIMER1, GPIO odd and GPIO even interrupt handling for
	 * this assignment.
	 */

	/* select input port for external interrupt low */
	*GPIO_EXTIPSELL = 0x22222222;

	/* enable interrupt generation, both button up and button down */
	*GPIO_EXTIFALL = 0xff;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_IEN = 0xff; //interupt generation

	/*
	 * Enable interrupt handling.
	 * 1 - Odd GPIO interrupt handler
	 * 11 - Even GPIO interrupt handler
	 * 26 - Low energy timer
	 */
	*ISER0 |= (1<<1) | (1<<11) | (1<<26) ;
}

/*
 * Enables energy mode 2
 */
static void 
setupEM2(void)
{
	*SCR = 0x6;
}

/*
 * Entry function, sets up the controller
 */
int
main(void) 
{  
	/* Call the peripheral setup functions */

	/* GPIO */
	setupGPIO();
	
	/* DAC */
	setupDAC();

	/* low energy timer */
  	setupLETimer(SAMPLE_PERIOD);

	/* interrupt handling */
	setupNVIC();

	/* init sampler */
	sampler_init();

	/* enable EM2  */
	setupEM2();
	
	/* enter deep sleep state */
	__asm__("wfi;");
	return 0;
}
