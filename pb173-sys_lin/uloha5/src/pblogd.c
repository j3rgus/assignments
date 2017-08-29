#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include "debug.h"

#define PB_PIPE		"/tmp/pb173_syslog"
#define PIPE_LEN	1000

volatile int state;

void handle_sigs(int sig)
{
	if (sig == SIGIO)
		state = 1;
	if (sig == SIGALRM)
		state = 2;
	if (sig == SIGTERM)
		state = 3;
}

void usage(void)
{
	printf("Usage: ./pblogd -f <filename> -i <interval>\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	char buf[PIPE_LEN];
	char *filename = NULL;
	int interval = 0;
	long count;
	int fd, pipe;
	int opt, ret, size;
	struct sigaction sa;

	while (1) {
		opt = getopt(argc, argv, "i:f:");
		if (opt == -1)
			break;

		switch (opt) {
		case 'i':
			interval = atoi(optarg);
			break;
		case 'f':
			filename = optarg;
			break;
		default:
			usage();
		}
	}

	if ((interval == 0) || (filename == NULL))
		usage();

	umask(0);

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	printerr_ex((fd == -1), filename, 1);

	clear_errno;
	ret = mkfifo(PB_PIPE, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
	printerr_ex((ret == -1) & !(errno & EEXIST), "mkfifo", 1);

	sa.sa_handler = handle_sigs;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	ret = sigaction(SIGALRM, &sa, NULL);
	printerr_ex((ret == -1), "sigaction (SIGALRM)", 1);

	ret = sigaction(SIGTERM, &sa, NULL);
	printerr_ex((ret == -1), "sigaction (SIGTERM)", 1);

	ret = sigaction(SIGIO, &sa, NULL);
	printerr_ex((ret == -1), "sigaction (SIGIO)", 1);

	openlog("pblogd", LOG_CONS, LOG_USER);

	ret = daemon(0, 0);
	printerr_ex((ret == -1), "daemon", 1);

	pipe = open(PB_PIPE, O_NONBLOCK | O_RDONLY);
	fcntl(pipe, F_SETOWN, getpid());
	fcntl(pipe, F_SETFL, O_ASYNC);

	count = 0;
	alarm(interval);
	while (1) {
		pause();
		if (state == 3)
			break;
		if (state == 1) {
			size = read(pipe, buf, PIPE_LEN - 1);
			write(fd, buf, size);
			count++;
		}
		if (state == 2) {
			syslog(LOG_INFO, "Number of messages: %ld", count);
			alarm(interval);
		}
		state = 0;
	}

	close(pipe);
	close(fd);
	closelog();

	return 0;
}
