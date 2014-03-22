/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* local includes */
#include "signal.h"

static void
signal_handler(int signum)
{
	printf("Received signal(%i)\n", signum);
}

int
signal_handler_init()
{
	struct sigaction signal_action;
	sigemptyset(&signal_action);
	signal_action.sa_flags = 0;
	int retval = sigaction(SIGUSR1, &signal_action, NULL);
	return retval;
}



