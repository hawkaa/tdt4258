
#ifndef SCREEN_H
#define SCREEN_H

typedef short Color;

typedef struct 
{
	int x;
	int y;
	int width;
	int height;
	int dx;
	int dy;
	Color c;
	
} screen_elem;

extern void 
init_screen(void);

extern void 
draw_element(screen_elem *old_elem, screen_elem *new_elem);

#endif
