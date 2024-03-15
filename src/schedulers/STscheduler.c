#include "../main.h"
#include "MTscheduler.h"
#include "../data_structures/queue.h"

void *STscheduler(void* args)
{
	struct queue* RT_queue = ((struct RTscheduler_thread_data*)args)->RT_queue;
	struct queue* P1_queue = ((struct RTscheduler_thread_data*)args)->P1_queue;
	struct queue* P2_queue = ((struct RTscheduler_thread_data*)args)->P2_queue;
	struct queue* P3_queue = ((struct RTscheduler_thread_data*)args)->P3_queue;

	printf("short-term scheduler called\n");

	struct process current_proc;

	while (true) {
		
		//if RT !empty
		//	if current_proc active && current_proc.priority > 0
		//		swap
		//	else
		//		pop and execute
		//	sleep(1);
		//	signal SIGTSTP
		//	enqueue P1
		//	continue

		//else if P1 !empty
		//	if current_proc active && current_proc.priority > 1
		//		swap
		//	else
		//		pop and execute
		//	sleep(2);
		//	signal SIGTSTP
		//	enqueue P2
		//	continue
		
		//else if P2 !empty
		//	if current_proc active && current_proc.priority > 2
		//		swap
		//	else
		//		pop and execute
		//	sleep(4);
		//	signal(SIGTSTP); //Q2
		//	enqueue P3
		//	continue

		//else if no current_proc && P3 !empty
		//	pop and execute
		//	sleep(1);
		//	signal(SIGTSTP); //RR
		//	enqueue P3
		//	continue;

		//current_proc = Qdequeue(RTqueue);
	       		
	}	
	
	printf("NO MORE PROCESSES (STS)\n");	
	return NULL;
}
