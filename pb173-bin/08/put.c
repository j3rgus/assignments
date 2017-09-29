#include <stdio.h>

int main(void)
{
	puts("Ahoj");
	asm volatile("int3");
	puts("svet");
	return 0;
}
