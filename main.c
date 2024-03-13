#include "main.h"
#include "queue.h"
#include "loader.h"

struct queue job_dispatch_list;
struct queue RT_queue; //real-time processes
struct queue UJ_queue; //user job queue
struct queue P1_queue; //priority 1
struct queue P2_queue; //priority 2
struct queue P3_queue; //priority 3

pthread_mutex_t loaderlock = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t processes_loaded = PTHREAD_COND_INTIALIZER;

int main() {
	//load job dispatch list from jobs.txt
	pthread_t tid_loader;
	struct thread_data* loader_data = malloc(sizeof(struct thread_data));
	loader_data->job_queue = &job_dispatch_list;
	loader_data->lock = &loaderlock;

	pthread_create(&tid_loader, NULL, load_queue_from_file, (void *)loader_data);
	printf("thread created, now waiting...\n");
	sleep(1);
	pthread_mutex_lock(&loaderlock);
	//pthread_cond_wait(&processes_loaded, &loaderlock);

	printf("lock released, dispatcher starts\n");
	//load_queue_from_file(&job_dispatch_list);
	pthread_join(tid_loader, NULL);
}
