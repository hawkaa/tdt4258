/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"
#include "sampler.h"

int pitch = 0;
static void
gpio_handler(void)
{
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
	/*
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	*GPIO_IFC = 0xff;
	switch(*GPIO_PC_DIN)
	{
		case 0xfe:
			pitch =  100;
			break;
		case 0xfd:
			pitch = 120;
			break;
		case 0xfb:
			pitch = 140;
			break;
		case 0xf7:
			pitch = 160;
			break;
		case 0xef:
			pitch = 180;
			break;
		case 0xdf:
			pitch = 200;
			break;
		case 0xbf:
			pitch = 220;
			break;
		case 0x7f:
			pitch = 240;
			break;		
		default:
			pitch = 0;
			break;
	} */
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
	
	*DAC0_CH0DATA = *DAC0_CH1DATA = sampler_get();
	/*
	++i;
	i = i % pitch;

	if(i == 200)
		*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;


	*DAC0_CH0DATA = *DAC0_CH1DATA = (i * 1024) / pitch;
*/
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
	*DAC0_CH0DATA = *DAC0_CH1DATA = sampler_get();
	//*GPIO_PA_DOUT = ~(*GPIO_PA_DOUT);	

	*LETIMER0_IFC  = 1;
}
