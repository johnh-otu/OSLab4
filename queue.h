#ifndef QUEUE_HEADER_GUARD
#define QUEUE_HEADER_GUARD

struct process
{
	int process_id;
	int time_to_live;
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
};

#endif
