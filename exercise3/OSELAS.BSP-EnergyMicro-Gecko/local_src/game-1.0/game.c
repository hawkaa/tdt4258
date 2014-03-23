/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <signal.h>

/* local includes */
#include "signal.h"
#include "input.h"
#include "timer.h"
#include "pong.h"

/* constants */

#define FILEPATH "/dev/fb0"
#define FILESIZE 320*240*2

#define GAMEPAD_DRIVER "/dev/tdt4258_gamepad"

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

	if (timer_init(1000)) {
		perror("Error initiating timer module");
		exit(EXIT_FAILURE);
	}

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

	/*	
	char val;
	char *val_pointer = &val;
	retval = read(fd, val_pointer, 1);
	printf("%c\n", val);
	*/
	/*
	
	printf("Starting framebuffer test..\n");

	int i;
    	int fd;
    	short *map;   mmapped array of int's 

    	fd = open(FILEPATH, O_RDWR, (mode_t)0600);
    	if (fd == -1) {
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
    	}		
	
	map =(short*)mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    	}
    	
	struct fb_copyarea rect;
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;

	for(i = 0; i < 320*240; ++i){
		map[i] = i%0xffff; // blue
	}

	ioctl(fd, 0x4680, &rect);
	
	printf("Ending framebuffer test..\n");
	exit(EXIT_SUCCESS);*/
}
