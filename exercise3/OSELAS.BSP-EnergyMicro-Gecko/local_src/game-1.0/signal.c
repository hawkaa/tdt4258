/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* local includes */
#include "signal.h"

#define GAMEPAD_SIGNAL SIGUSR1

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
	return sigaction(GAMEPAD_SIGNAL, &signal_action, NULL);
}



