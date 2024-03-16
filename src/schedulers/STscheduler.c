#include "../main.h"
#include "STscheduler.h"
#include "../data_structures/queue.h"

int mem[TOTALMEMORYMB] = {0};

struct queue* ST_RT_queue;
struct queue* ST_UJ_queue;
struct queue* ST_P1_queue;
struct queue* ST_P2_queue;
struct queue* ST_P3_queue;

void *STscheduler(void* args)
{
	ST_RT_queue = ((struct STscheduler_thread_data*)args)->RT_queue;
	ST_UJ_queue = ((struct STscheduler_thread_data*)args)->UJ_queue;
	ST_P1_queue = ((struct STscheduler_thread_data*)args)->P1_queue;
	ST_P2_queue = ((struct STscheduler_thread_data*)args)->P2_queue;
	ST_P3_queue = ((struct STscheduler_thread_data*)args)->P3_queue;

	printf("short-term scheduler called\n");

	struct node* current_proc_node;

	while (true) {
		
		//check for RT processes and run
		if(emptyRTqueue(current_proc_node, 0))
			continue;

		//check for P1 processes and run if able
		if (!Qisempty(ST_P1_queue))
		{
			//note: P1 jobs will never preempt other jobs
			current_proc_node = Qdequeue(ST_P1_queue);

			if (!current_proc_node->data->suspended && !allocateMemoryAndResources(current_proc_node->data)) { //if new but no room
				Qenqueue(ST_P1_queue, current_proc_node);
				current_proc_node = NULL;
				continue;
			}
			else {
				executeProcess(current_proc_node->data);
				
				for (int t = 0; t < QUANTUM1 || t == current_proc_node->data->time_to_live; t++) 
				{
					sleep(1); //simulate tick;
					emptyRTqueue(current_proc_node, t);
				}

				if (current_proc_node->data->time_to_live <= QUANTUM1) { //end ofprocess				    
					kill(current_proc_node->data->process_id, SIGINT);
					int status;
					waitpid(current_proc_node->data->process_id, &status, 0);
					if (WIFEXITED(status) || WIFSIGNALED(status)) 
						printf("Process %d terminated\n", current_proc_node->data->process_id);
					freeMemoryAndResources(current_proc_node->data);
				}
				else
				{
					kill(current_proc_node->data->process_id, SIGTSTP);
					current_proc_node->data->priority++;
					current_proc_node->data->time_to_live = current_proc_node->data->time_to_live - QUANTUM1;
					Qenqueue(ST_UJ_queue, current_proc_node);
					current_proc_node = NULL;
				}

				continue;

			}
		}

		//check for P2 processes and run if able
		if (!Qisempty(ST_P2_queue))
		{
			//note: P2 jobs will never preempt other jobs
			current_proc_node = Qdequeue(ST_P2_queue);
			if (!current_proc_node->data->suspended && !allocateMemoryAndResources(current_proc_node->data)) { //if new but no room
				Qenqueue(ST_P2_queue, current_proc_node);
				current_proc_node = NULL;
				continue;
			}
			else {
				executeProcess(current_proc_node->data);
				
				for (int t = 0; t < QUANTUM2 || t == current_proc_node->data->time_to_live; t++) 
				{
					sleep(1); //simulate tick;
					emptyRTqueue(current_proc_node, t);
				}

				if (current_proc_node->data->time_to_live <= QUANTUM2) { //end ofprocess				    
					kill(current_proc_node->data->process_id, SIGINT);
					int status;
					waitpid(current_proc_node->data->process_id, &status, 0);
					if (WIFEXITED(status) || WIFSIGNALED(status)) 
						printf("Process %d terminated\n", current_proc_node->data->process_id);
					freeMemoryAndResources(current_proc_node->data);
				}
				else
				{
					kill(current_proc_node->data->process_id, SIGTSTP);
					current_proc_node->data->priority++;
					current_proc_node->data->time_to_live = current_proc_node->data->time_to_live - QUANTUM2;
					Qenqueue(ST_UJ_queue, current_proc_node);
					current_proc_node = NULL;
				}

				continue;

			}
		}

		//check for P3 processes and run if able
		if (!Qisempty(ST_P3_queue))
		{
			//note: P3 jobs will never preempt other jobs
			current_proc_node = Qdequeue(ST_P3_queue);
			if (!current_proc_node->data->suspended && !allocateMemoryAndResources(current_proc_node->data)) { //if new but no room
				Qenqueue(ST_P3_queue, current_proc_node);
				current_proc_node = NULL;
				continue;
			}
			else {
				executeProcess(current_proc_node->data);
				
				for (int t = 0; t < 1; t++) 
				{
					sleep(1); //simulate tick;
					emptyRTqueue(current_proc_node, t);
					if (current_proc_node->data->time_to_live == 1)
						break;
				}

				if (current_proc_node->data->time_to_live == 1) { //end ofprocess				    
					kill(current_proc_node->data->process_id, SIGINT);
					int status;
					waitpid(current_proc_node->data->process_id, &status, 0);
					if (WIFEXITED(status) || WIFSIGNALED(status)) 
						printf("Process %d terminated\n", current_proc_node->data->process_id);
					freeMemoryAndResources(current_proc_node->data);
				}
				else
				{
					kill(current_proc_node->data->process_id, SIGTSTP);
					current_proc_node->data->priority++;
					current_proc_node->data->time_to_live = current_proc_node->data->time_to_live - 1;
					Qenqueue(ST_UJ_queue, current_proc_node);
					current_proc_node = NULL;
				}

				continue;

			}
		} 		
	}	
	
	printf("NO MORE PROCESSES (STS)\n");	
	return NULL;
}

void executeProcess(struct process* proc)
{
	if(proc->suspended) {
		kill(proc->process_id, SIGCONT);
	}
	else {
		int pid = fork();

		if (pid < 0)
			printf("Error: couldn't fork\n");
		else if (pid == 0) {
			//child process
			char *time_to_live = malloc(5 * sizeof(char));
			snprintf(time_to_live, 5*sizeof(char), "%d", proc->time_to_live); //convert to string
			char *args[] = {"./dispatcher.exe", time_to_live, NULL};
			int statuscode = execvp(args[0], args); //simulate process
			if (statuscode == -1)
				printf("EXITED INCORRECTLY");
		}
		else {
			proc->process_id = pid;
		}
	}
}

bool emptyRTqueue(struct node* proc_node, int t)
{
	struct node* new_proc_node;

	if (!Qisempty(ST_RT_queue))
	{
		if (proc_node != NULL) { //swap with current
			new_proc_node = Qdequeue(ST_RT_queue);
			kill(proc_node->data->process_id, SIGTSTP);
			proc_node->data->priority--;
			proc_node->data->time_to_live = proc_node->data->time_to_live - t - 1;
			Qenqueue(ST_UJ_queue, proc_node);
			proc_node = new_proc_node;
		}
		else { //pop
			proc_node = Qdequeue(ST_RT_queue);
		}
		
		allocateMemoryAndResources(proc_node->data);
		executeProcess(proc_node->data);
		waitpid(proc_node->data->process_id, NULL, 0);
		printf("Process %d terminated\n", proc_node->data->process_id);
		freeMemoryAndResources(proc_node->data);
		return true;
	}
	
	return false;
}

bool allocateMemoryAndResources(struct process* proc)
{
	return true;
}

void freeMemoryAndResources(struct process* proc)
{
	//free(proc);
}


