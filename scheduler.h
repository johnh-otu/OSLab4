#ifndef SCHEDULER_HEADER_GUARD
#define SCHEDULER_HEADER_GUARD

#include "main.h"
#include "queue.h"

struct scheduler_thread_data {
	struct queue* job_queue; //job dispatch list
	struct queue* RT_queue; //real-time processes
	struct queue* UJ_queue; //user job queue
	struct queue* P1_queue;
	struct queue* P2_queue;
	struct queue* P3_queue;
	pthread_mutex_t* lock;
	pthread_cond_t* condition;
	int* max_load_time;
};

void *scheduler(void* args);

#endif
