SHELL:=/usr/bin/env bash
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Wpedantic
LDFLAGS=

TARGET=list_args

.PHONY: all clean handin

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS)

clean:
	$(RM) $(TARGET)

handin: args.txt history.txt run.txt
	@cat <(echo -e "Passing arguments:\n" && cat args.txt) \
		<(echo -e "\n\nGet command from shell history:\n" && cat history.txt) \
		<(echo -e "\n\nRunning command from txt file:\n" && cat run.txt) \
		> task_3.txt
