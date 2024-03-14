dispatcher: main.c loader.c queue.c heap.c scheduler.c
	gcc main.c loader.c queue.c heap.c scheduler.c -Wall -o dispatcher.exe
debug: main.c loader.c queue.c heap.c scheduler.c
	gcc main.c loader.c queue.c heap.c scheduler.c -Wall -g -o dispatcher.exe
test: testscript.c queue.c
	gcc testscript.c queue.c -Wall -g -o test
