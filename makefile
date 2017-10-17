all:
	gcc -g -Wall -Wextra -O3 -o test launcher.c class_heap.c loader.c execute_engine.c 
