/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* local includes */
#include "timer.h"


static void (*timer_handler)(void);

int
timer_init(long int usec)
{
	

	struct itimerval val;
	val.it_value.tv_usec = usec;
	val.it_value.tv_sec = 0;
	val.it_interval.tv_usec = usec;
	val.it_interval.tv_sec = 0;

	return setitimer(ITIMER_REAL, &val, NULL);
}

void
timer_tick(void)
{
	printf("Timer tick!\n");
	if (timer_handler != NULL) {
		(*timer_handler)();
	}
}

void
register_timer_handler(void (*handler)(void))
{
	timer_handler = handler;
}
