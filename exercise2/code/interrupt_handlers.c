/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"
#include "sampler.h"

#ifndef SAMPLE_PERIOD
#define SAMPLE_PERIOD 1
#endif

extern void setupLETimer(uint16_t period);

int pitch = 0;
static int prev_dac_value = 0;

static void
gpio_handler(void)
{	
	if(prev_dac_value == 0){
		/* we want to start the timer again after sleep  */
		*CMU_LFCLKSEL = 1;
		setupLETimer(SAMPLE_PERIOD);
	}
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
	*GPIO_IFC = 0xff;
}


void __attribute__ ((interrupt))
TIMER1_IRQHandler(void)
{
	/*
	 * TODO
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


/* LETIMER interrupt handler */
void __attribute__((interrupt))
LETIMER0_IRQHandler()
{	
	prev_dac_value = sampler_get();
	if(prev_dac_value == -1){
		/* played long enough, disable low energy timer */
		*CMU_LFCLKSEL = 0;
	}else{
		*DAC0_CH0DATA = *DAC0_CH1DATA = sampler_get();
	}	

	*LETIMER0_IFC  = 1;
}
