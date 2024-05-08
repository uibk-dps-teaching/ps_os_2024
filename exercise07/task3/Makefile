CFLAGS = -std=c11 -Wall -Wextra -pedantic -O2 -g -pthread
LDFLAGS = -pthread

TARGETS = roulette.o task3a.o task3.o task3a task3

.PHONY:
all: $(TARGETS)

.PHONY:
clean:
	$(RM) $(TARGETS) *.o

task3: task3.o roulette.o
roulette.o:  roulette.h roulette.c
task3.o: task3.c roulette.h
	$(CC) $(CFLAGS) $(LDFLAGS) -DTHREAD_PER_JOB -c -o $@ $<

task3a: task3a.o roulette.o thread_pool.o
thread_pool.o: thread_pool.c thread_pool.h
task3a.o: task3.c roulette.h  thread_pool.h
	$(CC) $(CFLAGS) $(LDFLAGS) -DTHREAD_POOL -c -o $@ $<

