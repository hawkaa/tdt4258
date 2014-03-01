/* local include */
#include "sampler.h"

/* constants */
static const int FREQUENCY = 47945;
static const int NUM_TRACKS 3
static const int CHANNEL_RANGE = 2048;
static const int SAMPLER_RANGE = 1024;

/*
 * we can save alot of data here.
 */

/* saving samples and sizes */
static sample_t sample[NUM_TRACKS][50]; 
static int sample_sizes[NUM_TRACKS];

/* keeping track of wave state */
static int wave_counter[NUM_TRACKS];
static int wave_threshold[NUM_TRACKS];

/* for calculating with MS */
static int pull_counter = 0;

/* current track book-keeping */
static int sample_time_left[NUM_TRACKS];
static int sample_index[NUM_TRACKS];

/* kims, dunno what the do */
//static int current_sample_length[NUM_TRACKS];
//static int current_sample_index[NUM_TRACKS];
static sample_t current_sample[8];

static int mode = 0;
static int note_counter = 0;
static int threshold = 0;
static int i = 0;

static int sample_index = 0;


/*
 * Initialize
 */
void
sampler_init(void)
{
	#include "../sampler/tetris.c"	
	for (int i = 0; i < NUM_TRACKS; ++i) {
		sample_index[i] = 0;
	}
}

static int
get_sawtooth_signal(int j, int t, int max)
{
	return (j * max) / t;
}

static int
get_triangle_signal(int j, int t, int max)
{
	if(2 * j < threshold) {
		/* on the way up */
		return (j * 2 * max) / t;
	} else {
		/* on the way down */
		return 0;
		//return ((2 * t - 2 * j) * max) / t;
	}
}

static int
get_square_signal(int j, int t, int max)
{
	if (2 * j < threshold) {
		/* first half */
		return max;
	} else {
		/* second half */
		return 0;
	}
}


static int
get_threshold(float hz) 
{
	return FREQUENCY / hz;
}

static void
set_hz(int track, float hz)
{
	/* reset counter */
	wave_counter[track] = 0;

	/* generate new threshold */
	if (hz < 0.01) {
		/* hz is too low */
		wave_threshold[track] = 0;
	} else {	
		wave_threshold[track] = get_threshold(hz);
	}
}

/*
 * Sets sampler mode
 */
void
sampler_set_mode(int mode) {
	//mode = 0;
/*	note_counter = 0;
	switch(mode)
	{
	case 1:
		set_hz(261.63); //C
		sample_index = 0;
		break;
	case 2:
		set_hz(293.67); //D
		break;
	case 3:
		set_hz(329.63); //E
		break;
	case 4:
		set_hz(349.23); //F
		break;
	case 5:
		set_hz(392.00); //G
		break;
	case 6:
		set_hz(440.00); //A
		break;
	case 7:
		set_hz(493.88); //B
		break;
	case 8:
		set_hz(523.25);
		break;
	
	default:
		set_hz(0);
	
	}
*/
}

/*
 * Decreases the remaing time in the sample. If there is none left,
 * the next sample is set to current.
 */
void
update_track(int track)
{
	/*
	--sample_time_left[track];
	
	if (current_sampe

	if(current_sample_length[track] <= 0.0 && sample_sizes[track] >= current_sample_index[track])
	{
		current_sample_length[track] = sample[track][sample_index].ms;
		set_hz(sample[track][sample_index].hz);
		++sample_index;
	}
	*/
}

/*
 * Occurs every millisecond
 */
void
ms_tick()
{
	for(int i = 0; i < NUM_TRACKS; ++i
		update_track(i);
}

/*
 * Will return a value between 0 an 2048
 */
int
sampler_get() 
{
	++pull_counter;

	if( pull_counter >= (FREQUENCY / 1000)) {
		/* true when one ms has passed */
		pull_counter = 0;
		ms_tick();
	}
	
	return 0;
	 
}

