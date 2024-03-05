CFLAGS = -std=c11 -Wall -Wextra -g
CC = gcc

.PHONY: all clean

COLOUR_YELLOW = \033[0;33m
COLOUR_GREEN = \033[0;32m
COLOUR_RED = \033[0;31m
COLOUR_BLUE = \033[0;34m
END_COLOUR = \033[0m

SRC_DIRS = . includes
BUILD_DIR = build

SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
DEPS = $(wildcard $(addsuffix /*.h, $(SRC_DIRS)))
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

EXECUTABLE = ./task1

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

vpath %.c $(SRC_DIRS)
$(BUILD_DIR)/%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXECUTABLE)
	rm -f ./output.txt
	@echo -e "$(END_COLOUR)"

# run tests
.PHONY: test
test: $(BUILD_DIR) task1
	./task1 || echo "Something went wrong"
	@echo -e "$(END_COLOUR)"

.PHONY: check
check: $(BUILD_DIR) task1
	@echo -e "$(COLOUR_BLUE)Checking if test run successful: $(END_COLOUR)"
	@./task1 > output.txt
	@diff -u --ignore-matching-lines='Total allocated memory' --ignore-matching-lines='Total freed memory' expected_output.txt output.txt && (echo -e "$(COLOUR_GREEN)Test successful$(END_COLOUR)") || (echo -e "$(COLOUR_RED)Output is different from expected output$(END_COLOUR)")
	@rm ./output.txt
	@echo -e "$(END_COLOUR)"