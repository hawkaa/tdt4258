static void signal_user_application(void)
{
	...

	info.si_signo = SIGUSR1;
	info.si_code = SI_QUEUE;
	t = pid_task(find_pid_ns(pid, &init_pid_ns), PIDTYPE_PID);
	retval = send_sig_info(SIGUSR1, &info, t); 
	
	...
}
