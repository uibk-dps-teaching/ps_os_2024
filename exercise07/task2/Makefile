CFLAGS = -std=c11 -Wall -Wextra -pedantic -O2 -g -D_DEFAULT_SOURCE 
LDFLAGS = -pthread

TARGETS = task2

.PHONY:
all: $(TARGETS)

.PHONY:
clean:
	$(RM) $(TARGETS) *.o 

task2: task2.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

