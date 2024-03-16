#include "dispatcher.h"
#include "colours.h"

char* colours[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
#define N_COLOUR 7
#define NORMAL "\033[0m"
char* current_colour;

static int signal_SIGINT = FALSE;
static int signal_SIGTSTP = FALSE;
static int signal_SIGCONT = FALSE;

static void sighandler(int);
void dispatcherPrint(pid_t pid, char* string);
void dispatcherPrintTick(pid_t pid, int tick);

/********************************************************************/
//         Based on the sigtrap.c file from labs 7 and 8            //
/********************************************************************/

int main(int argc, char* argv[]) 
{
	pid_t pid = getpid();
	int i, cycle, rc;
	long clktck = sysconf(_SC_CLK_TCK);
	struct tms t;
	clock_t starttick, stoptick;
	sigset_t mask;

	current_colour = colours[pid % N_COLOUR]; //select colour

	if (argc > 2 || (argc == 2 && !isdigit((int)argv[1][0])))
	{
		printf("usage: %s [seconds]\n", argv[0]);
		fflush(stdout);
		exit(1);
	}

	dispatcherPrint(pid, "PROCESS START");

	//connect signals to handler
	signal(SIGINT, sighandler);
	signal(SIGTSTP, sighandler);
	
	rc = setpriority(PRIO_PROCESS, 0, 20); //lower process priority
	cycle = argc < 2 ? 1 : atoi(argv[1]); //get tick count

	for (i = 0; i < cycle;) {
		
		if (signal_SIGCONT) { //if continuing from previous suspension
			signal_SIGCONT = FALSE;
			dispatcherPrint(pid, "SIGCONT");
		}

		//tick
		starttick = times (&t);
		rc = sleep(1);
		stoptick = times (&t);

		if (rc == 0 || (stoptick-starttick) > clktck/2) //print tick
			dispatcherPrintTick(pid, ++i);
		
		if (signal_SIGINT) { //if interrupt signal received
			dispatcherPrint(pid, "SIGINT");
			exit(0);
		}

		if (signal_SIGTSTP) { //if suspend signal received 
			signal_SIGTSTP = FALSE;
			dispatcherPrint(pid, "SIGTSTP");
			sigemptyset (&mask); //unblock if necessary
			sigaddset (&mask, SIGTSTP);
			sigprocmask (SIG_UNBLOCK, &mask, NULL);
			signal(SIGTSTP, SIG_DFL); //reset trap to default
			raise(SIGTSTP); //suspend
			signal(SIGTSTP, sighandler); //reset trap to handler
			signal_SIGCONT = TRUE; //set continue flag on continue
	
		}
		fflush(stdout);

	}
	exit(0);
}

/********************************************************************/

void dispatcherPrint(pid_t pid, char* str)
{
	printf("%s%7d; %s%s\n", current_colour, (int)pid, str, WHITE);
	fflush(stdout);
}
void dispatcherPrintTick(pid_t pid, int tick)
{
	printf("%s%7d; tick %d%s\n", current_colour, (int)pid, tick, WHITE);
}

/********************************************************************/

static void sighandler(int sig)
{
	switch (sig) {
		case SIGINT:
			signal_SIGINT = TRUE;
			break;
		case SIGTSTP:
			signal_SIGTSTP = TRUE;
			break;
	}
}


