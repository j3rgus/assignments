#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct job {
	struct job* next;
	int jid;
};

/* list of pending jobs */
struct job* job_queue;

void *producer(void *tid)
{
	int i;
	struct job* aux_job;

	for (i = 0; i < 100; i++) {
		pthread_mutex_lock(&mutex);
		aux_job = job_queue;
		job_queue = (struct job*) malloc (sizeof (struct job));
		job_queue->jid = i;
		job_queue->next = aux_job;
		pthread_mutex_unlock(&mutex);
		sem_post(&sem);
	}
	return NULL;
}

void*
thread_func (void *tid)
{
	struct job* my_job;

	while (1) {
		sem_wait(&sem);
		pthread_mutex_lock(&mutex);
		my_job = job_queue;
		
		/* sleep to demonstrate race condition */
		//usleep(rand() % 1000000);

		job_queue = job_queue->next;
		pthread_mutex_unlock(&mutex);

		/* do the job */
		printf("%d by thread %d\n", my_job->jid, *(int*)tid);
		/* clean up */
		free(my_job);
	}
	return NULL;
}

int
main()
{
	pthread_t my_threads[3];
	
	int tid2 = 2, tid3 = 3;

	sem_init(&sem, 0, 0);

	pthread_create(&my_threads[0], NULL, producer, NULL);
	pthread_create(&my_threads[1], NULL, thread_func, &tid2);
	pthread_create(&my_threads[2], NULL, thread_func, &tid3);

	pthread_join(my_threads[0], NULL);
	pthread_join(my_threads[1], NULL);
	pthread_join(my_threads[2], NULL);

	sem_destroy(&sem);
	return 0;
}
