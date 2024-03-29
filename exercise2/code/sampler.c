/* local include */
#include "sampler.h"


/*
 * Constants
 */

/* number of tracks */
#define NUM_TRACKS 7 

/* max samples per track */
#define MAX_SAMPLES 200

/* expected sample pull frequency */
static const int FREQUENCY = 32768;

/* range per track */
static const int TRACK_RANGE = 2048;

/* sampler range */
static const int SAMPLER_RANGE = 2048;

/*
 * Global data 
 */

/* saving samples and track length */
static sample_t sample[NUM_TRACKS][MAX_SAMPLES]; 
static int sample_sizes[NUM_TRACKS];

/* sample book-keeping */
static int current_sample_length[NUM_TRACKS];
static int current_sample_index[NUM_TRACKS];

/* keeping track of wave state (height) */
static int current_height[NUM_TRACKS];
static int height_threshold[NUM_TRACKS];

/* for calculating with milliseconds */
static int pull_counter = 0;

/* waveform variable */
waveform_t waveform;

/* bitmap for which tracks are currently active */
int active_tracks = 0;

/* boolean value to check if the sampler is stopped */
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

/*
 * Enumerates waveform and returns corresponding signal value
 */
static int
get_signal(int height, int threshold, int max, waveform_t wf)
{
	switch (wf) {
	case SQUARE:
		return get_square_signal(height, threshold, max);
	case TRIANGLE:
		return get_triangle_signal(height, threshold, max);
	case SAWTOOTH:
	default:
		return get_sawtooth_signal(height, threshold, max);
	}
}

/*
 * Calculates threshold for given frequency
 */
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
		/* calculate threshold */
		height_threshold[track] = get_threshold(hz);
	}
}

/*
 * Checks if current track is active
 */
static int
is_active(int track)
{
	return ((1 << track) & active_tracks);
}

/*
 * Activates a bitmap by resetting wave state
 */
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
	/* load samples */
	 #include "../sampler/samples.c"	

	/* reset values */
	for (int i = 0; i < NUM_TRACKS; ++i) {
		height_threshold[i] = 0;
		current_height[i] = 0;
	}

	/* default waveform */
	waveform = SQUARE;

	/* enable track 0,1,2 */
	set_active(0b0111);
}
/*
 * Sets sampler mode
 * This is set up to take input from GPIO
 */
void
sampler_set_mode(int mode) {
	switch(mode)
	{
	case 1:
		break;
	case 2:
		waveform = SAWTOOTH;
		break;
	case 3:
		waveform = TRIANGLE;
		break;
	case 4:
		waveform = SQUARE;
		break;
	case 5:
		set_active(0b1000);
		break;
	case 6:
		set_active(0b0111);
		break;
	case 7:
		set_active(0b110000);
		break;
	case 8:
		set_active(0b11000000);
		break;
	default:
		break;	
	}
}

/*
 * Decreases the remaing time in the sample. If there is none left,
 * current sample index is increased.
 */
void
update_track(int track)
{		
	--current_sample_length[track];
	if (current_sample_length[track] <= 0) {
		/* sample has played long enough, increase index */
		++current_sample_index[track];
		if (current_sample_index[track] >= sample_sizes[track]) {
			/* no more samples left, halt the sampler */
			halted = 1;
		} else {
			/* load new variables from next sample */
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
	/* update all enabled tracks */
	for(int i = 0; i < NUM_TRACKS; ++i)
		if (is_active(i))
			update_track(i);
}

/*
 * Will return a value between 0 an 2048.
 * Will return -1 if the sampler has nothing to play.
 */
int
sampler_get() 
{	
	if (halted) {
		/* the sampler has nothing to play */
		return -1;
	}

	/* keep track of how many times a sample has been pulled */
	++pull_counter;

	
	/* The tracks are updated once per millisecound */
	if( pull_counter >= (FREQUENCY / 1000)) {
		/* true when one ms has passed */
		pull_counter = 0;
		ms_tick();
	}

	/* accumulate signals */
	int signal_values = 0;
	int active_signals = 0;
	for (int i = 0; i < NUM_TRACKS; ++i) {
		if (!is_active(i)) {
			/* given track is not active, continue loop */
			continue;
		}

		/* update wave state (height) */
		++current_height[i];
		current_height[i] %= height_threshold[i];
		
		/* calculate signal value for track */
		++active_signals;
		signal_values += get_signal(current_height[i], height_threshold[i], TRACK_RANGE, waveform);
	}
	
	/* returns scaled signal value of the accumulated signals */
	return (signal_values * SAMPLER_RANGE) / (active_signals * TRACK_RANGE);
}
