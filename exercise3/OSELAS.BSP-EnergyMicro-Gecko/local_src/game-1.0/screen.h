
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

typedef struct
{
	int min_x;
	int max_x;
	int min_y;
	int max_y;
} bounds_t;

extern void 
init_screen(void);

extern void 
close_screen(void);

extern void 
draw_element(const screen_elem *old_elem, const screen_elem *new_elem);

extern void 
draw_element_one_update(const screen_elem *old_elem, const screen_elem *new_elem, 
			int whole_screen);

extern void 
draw_element_y_overlap(const screen_elem *old_elem, const screen_elem *new_elem);

extern void 
draw_element_entire_screen(const screen_elem *old_elem, const screen_elem *new_elem);
#endif
