/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* local includes */
#include "signal.h"
#include "input.h"

static void
signal_handler(int signum)
{
	printf("Received signal(%i)\n", signum);
	process_input();
}

int
signal_handler_init(void)
{
	struct sigaction signal_action;
	int retval;

	/* add handler */
	signal_action.sa_handler = signal_handler;
	
	/* empty block mask */
	sigemptyset(&signal_action.sa_mask);

	/* flags */
	signal_action.sa_flags = 0;

	/* replace old with new action, not preserving the old */
	retval = sigaction(SIGUSR1, &signal_action, NULL);
	return retval;
}



