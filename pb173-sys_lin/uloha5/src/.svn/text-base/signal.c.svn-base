#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>

#include "debug.h"

volatile int print;

void sig_handle(int sig)
{
	if (sig == SIGALRM)
		print = 1;
}

int main(void)
{
	int ret;
	struct sigaction sa;

	sa.sa_handler = sig_handle;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	ret = sigaction(SIGALRM, &sa, NULL);
	printerr_ex((ret == -1), "sigaction", 1);

	openlog("signal.c", LOG_CONS, LOG_USER);

	alarm(4);
	while (1) {
		if (print == 1) {
			printf("Got signal - check syslog!\n");
			syslog(LOG_DEBUG, "Got signal");
			break;
		}
	}

	closelog();

	return 0;
}
