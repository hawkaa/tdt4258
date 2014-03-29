struct itimerval val;
val.it_value.tv_usec = usec;
val.it_value.tv_sec = 0;
val.it_interval.tv_usec = usec;
val.it_interval.tv_sec = 0;
setitimer(ITIMER_REAL, &val, NULL);
