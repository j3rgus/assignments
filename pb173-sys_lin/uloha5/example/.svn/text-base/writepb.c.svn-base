#include <stdio.h>

#include "debug.h"
#include "pblog.h"

int main(void)
{
	int ret;

	ret = sl_log("Test", "This is testing message");
	printerr_ex((ret == -1), "sl_log", 1);
	printf("Written message into pblog.\n");

	return 0;
}
