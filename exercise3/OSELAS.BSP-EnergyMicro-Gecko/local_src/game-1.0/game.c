#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/sched.h>
#include <signal.h>

#define FILEPATH "/dev/fb0"
#define FILESIZE 320*240*2

#define GAMEPAD_DRIVER "/dev/tdt4258_gamepad"


void
signal_handler(int signum)
{
	printf("Received signal(%i)\n", signum);
	exit(0);
}


int main(int argc, char *argv[])
{
	int retval;
	struct sigaction signal_action;
	
	signal_action.sa_handler = signal_handler;
	sigemptyset(&signal_action);
	signal_action.sa_flags = 0;
	retval = sigaction(SIGUSR1, &signal_action, NULL);

	if (retval < 0) {
		printf("Sigaction error\n");
	}


	int fd = open(GAMEPAD_DRIVER, O_RDWR, (mode_t)0600);

	if (fd == -1) {
		perror("Error opening");
		exit(EXIT_FAILURE);
	}

	
	char val;
	char *val_pointer = &val;
	retval = read(fd, val_pointer, 1);
	printf("%c\n", val);

	for (;;) {
		sleep(1000);
	}
	
	exit(EXIT_SUCCESS);

	/*
	
	printf("Starting framebuffer test..\n");

	int i;
    	int fd;
    	short *map;  /* mmapped array of int's */

    	/* Open a file for writing.
     	*  - Creating the file if it doesn't exist.
     	*  - Truncating it to 0 size if it already exists. (not really needed)
     	*
     	* Note: "O_WRONLY" mode is not sufficient when mmaping.
     	*/
	/*
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
