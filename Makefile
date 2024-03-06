dispatcher: main.c loader.c queue.c heap.c
	gcc main.c loader.c queue.c heap.c -Wall -o dispatcher.exe
debug: main.c loader.c queue.c heap.c
	gcc main.c loader.c queue.c heap.c -Wall -g -o dispatcher.exe
