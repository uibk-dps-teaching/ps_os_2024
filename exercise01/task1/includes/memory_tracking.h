#ifndef MEMORY_TRACKING_H
#define MEMORY_TRACKING_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Memory tracker to keep track of memory allocations and deallocations

#ifndef MEMORY_TRACKER_SIZE
	#define MEMORY_TRACKER_SIZE 100
#endif

enum error {
	NULL_FREE,
	DOUBLE_FREE,
	MAX_ALLOC,
	FAILED_ALLOC,
	MEMORY_LEAK
};

typedef struct _ {
	size_t address;
	size_t size;
	uint16_t line;
} memory_t;

typedef struct memory_tracker {
	memory_t memory[MEMORY_TRACKER_SIZE];
	size_t total_allocated_memory;
	size_t total_freed_memory;
	uint8_t error;
} memory_tracker_t;

// find the first free memory slot in the memory tracker
memory_t* find_address(size_t address);

// add a memory allocation to the memory tracker
void allocate_memory(size_t address, size_t size, uint16_t line);

// remove a memory allocation from the memory tracker
bool free_memory(size_t address, uint16_t line);

// print the memory tracker
void print_memory_tracker();

bool check_memory_leak();
bool check_clean_memory_handling();

static inline void* my_malloc(size_t size, uint32_t line) {
	void* ptr = malloc(size);

	allocate_memory((size_t)ptr, size, line);

	return ptr;
}

static inline void* my_calloc(size_t num, size_t size, uint32_t line) {
	void* ptr = calloc(num, size);

	allocate_memory((size_t)ptr, size * num, line);

	return ptr;
}

static inline void my_free(void* ptr, uint32_t line) {
	if (free_memory((size_t)ptr, line)) {
		free(ptr);
	}
}

#define malloc(size) my_malloc(size, __LINE__)
#define calloc(num, size) my_calloc(nun, size, __LINE__)
#define free(size) my_free(size, __LINE__)

#endif