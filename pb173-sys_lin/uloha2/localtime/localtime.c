#include <time.h>

struct tm my_time = {
.tm_sec = 50,         /* seconds */
.tm_min = 28,         /* minutes */
.tm_hour = 3        /* hours */
};

struct tm *localtime(const time_t *timep)
{
	return &my_time;
}
