#ifndef LOADER_HEADER_GUARD
#define LOADER_HEADER_GUARD

#include "main.h"
#include "queue.h"

struct thread_data {
	struct queue* job_queue;
	pthread_mutex_t* lock;
};

void* load_queue_from_file(void *args);

#endif
