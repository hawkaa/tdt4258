/* local include */
#include "sampler.h"

/*
 * we can save alot of data here.
 */
static sample_t sample[8][20]; 
static int sample_sizes[8];
static int mode = 0;
static int note_counter = 0;
static int threshold = 0;
static int i = 0;

static float pull_counter = 0.0;
static int sample_index = 0;
static int current_sample_length = 0;

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
	if(hz < 0.01)
		threshold = 0;
	else
		threshold = get_threshold(hz);
}

/*
 * Initialize
 */
void
sampler_init() {
/* note G4, length 4 */
sample[0][0].hz = 392.000000;
sample[0][0].ms = 386.250000;

/* pause */
sample[0][1].hz = 0.000000;
sample[0][1].ms = 42.916667;

/* note G4, length 4 */
sample[0][1].hz = 392.000000;
sample[0][1].ms = 386.250000;

/* pause */
sample[0][2].hz = 0.000000;
sample[0][2].ms = 42.916667;

/* note G4, length 4 */
sample[0][2].hz = 392.000000;
sample[0][2].ms = 386.250000;

/* pause */
sample[0][3].hz = 0.000000;
sample[0][3].ms = 42.916667;

/* note E4, length 6 */
sample[0][3].hz = 329.630000;
sample[0][3].ms = 257.500000;

/* pause */
sample[0][4].hz = 0.000000;
sample[0][4].ms = 28.611111;

/* note A4, length 16 */
sample[0][4].hz = 440.000000;
sample[0][4].ms = 96.562500;

/* pause */
sample[0][5].hz = 0.000000;
sample[0][5].ms = 10.729167;

/* note G4, length 4 */
sample[0][5].hz = 392.000000;
sample[0][5].ms = 386.250000;

/* pause */
sample[0][6].hz = 0.000000;
sample[0][6].ms = 42.916667;

/* note E4, length 6 */
sample[0][6].hz = 329.630000;
sample[0][6].ms = 257.500000;

/* pause */
sample[0][7].hz = 0.000000;
sample[0][7].ms = 28.611111;

/* note A4, length 16 */
sample[0][7].hz = 440.000000;
sample[0][7].ms = 96.562500;

/* pause */
sample[0][8].hz = 0.000000;
sample[0][8].ms = 10.729167;

/* note G4, length 2 */
sample[0][8].hz = 392.000000;
sample[0][8].ms = 772.500000;

/* pause */
sample[0][9].hz = 0.000000;
sample[0][9].ms = 85.833333;

sample_sizes[0] = 10;
//	set_hz(10);
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
}


/*
 * Will return a value between 0 an 2048
 */
int
sampler_get() 
{
	pull_counter += 1.0;
	/* true when one ms has passed */
	if(pull_counter > (FREQUENCY / 1000))
	{
		pull_counter = 0.0;
		/*holds the remaining amount of millisecounds in the sample */
		current_sample_length--;
		/* true when the track needs to change sample */
		if(current_sample_length <= 0.0 && sample_sizes[0] >= sample_index)
		{

			current_sample_length = sample[0][sample_index].ms;
			set_hz(sample[0][sample_index].hz);
			/* the next sample is selected */
			++sample_index;
		}
	}

	if(threshold ==0)
		return 0;
	
	++i;
	i = i % threshold;
	return (i * 1024) / threshold;
	 
//	++i;
//	i = i % get_threshold(261.63);
//	++j;
//	j = j % get_threshold(329.63);
//	++k;
//	k = k % get_threshold(392.00);
//	return ((i * 1024) / get_threshold(261.63)) + ((j * 1024) / get_threshold(329.63)) 
//			+ ((k * 1024) / get_threshold(392.00));

}


















