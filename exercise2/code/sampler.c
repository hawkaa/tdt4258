
/*
 * we can save alot of data here.
 */

static int mode = 0;
static int tick = 0;
static int note_counter = 0;


/*
 * Sets sampler mode
 */
void
set_mode(int mode) {
	mode = 0;
	tick = 0;
	node_counter = 0;
}

/*
 * Will return a value between 0 an 2048
 */
int
get_sample() {
	return 0;
}

