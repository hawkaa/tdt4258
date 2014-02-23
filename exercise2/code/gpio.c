/* system headers */
#include <stdint.h>
#include <stdbool.h>

/* local headers */
#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void
setupGPIO(void)
{
	 
	/* enable GPIO clock*/
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; 
	
	/* set high drive strength */
	*GPIO_PA_CTRL = 1; 

	*GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */

	/* turn off all LEDs */
	*GPIO_PA_DOUT = 0xffff;

	/* set pins 8-15 to input */
	*GPIO_PC_MODEL = 0x33333333;

	/* enable internal pull up register */
	*GPIO_PC_DOUT = 0xff;
}
