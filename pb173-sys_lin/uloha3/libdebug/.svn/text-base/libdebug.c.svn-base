#include "debug.h"

int dbg_lvl;
int sys_lvl;

void print_debug(int level, char *format, ...)
{
	va_list arg;

	if (dbg_lvl >= level) {
		fprintf(stderr, "%s:%d : ", __FILE__, __LINE__);
		va_start(arg, format);
		vfprintf(stderr, format, arg);
		va_end(arg);
	}
	if (sys_lvl >= level) {
		openlog("libdebug", LOG_CONS, LOG_USER);
		syslog(LOG_DEBUG, "%s:%d : ", __FILE__, __LINE__);
		va_start(arg, format);
		vsyslog(LOG_DEBUG, format, arg);
		va_end(arg);
		closelog();
	}
}

void printerr(int cond, char *msg)
{
	if (cond)
		perror(msg);
}

void printerr_ex(int cond, char *msg, int ret)
{
	if (cond) {
		perror(msg);
		exit(ret);
	}
}
