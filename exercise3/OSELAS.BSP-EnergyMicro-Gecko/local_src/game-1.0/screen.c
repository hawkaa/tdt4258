/* global includes  */
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>

/* local includes */
#include "screen.h"

/* screen values */
#define SCREEN_SIZE 320*240*2
#define SCREEN_WIDTH 320 
#define SCREEN_HEIGHT 240

#define SCREEN_PATH "/dev/fb0"

static short* disp;
static int fd;
static struct fb_copyarea update_sq;


void init_screen()
{
	int i;
  
        fd = open(SCREEN_PATH, O_RDWR, (mode_t)0600);
        if (fd == -1) {
                perror("Error opening file for writing");
                exit(EXIT_FAILURE);
        }               
          
        disp =(short*)mmap(0, SCREEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (disp == MAP_FAILED) {
		close(fd);
                perror("Error mmapping the file");
                exit(EXIT_FAILURE);
        }

	for(i = 0; i < SCREEN_SIZE/2; ++i){ 
		/* reset the screen */
		disp[i] = 0x0;
	}

	update_sq.dx = 0;
	update_sq.dy = 0;
	update_sq.width = SCREEN_WIDTH;
	update_sq.height = SCREEN_HEIGHT;

	ioctl(fd, 0x4680, &update_sq);
}

void close_screen()
{
	munmap(disp, SCREEN_SIZE);
	close(fd);
}


void draw_element(screen_elem old_elem, screen_elem new_elem) 
{

}

