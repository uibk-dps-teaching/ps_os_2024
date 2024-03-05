#include "memory_tracking.h"
#include "colorfull_print.h"

#include <stdio.h>

// print warning in yellow
// #define PRINT_LINE_WARNING(msg, line) PRINT_WARNING("WARNING on line " line ": " msg)
#define PRINT_WARNING(msg, line) \
	PRINT_COLOR(YELLOW_FG, printf("WARNING on line %d: %s\n", line, msg))

// global memory tracker
memory_tracker_t memory_tracker;

memory_t* find_address(size_t address) {
	for (size_t i = 0; i < MEMORY_TRACKER_SIZE; i++) {
		if (memory_tracker.memory[i].address == address) {
			return &memory_tracker.memory[i];
		}
	}

	return NULL;
}

void allocate_memory(size_t address, size_t size, uint16_t line) {
	if (address == 0) {
		memory_tracker.error |= FAILED_ALLOC;
		PRINT_WARNING("malloc failed", line);
		return;
	}

	memory_t* memory = find_address(0);

	if (memory == NULL) {
		memory_tracker.error |= MAX_ALLOC;
		PRINT_WARNING("Maximum memory allocations reached", line);
		return;
	}

	memory->address = address;
	memory->size = size;
	memory->line = line;

	memory_tracker.total_allocated_memory += size;
}

bool free_memory(size_t address, uint16_t line) {
	if (address == 0) {
		memory_tracker.error |= NULL_FREE;
		PRINT_WARNING("freeing NULL pointer (possible double free)", line);
		return false;
	}

	memory_t* memory = find_address(address);

	if (memory == NULL) {
		memory_tracker.error |= DOUBLE_FREE;
		PRINT_WARNING("freeing unallocated memory (possible double free)", line);
		return false;
	}

	memory->address = 0;
	memory_tracker.total_freed_memory += memory->size;
	return true;
}

void print_memory_tracker(void) {
	printf("\n======== Memory Summary ========\n");
	printf(" Total allocated memory %zu bytes\n", memory_tracker.total_allocated_memory);
	printf(" Total freed memory     %zu bytes\n", memory_tracker.total_freed_memory);
	size_t total_leaked_memory =
	    memory_tracker.total_allocated_memory - memory_tracker.total_freed_memory;
	printf(" Memory leaked          %zu bytes\n", total_leaked_memory);

	if (total_leaked_memory > 0) {
		printf("\n Still allocated memory:\n");
		for (size_t i = 0; i < MEMORY_TRACKER_SIZE; i++) {
			if (memory_tracker.memory[i].address != 0) {
				printf(" Memory of size %zu allocated on line %d\n", memory_tracker.memory[i].size,
				       memory_tracker.memory[i].line);
			}
		}
	}
	printf("================================\n");
}

bool is_memory_leaked(void) {
	return memory_tracker.total_allocated_memory != memory_tracker.total_freed_memory;
}

bool check_clean_memory_handling() {
	return memory_tracker.error == 0 && !is_memory_leaked();
}