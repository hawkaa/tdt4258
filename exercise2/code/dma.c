#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDMA()
{
	*CMU_HFCORECLKEN0 |= CMU_HFCORECLKEN0_DMA;
	
}
