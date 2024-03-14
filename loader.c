#include "main.h"
#include "loader.h"
#include "queue.h"
#include "heap.h"

struct heap process_heap;

void *load_queue_from_file(void *args) {
	
	printf("loader called\n");	
	struct queue* job_queue = ((struct loader_thread_data*)args)->job_queue;
	pthread_mutex_t *lock = ((struct loader_thread_data*)args)->lock;
	pthread_cond_t *loading_finished = ((struct loader_thread_data*)args)->condition;
	int *max_load_time = ((struct loader_thread_data*)args)->max_load_time;

	printf("job queue loaded successfully: %p\n", job_queue);

	pthread_mutex_lock(lock); //take lock once released
	printf("lock taken, loading can start\n");
	printf("loading...\n");
	
	//load from jobs.txt
	
	/* 
	 * Jobs.txt format:
	 * ===================================================================================================================================
	 * <arrival time>,	<priority>,	<processor time>,	<Mbytes>,	<#printers>,	<#scanners>,	<#modems>,	<#CDs>
	 * INT			INT		INT			INT		INT		INT		INT		INT
	 * [0, inf)		[0, 3]		[0, inf)		[0, 1024]	[0, 2]		[0, 1]		[0, 1]		[0, 2]
	 *
	 * examples:
	 * 12,0,1,64,0,0,0,0
	 * 12,1,2,128,1,0,0,1
	 * 13,3,6,128,1,0,1,2
	 *
	 */

	FILE *fp = fopen(FILENAME, "r");

	if (fp == NULL) {
		printf("null file\n");
		return NULL;
	}
	else {
		printf("file loaded\n");
	}
	
	//Get process info from file and store in N buckets, where N is the max arrival time for all processes in jobs.txt
	//Each process is stored in a bucket according to their arrival time

	struct process temp_process;
	Hinit(&process_heap);

	int id_iterator = 1;

	while(1) {
		temp_process.process_id = id_iterator++;
		fscanf(fp, "%d,%d,%d,%d,%d,%d,%d,%d ", &temp_process.arrival_time, &temp_process.priority, &temp_process.time_to_live, &temp_process.Nmegabytes, &temp_process.Nprinters, &temp_process.Nscanners, &temp_process.Nmodems, &temp_process.Ncds);

		Hinsert(&process_heap, &temp_process);
		//Hprint(&process_heap);

		if (feof(fp)) //if end of file, finished reading
			break;

	}
	
	fclose(fp);
	
	int total_processes = process_heap.size;
	Hsort(&process_heap);
	Hprint(&process_heap);
	
	printf("file read and closed\n");
	pthread_cond_signal(loading_finished); //tell scheduler loading is finished
	pthread_mutex_unlock(lock);
	printf("condition signal sent, loading ended\n");

	//fork and use child
	//child:
	//	after each time interval,
	//	enqueue all processes where process->arrival_time == time interval;

	int i = 0;
	*max_load_time = process_heap.head[total_processes - 1].arrival_time;
	
	for (int t = 0; t < *max_load_time + 1; t++)
	{
		printf("L%d:", t);
		while(process_heap.head[i].arrival_time == t)
		{
			printf(" %d", process_heap.head[i].process_id);
			struct node* temp = malloc(sizeof(struct node));
			temp->data = &process_heap.head[i];
			temp->next = NULL;

			Qenqueue(job_queue,temp); 
			
			i++;
			if (i >= total_processes)
				break;
		}
		printf("\n");
		Qheadinfo(job_queue);
		if (i >= total_processes)
			break;

		sleep(1); //simulate tick
	}

	printf("loader finished, waiting...\n");
	pthread_mutex_lock(lock);
	pthread_cond_wait(loading_finished, lock);
	printf("condition met, goodbye!\n");
	return NULL;
}
