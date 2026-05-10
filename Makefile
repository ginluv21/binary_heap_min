CC     = gcc
CFLAGS = -Wall -Wextra -O2

.PHONY: all experiment menu clean

all: experiment menu

experiment: heap_experiment

menu: heap_menu

# --- experiment ---

heap_experiment: experiment/main.o experiment/experiment.o min_heap/minheap.o
	$(CC) $(CFLAGS) -o heap_experiment experiment/main.o experiment/experiment.o min_heap/minheap.o

experiment/main.o: experiment/main.c experiment/experiment.h
	$(CC) $(CFLAGS) -c experiment/main.c -o experiment/main.o

experiment/experiment.o: experiment/experiment.c experiment/experiment.h min_heap/minheap.h
	$(CC) $(CFLAGS) -c experiment/experiment.c -o experiment/experiment.o

# --- menu ---

heap_menu: min_heap/menu.o min_heap/minheap.o
	$(CC) $(CFLAGS) -o heap_menu min_heap/menu.o min_heap/minheap.o

min_heap/menu.o: min_heap/menu.c min_heap/menu.h min_heap/minheap.h
	$(CC) $(CFLAGS) -Wno-unused-result -c min_heap/menu.c -o min_heap/menu.o

# --- shared ---

min_heap/minheap.o: min_heap/minheap.c min_heap/minheap.h
	$(CC) $(CFLAGS) -c min_heap/minheap.c -o min_heap/minheap.o

clean:
	rm -f experiment/*.o min_heap/*.o heap_experiment heap_menu
