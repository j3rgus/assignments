#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int ret;

	ret = system("ps aux");
	printf("ps returned %d\n", ret);
	return 0;
}
