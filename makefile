compile: main.c stress.s
	gcc main.c stress.s -pthread
