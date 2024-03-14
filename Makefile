dispatcher: main.c loader.c queue.c heap.c LTscheduler.c MTscheduler.c dispatcher.c
	gcc main.c loader.c queue.c heap.c LTscheduler.c MTscheduler.c -Wall -o HOST.exe
	gcc -Wall -std=gnu99 dispatcher.c -o dispatcher.exe
debug: main.c loader.c queue.c heap.c LTscheduler.c MTscheduler.c dispatcher.c
	gcc main.c loader.c queue.c heap.c LTscheduler.c MTscheduler.c -Wall -Wextra -g -o HOST.exe
	gcc -Wall -Wextra -std=gnu99 dispatcher.c -g -o dispatcher.exe
test: testscript.c queue.c
	gcc testscript.c queue.c -Wall -g -o test
