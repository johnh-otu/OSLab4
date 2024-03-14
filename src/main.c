#include "main.h"
#include "data_structures/queue.h"
#include "loader/loader.h"
#include "schedulers/LTscheduler.h"
#include "schedulers/MTscheduler.h"

struct queue job_dispatch_list;
struct queue RT_queue; //real-time processes
struct queue UJ_queue; //user job queue
struct queue P1_queue; //priority 1
struct queue P2_queue; //priority 2
struct queue P3_queue; //priority 3

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t loading_finished = PTHREAD_COND_INITIALIZER;

pthread_mutex_t jdl_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t RT_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t UJ_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P1_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P2_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P3_lock = PTHREAD_MUTEX_INITIALIZER;

int main() {
	//initialize queue locks
	job_dispatch_list.lock = &jdl_lock;
	RT_queue.lock = &RT_lock;
	UJ_queue.lock = &UJ_lock;
	P1_queue.lock = &P1_lock;
	P2_queue.lock = &P2_lock;
	P3_queue.lock = &P3_lock;

	//declare thread ids
	pthread_t tid_loader, tid_LTscheduler, tid_MTscheduler;
	
	//declare/init max_load_time
	int max_load_time = 0; //max arrival time value, dynamically set by loader after file is read

	//prep loader thread
	struct loader_thread_data* loader_data = malloc(sizeof(struct loader_thread_data));
	loader_data->job_queue = &job_dispatch_list;
	loader_data->lock = &lock;
	loader_data->condition = &loading_finished;
	loader_data->max_load_time = &max_load_time;

	//prep long-term scheduler thread
	struct LTscheduler_thread_data* LTscheduler_data = malloc(sizeof(struct LTscheduler_thread_data));
	LTscheduler_data->job_queue = &job_dispatch_list;
	LTscheduler_data->RT_queue = &RT_queue;
	LTscheduler_data->UJ_queue = &UJ_queue;
	LTscheduler_data->lock = &lock;
	LTscheduler_data->condition = &loading_finished;
	LTscheduler_data->max_load_time = &max_load_time;

	//prep medium-term scheduler thread
	struct MTscheduler_thread_data* MTscheduler_data = malloc(sizeof(struct MTscheduler_thread_data));
	MTscheduler_data->UJ_queue = &UJ_queue;
	MTscheduler_data->P1_queue = &P1_queue;
	MTscheduler_data->P2_queue = &P2_queue;
	MTscheduler_data->P3_queue = &P3_queue;

	//create threads
	pthread_create(&tid_LTscheduler, NULL, LTscheduler, (void *)LTscheduler_data);
	sleep(1);
	pthread_create(&tid_loader, NULL, load_queue_from_file, (void *)loader_data);
	pthread_create(&tid_MTscheduler, NULL, MTscheduler, (void *)MTscheduler_data);

	//join threads
	pthread_join(tid_loader, NULL);
	pthread_join(tid_LTscheduler, NULL);
	pthread_join(tid_MTscheduler, NULL);

	//free pointers
	free(loader_data);
	free(LTscheduler_data);
	free(MTscheduler_data);
}