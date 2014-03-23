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

static void update_square(int dx, int dy, int s_width, int s_height)
{
	update_sq.dx = dx;
	update_sq.dy = dy;
	update_sq.width = s_width;
	update_sq.height = s_height;
}

static void update_display(int x_start, int x_end, int y_start, int y_end, Color c)
{
	int x, y;
	for(x = x_start; x < x_end; ++x){
		for(y = y_start; y < y_end; ++y){
			disp[y*SCREEN_WIDTH + x] = c;	
		}
	}	
}

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

	update_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	ioctl(fd, 0x4680, &update_sq);
}

void close_screen()
{
	munmap(disp, SCREEN_SIZE);
	close(fd);
}


void draw_element(const screen_elem *old_elem, const screen_elem *new_elem) 
{
	if(old_elem != NULL){
		printf("clearing x=%d, y=%d\n", old_elem->x, old_elem->y);	
		update_display(old_elem->x, old_elem->y, 
				old_elem->x+old_elem->width, old_elem->y+old_elem->height, 0);
		update_square(old_elem->x, old_elem->y, old_elem->width, old_elem->height);
		ioctl(fd, 0x4680, &update_sq);		
	}

	if(new_elem != NULL){
		printf("drawing x=%d, y=%d\n", new_elem->x, new_elem->y);	
		update_display(new_elem->x, new_elem->x+new_elem->width, 
				new_elem->y, new_elem->y+new_elem->height, new_elem->c);
		update_square(new_elem->x, new_elem->y, new_elem->width, new_elem->height);
		ioctl(fd, 0x4680, &update_sq);
	}
}

