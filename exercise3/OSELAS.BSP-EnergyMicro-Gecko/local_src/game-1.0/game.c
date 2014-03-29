/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <signal.h>

/* local includes */
#include "signal.h"
#include "input.h"
#include "timer.h"
#include "pong.h"
#include "screen.h"

int
main(int argc, char *argv[])
{	
	/* init button signal handler */
	if (button_signal_init()) {
		perror("Error initiating button signal handler");
		exit(EXIT_FAILURE);
	}

	/* init timer signal handler */
	if (timer_signal_init()) {
		perror("Error initiating timer signal handler");
		exit(EXIT_FAILURE);
	}

	/* init input module */
	if (input_init() == -1) {
		perror("Error loading input module");
		exit(EXIT_FAILURE);
	}

	if (timer_init(100000)) {
		perror("Error initiating timer module");
		exit(EXIT_FAILURE);
	}

	init_screen();		
	pong_init();
	register_timer_handler(timer_tick);
	/*
	 * Register handlers for the pong game
	 */

	register_button_down_handler(2, &up_left_button_press);
	register_button_up_handler(2, &up_left_button_release);

	register_button_down_handler(4, &down_left_button_press);
	register_button_up_handler(4, &down_left_button_release);

	register_button_down_handler(6, &up_right_button_press);
	register_button_up_handler(6, &up_right_button_release);

	register_button_down_handler(8, &down_right_button_press);
	register_button_up_handler(8, &down_right_button_release);

	register_timer_handler(&timer_tick);

	
	for (;;) {
		pause();
	}

	exit(EXIT_SUCCESS);
	
}
