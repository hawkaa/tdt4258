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
sigaction(SIGUSR1, &signal_action, NULL);
