
#ifndef PONG_H
#define PONG_H

typedef struct 
{
	int x;
	int y;
	int width;
	int height;
} screen_elem;

extern void 
timer_tick(void);

extern void 
init_pong(short screen, int screen_size);

extern void
up_left_button_press(void);

extern void
up_left_button_release(void);

extern void 
down_left_button_press(void);

extern void
down_left_button_release(void);

extern void 
up_right_button_press(void);

extern void
up_right_button_release(void);

extern void 
down_right_button_press(void);

extern void
down_right_button_release(void);


#endif
