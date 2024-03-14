#include "main.h"
#include "queue.h"

void Qenqueue(struct queue *queue, struct node *new_node)
{
	pthread_mutex_lock(queue->lock); //take lock
	
	if (queue->head == NULL) { //if queue is empty
		queue->head = new_node;
		queue->tail = new_node; //set node to head and tail
	}
	else {	
		queue->tail->next = new_node; //add node to tail
		queue->tail = new_node;
	}

	pthread_mutex_unlock(queue->lock); //release lock
}

struct node* Qdequeue(struct queue *queue)
{
	pthread_mutex_lock(queue->lock); //take lock
	
	if (queue->head == NULL) //if queue is empty
	{
		pthread_mutex_unlock(queue->lock);
		return NULL;
	}

	struct node *output = queue->head; //get head
	queue->head = queue->head->next; //set head to next node
	
	if (queue->head == NULL) //if queue is empty
		queue->tail = NULL; //reset tail
	
	pthread_mutex_unlock(queue->lock); //release lock
	return output;
}

bool Qisempty(struct queue *queue)
{
	pthread_mutex_lock(queue->lock); //take lock
	bool output = (queue->head == NULL);
	pthread_mutex_unlock(queue->lock); //release lock
	return output;
}

void Qwalk(struct queue *queue)
{
	pthread_mutex_lock(queue->lock); //take lock
	
	if (queue->head == NULL) {
		printf("Q:queue empty\n");
		pthread_mutex_unlock(queue->lock);
		return;
	}

	struct node* temp = queue->head;

	printf("Q:");
	while(temp != NULL) {
		printf(" %d", temp->data->process_id);
		temp = temp->next;
	}
	printf("\n");
	pthread_mutex_unlock(queue->lock); //release lock
	return;
}

void Qheadinfo(struct queue *queue)
{
	pthread_mutex_lock(queue->lock);
	if (queue->head == NULL)
		printf("Q: queue empty\n");
	else if (queue->tail == NULL)
		printf("Q!!!: head->%d, tail->NULL\n", queue->head->data->process_id);
	else if (queue->head->next == NULL)
		printf("Q: head->%d, tail->%d, head->next->NULL\n", queue->head->data->process_id, queue->tail->data->process_id);
	else
		printf("Q: head->%d, tail->%d, head->next->%d\n", queue->head->data->process_id, queue->tail->data->process_id, queue->head->next->data->process_id);

	pthread_mutex_unlock(queue->lock);
	return;
}
