/**
 * API for PB173 homework (lesson 7)
 * Queues library
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct elem {
	uint16_t id;
	void *data;
	struct elem *next;
} ELEM;

typedef struct queue {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	ELEM *first_elem;
	ELEM *actual_elem;
} QUEUE;

/**
 * @brief Initialize library
 *
 * @param[in] count Number of queues to be used.
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int q_init(uint16_t count);

/**
 * @brief Destroy all internal structures
 */
void q_destroy(void);

/**
 * @brief Enqueue data into the specified queue
 *
 * @param[in] queue Queue ID where the item will be enqueued. (IDs start from 0)
 * @param[in] id ID of the item.
 * @param[in] data Pointer to the data to be stored.
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int q_enqueue(uint16_t queue, uint16_t id, void* data);

/**
 * @brief Get the data from the specified queue. If no data available, function
 * blocks until an item is enqueued into the queue.
 *
 * @param[in] queue Queue ID where the item will be dequeued. (IDs start from 0)
 * @param[out] data Data of the dequeued item.
 * @return Id of the dequeued data, -1 on error
 */
int32_t q_dequeue(uint16_t queue, void** data);

#endif /* QUEUE_H_ */
