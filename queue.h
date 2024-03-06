#ifndef QUEUE_HEADER_GUARD
#define QUEUE_HEADER_GUARD

struct process
{
	int process_id;
	int arrival_time;
	int priority;
	int time_to_live;
	int Nmegabytes;
	int Nprinters;
	int Nscanners;
	int Nmodems;
	int Ncds;
};

struct node
{
	struct process* data;
	struct node* next;
};

struct queue
{ 
	struct node* head;
	struct node* tail;
	int quantum; //optional
};

void Qenqueue(struct queue *queue, struct node *new_node);

struct node* Qdequeue(struct queue *queue);

bool Qisempty(struct queue *queue);

#endif
