#include "main.h"
#include "loader.h"


void load_queue_from_file(struct queue *job_queue) {
	
	printf("loading...\n");
	
	//load from jobs.txt
	
	/* 
	 * Jobs.txt format:
	 * ============================================================
	 * processID	timetolive	type		priority
	 * INT		INT		0 -> RT		INT
	 * [0, inf)	[0,inf)		1 -> UJ		[0,3]
	 *
	 * example:
	 * 1 2 1 2
	 * 2 5 1 1
	 * 3 1 0 0
	 * 4 4 0 0
	 * 5 2 1 1
	 *
	 */

	FILE *fp = fopen(FILENAME, "r");

	if (fp == NULL) {
		printf("null file\n");
		return;
	}
	else {
		printf("file loaded\n");
	}
	
	fclose(fp);
}
