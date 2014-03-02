/* local include */
#include "sampler.h"

/* typedefs */
typedef enum {SAWTOOTH, SQUARE, TRIANGLE} signal_t;

/* constants */
//static const int FREQUENCY = 47945;
static const int FREQUENCY = 32768;
#define NUM_TRACKS 4 
static const int CHANNEL_RANGE = 2048;
static const int SAMPLER_RANGE = 2048;

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

int active_tracks = 0;
int halted = 0;

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

static inline int
is_active(int track)
{
	return ((1 << track) & active_tracks);
}

static void
set_active(int tracks)
{	
	active_tracks = tracks;
	halted = 0;
	for (int i = 0; i < NUM_TRACKS; ++i) {
		if (!is_active(i))
			continue;
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
	#include "../sampler/samples.c"	

	/* reset values */
	for (int i = 0; i < NUM_TRACKS; ++i) {
		height_threshold[i] = 0;
		current_height[i] = 0;
	}
	set_active(0b1001);
}
/*
 * Sets sampler mode
 */
void
sampler_set_mode(int mode) {
	switch(mode)
	{
	case 1:
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
		set_active(0b1000);
		break;
	case 6:
		set_active(0b0111);
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
			halted = 1;

		} else {
			current_sample_length[track] = sample[track][current_sample_index[track]].ms;
			set_hz(sample[track][current_sample_index[track]].hz, track);
		}
	}
}


/*
 * Occurs every millisecond
 */
static void
ms_tick()
{
	for(int i = 0; i < NUM_TRACKS; ++i)
		if (is_active(i))
			update_track(i);
}

/*
 * Will return a value between 0 an 2048
 */
int
sampler_get() 
{	
	if (halted) {
		return -1;
	}
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
	int signal_values = 0;
	int active_signals = 0;
	for (int i = 0; i < NUM_TRACKS; ++i) {
		if (!((1 << i) & active_tracks))
			continue;
		++active_signals;
		++current_height[i];
		current_height[i] %= height_threshold[i];
		signal_values += (current_height[i] * CHANNEL_RANGE) / height_threshold[i];
	}

	return (signal_values * SAMPLER_RANGE) / (active_signals * CHANNEL_RANGE);
}
