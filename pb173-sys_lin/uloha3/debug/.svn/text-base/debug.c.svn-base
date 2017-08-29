#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int debug_level;
int syslog_level;

#define DEBUG_MSG(level, format, args...) \
	do {	\
		if (debug_level >= level)	\
			fprintf(stderr, "%s:%d : "format"\n", __FILE__, __LINE__, ##args);	\
		if (syslog_level >= level)	\
			syslog(LOG_DEBUG, "%s:%d : "format"\n", __FILE__, __LINE__, ##args);	\
	} while (0)

int main(int argc, char *argv[])
{
	openlog(argv[0], LOG_CONS, LOG_USER);

	if (argc > 2) {
		debug_level = atoi(argv[1]);
		syslog_level = atoi(argv[1]);
	} else
		debug_level = -1;

	DEBUG_MSG(1, "This is a debug message!");

	closelog();
	return 0;
}
