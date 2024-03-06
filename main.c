#include "main.h"
#include "queue.h"
#include "loader.h"

struct queue job_dispatch_list;
struct queue RT_queue; //real-time processes
struct queue UJ_queue; //user job queue
struct queue P1_queue; //priority 1
struct queue P2_queue; //priority 2
struct queue P3_queue; //priority 3

int main() {
	//load job dispatch list from jobs.txt
	load_queue_from_file(&job_dispatch_list);
}
