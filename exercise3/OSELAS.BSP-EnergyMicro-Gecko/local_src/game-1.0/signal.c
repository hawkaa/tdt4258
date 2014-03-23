/*
 * SIGNAL MODULE
 *
 * Will set up signals for driver and timer interraction
 */

/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* local includes */
#include "signal.h"
#include "input.h"
#include "timer.h"

/*
 * Alarm signal handler
 * Will pass the control off to timer.c
 */
static void
sigalrm_handler(int signum)
{
	sigalrm_timer_tick();
}

/*
 * User defined signal handler (driver signals that new input is available)
 * Will pass the signal over to input.c
 */
static void
sigusr1_handler(int signum)
{
	process_input();
}

/*
 * Timer signal init function
 * Sets the SIGALRM handler
 */
int
timer_signal_init(void)
{

	struct sigaction signal_action;

	/* add handler */
	signal_action.sa_handler = sigalrm_handler;
	
	/* block alarm and user defined signals */
	sigemptyset(&signal_action.sa_mask);
	sigaddset(&signal_action.sa_mask, SIGUSR1);
	sigaddset(&signal_action.sa_mask, SIGALRM);

	/* flags, let library procedures continue after signal handling is done */
	signal_action.sa_flags = SA_RESTART;

	/* replace old with new action, not preserving the old */
	return sigaction(SIGALRM, &signal_action, NULL);
}

/*
 * Button signal init function
 * Sets the SIGUSR1 handler
 */
int
button_signal_init(void)
{
	struct sigaction signal_action;

	/* add handler */
	signal_action.sa_handler = sigusr1_handler;
	
	/* empty block mask. SIGUSR1 signals will still be blocked */
	sigemptyset(&signal_action.sa_mask);
	sigaddset(&signal_action.sa_mask, SIGUSR1);
	sigaddset(&signal_action.sa_mask, SIGALRM);

	/* flags, let library procedures continue after signal handling is done */
	signal_action.sa_flags = SA_RESTART;

	/* replace old with new action, not preserving the old */
	return sigaction(SIGUSR1, &signal_action, NULL);
}
