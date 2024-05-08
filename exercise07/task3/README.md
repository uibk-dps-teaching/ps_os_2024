# Thread Pool

A _thread pool_ should have the following characteristics:

- Have a limited number of worker threads that compute the given jobs.
- As long as the thread pool is active so are the threads.
- The pool contains a queue (first-in-first-out), for receiving jobs for the threads.
- The worker threads wait for jobs to become available in the queue.
- When there is a job available, one of the threads takes the job and computes it.
- Submitting a job returns a handle for this job, which allows waiting for its completion.

## Implementation

- To guide your efforts we provide you with a header file [`thread_pool.h`](./thread_pool.h).
- The file defines
  - prototypes for functions that are REQUIRED to be implemented
  - exemplary structs, that need to be completed
- The file is NOT complete.
- The implementation of the prototypes should be done in a separate file called: [`thread_pool.c`](./thread_pool.c).

### Prototypes

- The `void pool_create(thread_pool* pool, size_t size)` function initializes a `thread_pool` by starting `size` worker threads and initializing a queue for jobs.
  Each worker thread continuously checks the queue for submitted jobs.
  Whenever a job is available, (exactly) one worker thread removes it from the queue and runs it.
- The `job_id pool_submit(thread_pool* pool, job_function start_routine, job_arg arg)` submits a job to the thread pool and returns a `job_id`.
- The `void pool_await(job_id id)` function waits for the job with the given `job_id` to finish.
- The `void pool_destroy(thread_pool* pool)` shuts down the thread pool and frees all associated resources.
  Worker threads finish the currently running job (if any) and then stop gracefully.

### Job Specification

The `job_function` and `job_arg` types are defined as follows, similar to the arguments for `pthread_create`:

```c
typedef void* (*job_function)(void*);
typedef void* job_arg;
```

In [`task3.c`](./task3.c), we use `void* simulateBetsOnBlack(void* arg))` as the job function.
Job functions take a single argument.
You can ignore the return value of the job function.
Results can be returned by writing to memory specified by a pointer provided as the argument, like it is done in [`task3.c`](./task3.c).
