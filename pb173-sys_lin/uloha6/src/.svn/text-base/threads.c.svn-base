#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "debug.h"

#define NUM 5

void *identify(void *nothing)
{
	(void) nothing;
	printf("I am %d\n", (unsigned) syscall(SYS_gettid));
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t threads[5];
	int ret, i;

	for (i = 0; i < NUM; i++) {
		ret = pthread_create(&threads[i], NULL, identify, NULL);
		printerr((ret != 0), "pthread_create");
	}

	for (i = 0; i < NUM; i++)
		pthread_join(threads[i], NULL);

	return 1;
}
