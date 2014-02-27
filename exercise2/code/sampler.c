/* local include */
#include "sampler.h"


/*
 * we can save alot of data here.
 */

static int mode = 0;
static int note_counter = 0;

/*
 * Initialize
 */
void
sampler_init() {

}

/*
 * Sets sampler mode
 */
void
sampler_set_mode(int mode) {
	mode = 0;
	note_counter = 0;
}

/*
 * Will return a value between 0 an 2048
 */
int
sampler_get() {
	return 0;
}

