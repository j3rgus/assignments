#include <stdio.h>
#include <stdlib.h>

void end(void)
{
	printf("Koncim!\n");
}

void on_end(int stat, void *str)
{
	printf("Koncim %d %s\n", stat, (char *) str);
}

int main(void)
{
	on_exit(on_end, (void *) "Ahoj");
	atexit(end);

	return 0;
}
