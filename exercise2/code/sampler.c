/* local include */
#include "sampler.h"

/* typedefs */
typedef enum {SAWTOOTH, SQUARE, TRIANGLE} signal_t;

/* constants */
#define NUM_TRACKS 3
static const int FREQUENCY = 47945;
static const int CHANNEL_RANGE = 2048;
static const int SAMPLER_RANGE = 1524;

/*
 * Global data 
 */


/* saving samples and sizes */
static sample_t sample[NUM_TRACKS][100]; 
static int sample_sizes[NUM_TRACKS];

/* sample book-keeping */
static int current_sample_length[NUM_TRACKS];
static int current_sample_index[NUM_TRACKS];

/* keeping track of wave state (height) */
static int current_height[NUM_TRACKS];
static int height_threshold[NUM_TRACKS];

/* for calculating with MS */
static int pull_counter = 0;

/* signal variable */
signal_t signal;

/*
 * Will generate a sawtooth signal
 */
static int
get_sawtooth_signal(int height, int threshold, int max)
{
	return (height * max) / threshold;
}

/*
 * Will generate a triangle signal
 */
static int
get_triangle_signal(int height, int threshold, int max)
{
	//return (height * max) / threshold;
	if(2 * height <= threshold) {
		/* on the way up */
		return (2 * height * max) / threshold;
	} else {
		return ((2 * threshold - 2 * height) * max) / threshold;
		return 0;
	}
}

/*
 * Will generate a square signal
 */
static int
get_square_signal(int height, int threshold, int max)
{
	if (2 * height < threshold) {
		/* first half */
		return max;
	} else {
		/* second half */
		return 0;
	}
}

static int
get_signal(int height, int threshold, int max, signal_t sig)
{
	switch (sig) {
	case SQUARE:
		return get_square_signal(height, threshold, max);
	case TRIANGLE:
		return get_triangle_signal(height, threshold, max);
	case SAWTOOTH:
	default:
		return get_sawtooth_signal(height, threshold, max);
	}
}

static int
get_threshold(float hz) 
{
	return FREQUENCY / hz;
}

/*
 * Sets the hz for given track
 */
static void
set_hz(float hz, int track)
{
	/* reset counter */
	current_height[track] = 0;

	/* generate new threshold */
	if (hz < 0.01) {
		/* hz is too low */
		height_threshold[track] = 0;
	} else {	
		height_threshold[track] = get_threshold(hz);
	}
}

static void
reset_samples()
{	
	for (int i = 0; i < NUM_TRACKS; ++i) {
		current_sample_index[i] = -1;
		current_sample_length[i] = 0;
	}
}

/*
 * Initialize
 */
void
sampler_init(void)
{
	signal = SQUARE;

	#include "../sampler/tetris.c"	

	/* reset values */
	for (int i = 0; i < NUM_TRACKS; ++i) {
		height_threshold[i] = 0;
		current_height[i] = 0;
	}
	reset_samples();
}
/*
 * Sets sampler mode
 */
void
sampler_set_mode(int mode) {
	switch(mode)
	{
	case 1:
		reset_samples();
		break;
	case 2:
		signal = SAWTOOTH;
		break;
	case 3:
		signal = TRIANGLE;
		break;
	case 4:
		signal = SQUARE;
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	default:
		break;	
	}
}
/*
 * Decreases the remaing time in the sample. If there is none left,
 * the next sample is set to current.
 */
void
update_track(int track)
{	
	
	--current_sample_length[track];
	
	if (current_sample_length[track] <= 0) {
		++current_sample_index[track];
		if (current_sample_index[track] >= sample_sizes[track]) {
			reset_samples();
		} else {
			current_sample_length[track] = sample[track][current_sample_index[track]].ms;
			set_hz(sample[track][current_sample_index[track]].hz, track);
		}
	}
}


/*
 * Occurs every millisecond
 */
void
ms_tick()
{
	for(int i = 0; i < NUM_TRACKS; ++i)
		update_track(i);
}

/*
 * Will return a value between 0 an 2048
 */
int
sampler_get() 
{
	++pull_counter;
	
	/*
	 * The tracks are updated once per millisecound
	 */	
	if( pull_counter >= (FREQUENCY / 1000)) {
		/* true when one ms has passed */
		pull_counter = 0;
		ms_tick();
	}

	/* accumulate signals */
	int signals = 0;
	for (int i = 0; i < NUM_TRACKS; ++i) {
		++current_height[i];
		current_height[i] %= height_threshold[i];
		signals += get_signal(current_height[i], height_threshold[i],
				CHANNEL_RANGE, signal);
	}

	return (signals * SAMPLER_RANGE) / (NUM_TRACKS * CHANNEL_RANGE);
}
