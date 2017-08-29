#include "pblog.h"

int sl_log(const char *caller, const char* message)
{
	int pipe;
	time_t r_time;
	char *str_time;

	time(&r_time);
	str_time = ctime(&r_time);

	pipe = open(PB_PIPE, O_WRONLY);
	if (pipe == -1)
		return 1;

	write(pipe, str_time, strlen(str_time));
	write(pipe, " ", 1);
	write(pipe, caller, strlen(caller));
	write(pipe, " ", 1);
	write(pipe, message, strlen(message));
	write(pipe, "\n", 1);

	close(pipe);
	return 0;
}
