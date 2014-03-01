/* local include */
#include "sampler.h"

#define MAX_TRACKS 8
/*
 * we can save alot of data here.
 */
static sample_t sample[MAX_TRACKS][20]; 
static int sample_sizes[MAX_TRACKS];
static int current_sample_length[MAX_TRACKS];
static int current_sample_index[MAX_TRACKS];
static int height_threshold[MAX_TRACKS];
static int height_current[MAX_TRACKS];

static sample_t current_sample[MAX_TRACKS];

static int mode = 0;
static int note_counter = 0;

static float pull_counter = 0.0;
static int sample_index = 0;

static const int FREQUENCY = 47945;

/*
 * Initialize
 */
void
sampler_init() {
	for(int j = 0; j < MAX_TRACKS; j++)
	{
		sample_sizes[j] = 0; //Sample sizes is set in tetris.c
		current_sample_length[j] = 0;
		current_sample_index[j] = 0;
		height_threshold[j] = 0;
		height_current[j] = 0;
	}
	#include "../sampler/tetris.c"	
}

static int
get_sawtooth_signal(int j, int t, int max)
{
	return (j * max) / t;
}

static int
get_triangle_signal(int j, int t, int max)
{
	if(2 * j < height_threshold[0]) {
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
	if (2 * j < height_threshold) {
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

/*
 * Sets the hz for all tracks
 */
static void
set_hz(float hz, int track)
{
	height_current[track] = 0;
	if(hz < 0.01)
		height_threshold[track] = 0;
	else
		height_threshold[track] = get_threshold(hz);
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
		set_hz(261.63, 0); //C
		sample_index = 0;
		break;
	case 2:
		set_hz(293.67, 0); //D
		break;
	case 3:
		set_hz(329.63, 0); //E
		break;
	case 4:
		set_hz(349.23, 0); //F
		break;
	case 5:
		set_hz(392.00, 0); //G
		break;
	case 6:
		set_hz(440.00, 0); //A
		break;
	case 7:
		set_hz(493.88, 0); //B
		break;
	case 8:
		set_hz(523.25, 0);
		break;
	
	default:
		set_hz(0, 0);
	
	}
}
/*
 * Decreases the remaing time in the sample. If there is none left,
 * the next sample is set to current.
 */
void
update_track(int track)
{
	pull_counter = 0.0;
	/*holds the remaining amount of millisecounds in the sample */
	current_sample_length[track]--;
	/* true when the track needs to change sample */
	if(current_sample_length[track] <= 0.0 && sample_sizes[track] >= current_sample_index[track])
	{
		current_sample_length[track] = sample[track][sample_index].ms;
		set_hz(sample[track][sample_index].hz, track);
		/* the next sample is selected */
		++sample_index;
	}
}

int
update_height()
{
	int height = 0;
	for(int i = 0; i < 3; i++)
	{
		++height_current[i];
		height += (height_current[i] % height_threshold[i]) * 1024 / height_threshold[i];
	}

	return height;
}

/*
 * Will return a value between 0 an 2048
 */
int
sampler_get() 
{
	pull_counter += 1.0;
	/*
	 * The tracks are updated once per millisecound
	 */
	if(pull_counter > (FREQUENCY / 1000))
		for(int i = 0; i < 1; i++)
			update_track(i);

	if(height_threshold[0] == 0)
		return 0;
	/*
	 * TODO: The product of all tracks are computed
	 */	
	
	return update_height();//( (height_current[0] % height_threshold[0]) * 1024) / height_threshold[0];
	 
}


















