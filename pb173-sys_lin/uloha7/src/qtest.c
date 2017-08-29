#include <stdio.h>
#include <unistd.h>
#include "queue.h"

#define QNUM	5
#define NELEM	5

void *consumer(void *data)
{
	char *str;
	int i, id;
	int qid = (int) data;

	for (i = 0; i < NELEM; i++) {
		id = q_dequeue(qid, (void **) &str);
		printf("Queue:%d, Id:%d, Data:%s\n", qid, id, str);
	}

	pthread_exit(NULL);
}

int main(void)
{
	char *str = "ahoj";
	pthread_t threads[QNUM];
	int i, j;

	q_init(QNUM);

	for (i = 0; i < QNUM; i++)
		pthread_create(&threads[i], NULL, consumer, (int *) i);

	for (i = 0; i < NELEM; i++) {
		for (j = 0; j < QNUM; j++)
			q_enqueue(j, i, (void *) str);
	}

	for (i = 0; i < QNUM; i++)
		pthread_join(threads[i], NULL);

	q_destroy();
	return 0;
}
