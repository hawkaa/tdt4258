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
		update_display(old_elem->x, old_elem->x+old_elem->width, 
				old_elem->y, old_elem->y+old_elem->height, 0);
		update_square(old_elem->x, old_elem->y, old_elem->width, old_elem->height);

		ioctl(fd, 0x4680, &update_sq);		
	}
	if(new_elem != NULL){
		update_display(new_elem->x, new_elem->x+new_elem->width, 
				new_elem->y, new_elem->y+new_elem->height, new_elem->c);
		update_square(new_elem->x, new_elem->y, new_elem->width, new_elem->height);
		ioctl(fd, 0x4680, &update_sq);
	}
}

static bounds_t
get_bounds(const screen_elem *elem1, const screen_elem *elem2)
{
	bounds_t bounds;	

	if(elem1->x < elem2->x)
		bounds.min_x = elem1->x;
	else bounds.min_x = elem2->x;

	if(elem1->y < elem2->y)
		bounds.min_y = elem1->y;
	else bounds.min_y = elem2->y;	
	
	if(elem1->x + elem1->width > elem2->x + elem2->width)
		bounds.max_x = elem1->x + elem1->width;
	else bounds.max_x = elem2->x + elem2->width;

	if(elem1->y + elem1->height > elem2->y + elem2->height)
		bounds.max_y = elem1->y + elem1->height;
	else bounds.max_y = elem2->y + elem2->height;

	return bounds;
}

void draw_element_one_update(const screen_elem *old_elem, const screen_elem *new_elem, 
				int whole_screen)
{

	
	bounds_t b = get_bounds(old_elem, new_elem);

	update_display(old_elem->x, old_elem->x+old_elem->width, 
				old_elem->y, old_elem->y+old_elem->height, 0);
	update_display(new_elem->x, new_elem->x+new_elem->width, 
				new_elem->y, new_elem->y+new_elem->height, new_elem->c);
	if(whole_screen) update_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	else update_square(b.min_x, b.min_y, b.max_x-b.min_x+1, b.max_y-b.min_y+1);
	ioctl(fd, 0x4680, &update_sq);
}

void draw_element_y_overlap(const screen_elem *old_elem, const screen_elem *new_elem)
{
	int dy;
	
	dy = new_elem->y - old_elem->y;
	if(dy > 0){
		update_display(old_elem->x, old_elem->x +old_elem->width,
				old_elem->y, new_elem->y, 0);
		update_display(old_elem->x, old_elem->x +old_elem->width,
				old_elem->y+old_elem->height, new_elem->y+new_elem->height, 
				new_elem->c);

	} else {
		update_display(old_elem->x, old_elem->x +old_elem->width,
				new_elem->y+new_elem->height, old_elem->y+old_elem->height, 
				0);
		update_display(old_elem->x, old_elem->x +old_elem->width,
				new_elem->y, old_elem->y, new_elem->c);
	}

	bounds_t b = get_bounds(old_elem, new_elem);
	update_square(b.min_x, b.min_y, b.max_x-b.min_x+1, b.max_y-b.min_y+1);
	ioctl(fd, 0x4680, &update_sq);

}






