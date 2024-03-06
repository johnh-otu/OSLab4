#include "main.h"
#include "queue.h"

void Qenqueue(struct queue *queue, struct node *new_node)
{
	queue->tail->next = new_node;
	queue->tail = new_node;
	if (Qisempty(queue))
		queue->head = new_node;
}

struct node* Qdequeue(struct queue *queue)
{
	if (Qisempty(queue))
		return NULL;
	
	struct node *output = queue->head;
	queue->head = queue->head->next;
	
	if (Qisempty(queue))
		queue->tail = NULL;
	
	return output;
}

bool Qisempty(struct queue *queue)
{
	return (queue->head == NULL);
}


