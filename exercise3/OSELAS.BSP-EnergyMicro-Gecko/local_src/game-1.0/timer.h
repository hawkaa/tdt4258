#ifndef TIMER_H_
#define TIMER_H_

extern int
timer_init(long int usec);

extern void
timer_tick(void);

extern void
register_timer_handler(void (*handler)(void));


#endif
