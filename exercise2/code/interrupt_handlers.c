/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"
#include "sampler.h"
#include "timer.h"

static void
gpio_handler(void)
{	
	/* set sampler mode */	
	switch (*GPIO_PC_DIN) {
	case 0xfe:
		sampler_set_mode(1);
		break;
	case 0xfd:
		sampler_set_mode(2);
		break;
	case 0xfb:
		sampler_set_mode(3);
		break;
	case 0xf7:
		sampler_set_mode(4);
		break;
	case 0xef:
		sampler_set_mode(5);
		break;
	case 0xdf:
		sampler_set_mode(6);
		break;
	case 0xbf:
		sampler_set_mode(7);
		break;
	case 0x7f:
		sampler_set_mode(8);
		break;		
	default:
		sampler_set_mode(0);
		break;
	}
	
	if (*CMU_STATUS << 6) {
		/* we want to start the timer again after sleep  */
		startLETimer();
	}
	
	/* clear interrupt flag */
	*GPIO_IFC = 0xff;
}

/*
 * High frequency timer interrupt handler.
 * Not used in this appliaction
 */
void __attribute__ ((interrupt))
TIMER1_IRQHandler(void)
{
	/*
	 * Feed new samples to the DAC. Remember to clear the pending
	 * interrupt by writing 1 to TIMER1_IFC
	 */
	
	*DAC0_CH0DATA = *DAC0_CH1DATA = sampler_get();
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


/*
 * LETIMER interrupt handler
 */
void __attribute__((interrupt))
LETIMER0_IRQHandler()
{	
	int dac_value = sampler_get();
	if (dac_value == -1){
		/*
		 * Sampler indicates that there is nothing to play. Go to
		 * sleep.
		 */
		stopLETimer();
	} else {
		/* put sampler value on DAC */
		*DAC0_CH0DATA = *DAC0_CH1DATA = dac_value;
	}	
	
	/* clear interrupt */
	*LETIMER0_IFC  = 1;
}
