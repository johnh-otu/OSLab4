#include "main.h"
#include "loader.h"
#include "queue.h"
#include "heap.h"

struct heap process_heap;

void *load_queue_from_file(void *args) {
	
	struct queue* job_queue = ((struct thread_data*)args)->job_queue;
	pthread_mutex_t *lock = ((struct thread_data*)args)->lock;
	printf("job queue loaded successfully: %p\n", job_queue);

	pthread_mutex_lock(lock);
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
	printf("file read and closed\n");
	sleep(1);
	pthread_mutex_unlock(lock);
	printf("lock released, loading ended\n");

	int total_processes = process_heap.size;
	Hsort(&process_heap);
	//Hprint(&process_heap);

	//fork and use child
	//child:
	//	after each time interval,
	//	enqueue all processes where process->arrival_time == time interval;

	int i = 0;
	for (int t = 0; t < process_heap.head[total_processes - 1].arrival_time + 1; t++)
	{
		temp_process = process_heap.head[i];
		printf("%d:", t);
		while(process_heap.head[i].arrival_time == t)
		{
			printf(" %d", process_heap.head[i].process_id);
			i++;
			if (i >= total_processes)
				break;
		}
		printf("\n");
		if (i >= total_processes)
			break;
	}
	return NULL;
}
