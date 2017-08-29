#include <stdio.h>
#include "debug.h"

int main(void)
{
	setdebug(2);
	print_debug(2, "Test\n");
	printerr_ex((1 == 1), "1==1", 0);
	printf("Ahoj\n");
	return 0;
}
