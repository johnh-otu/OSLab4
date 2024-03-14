#ifndef LOADER_HEADER_GUARD
#define LOADER_HEADER_GUARD

#include "main.h"
#include "queue.h"

struct loader_thread_data {
	struct queue* job_queue;
	pthread_mutex_t* lock;
	pthread_cond_t* condition;
	int* max_load_time;
};

void* load_queue_from_file(void *args);

#endif
