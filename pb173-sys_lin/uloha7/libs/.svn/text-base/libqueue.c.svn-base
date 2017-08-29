#include "queue.h"

static QUEUE **queues;
static uint32_t number;

int q_init(uint16_t count)
{
	size_t i;

	if ((count == 0) || (queues != NULL))
		return EXIT_FAILURE;

	queues = (QUEUE **) malloc(sizeof(QUEUE *) * count);
	if (queues == NULL)
		return EXIT_FAILURE;

	for (i = 0; i < count; i++) {
		queues[i] = (QUEUE *) malloc(sizeof(QUEUE));
		if (queues[i] == NULL)
			return EXIT_FAILURE;

		queues[i]->first_elem = NULL;
		queues[i]->actual_elem = NULL;
		pthread_mutex_init(&queues[i]->mutex, NULL);
		pthread_cond_init(&queues[i]->cond, NULL);
	}

	number = count;

	return EXIT_SUCCESS;
}

void q_destroy(void)
{
	size_t i;
	ELEM *aux_elem;

	if (queues == NULL)
		return;

	for (i = 0; i < number; i++) {
		while (queues[i]->first_elem != NULL) {
			aux_elem = queues[i]->first_elem->next;
			free(queues[i]->first_elem);
			queues[i]->first_elem = aux_elem;
		}
		pthread_mutex_destroy(&queues[i]->mutex);
		pthread_cond_destroy(&queues[i]->cond);
		free(queues[i]);
	}

	free(queues);
	queues = NULL;
}

int q_enqueue(uint16_t queue, uint16_t id, void *data)
{
	ELEM *new;

	if (queue >= number)
		return EXIT_FAILURE;

	new = (ELEM *) malloc(sizeof(ELEM));
	if (new == NULL)
		return EXIT_FAILURE;

	new->id = id;
	new->data = data;
	new->next = NULL;

	pthread_mutex_lock(&queues[queue]->mutex);
	if (queues[queue]->first_elem == NULL) {
		queues[queue]->first_elem = queues[queue]->actual_elem = new;
	} else {
		queues[queue]->actual_elem->next = new;
		queues[queue]->actual_elem = new;
	}
	pthread_cond_signal(&queues[queue]->cond);
	pthread_mutex_unlock(&queues[queue]->mutex);

	return EXIT_SUCCESS;
}

int32_t q_dequeue(uint16_t queue, void **data)
{
	ELEM *aux;
	int32_t id;

	if (queue >= number)
		return -1;

	pthread_mutex_lock(&queues[queue]->mutex);
	while (queues[queue]->first_elem == NULL)
		pthread_cond_wait(&queues[queue]->cond, &queues[queue]->mutex);

	id = queues[queue]->first_elem->id;
	if ((data != NULL) && (*data != NULL))
		*data = queues[queue]->first_elem->data;

	aux = queues[queue]->first_elem->next;
	free(queues[queue]->first_elem);
	queues[queue]->first_elem = aux;
	pthread_mutex_unlock(&queues[queue]->mutex);

	return id;
}
