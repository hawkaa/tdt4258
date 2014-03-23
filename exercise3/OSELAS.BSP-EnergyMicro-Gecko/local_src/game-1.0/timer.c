/*
 * TIMER MODULE
 *
 * Will enable a timer and store the timer tick handler
 */

/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* local includes */
#include "timer.h"

/* timer handler */
static void (*timer_handler)(void);

/*
 * Initiate timer with the current interval.
 * watch out for usec overflow
 */
int
timer_init(long int usec)
{
	/* timer value */
	struct itimerval val;
	val.it_value.tv_usec = usec;
	val.it_value.tv_sec = 0;
	val.it_interval.tv_usec = usec;
	val.it_interval.tv_sec = 0;

	/* set timer */
	return setitimer(ITIMER_REAL, &val, NULL);
}

/*
 * Will be called on each SIGALRM
 */
void
sigalrm_timer_tick(void)
{
	if (timer_handler != NULL) {
		(*timer_handler)();
	}
}

/*
 * Register timer handler
 */
void
register_timer_handler(void (*handler)(void))
{
	timer_handler = handler;
}
