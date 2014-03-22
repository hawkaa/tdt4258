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

/* constants */

#define FILEPATH "/dev/fb0"
#define FILESIZE 320*240*2

#define GAMEPAD_DRIVER "/dev/tdt4258_gamepad"

int
main(int argc, char *argv[])
{
	
	/* init signal handler */
	if (signal_handler_init()) {
		perror("Error initiating signal handler");
		exit(EXIT_FAILURE);
	}

	/* init input module */
	if (input_init() == -1) {
		perror("Error loading input module");
		exit(EXIT_FAILURE);
	}
	
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

    	* Open a file for writing.
     	*  - Creating the file if it doesn't exist.
     	*  - Truncating it to 0 size if it already exists. (not really needed)
     	*
     	* Note: "O_WRONLY" mode is not sufficient when mmaping.
     	*
	*
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
