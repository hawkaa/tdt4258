/* local include */
#include "sampler.h"

/*
 * we can save alot of data here.
 */
static sample_t sample[8][20]; 
static int mode = 0;
static int note_counter = 0;
static int threshold = 0;
static int i = 0;

static const int FREQUENCY = 47945;

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
set_hz(float hz)
{
	i = 0;
	if(hz == 0)
		threshold = 0;
	else
		threshold = get_threshold(hz);
}

/*
 * Initialize
 */
void
sampler_init() {

	sample[0][0].hz = 150.0;
	sample[0][0].ms = 1000;
	set_hz(150);
}

/*
 * Sets sampler mode
 */
void
sampler_set_mode(int mode) {
	//mode = 0;
	note_counter = 0;
	switch(mode)
	{
	case 1:
		set_hz(261.63);
		break;
	case 2:
		set_hz(293.67);
		break;
	case 3:
		set_hz(329.63);
		break;
	case 4:
		set_hz(349.23);
		break;
	case 5:
		set_hz(392.00);
		break;
	case 6:
		set_hz(440.00);
		break;
	case 7:
		set_hz(493.88);
		break;
	case 8:
		set_hz(523.25);
		break;
	
	default:
		set_hz(0);
	
	}
}

static float pull_counter = 0.0;
static int sample_index = 0;
static int current_sample_length = 0;

/*
 * Will return a value between 0 an 2048
 */
int
sampler_get() {
	if(pull_counter > (FREQUENCY / 1000))
	{
		pull_counter = 0;
		current_sample_length--;

		if(current_sample_length == 0)
		{
			++sample_index;
			current_sample_length = sample[0][sample_index].ms;
		}
	}
	if(threshold ==0)
		return 0;
	 
	++i;
	i = i % threshold;
	
	return get_triangle_signal(i, threshold, 1024);
	return (i * 1024) / threshold;
}

