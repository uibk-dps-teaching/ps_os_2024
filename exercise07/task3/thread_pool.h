#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stddef.h> // for size_t

typedef void* (*job_function)(void*);
typedef void* job_arg;
typedef /* TODO */ job_id;

/***
 * This is the stub of a simple job queue.
 */

struct job_queue_entry {
	// TODO: Design the contents for a queue, which stores jobs.
};

/***
 * This is the stub for the thread pool that uses the queue.
 * Implement at LEAST the Prototype functions below.
 */

typedef struct {
	// TODO: Design the contents of a thread pool
} thread_pool;

// Prototypes for REQUIRED functions
void pool_create(thread_pool* pool, size_t size);
job_id pool_submit(thread_pool* pool, job_function start_routine,
                   job_arg arg); // You need to define a datatype for the job_id (chose wisely).
void pool_await(job_id id);
void pool_destroy(thread_pool* pool);

#endif
