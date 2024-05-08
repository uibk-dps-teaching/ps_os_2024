#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "roulette.h"

#if ((!defined(THREAD_PER_JOB) && !defined(THREAD_POOL)) || (defined(THREAD_PER_JOB) && defined(THREAD_POOL)))
	#error "Define exactly one of THREAD_PER_JOB, THREAD_POOL" // this causes an error
#endif

#ifdef THREAD_POOL
	#include "thread_pool.h"
	#define POOL_SIZE 5
#else
	#include <pthread.h>
#endif

#define NUM_EVALUATIONS 2000
#define NUM_SPINS 100000lu

typedef struct job_struct {
	unsigned int seed;
	uint64_t result;
	RouletteNumber* wheel;
} job_struct;

void* simulateBetsOnBlack(void* arg) {
	job_struct* job = (job_struct*)arg;
	for (long unsigned i = 0; i < NUM_SPINS; i++) {
		int spinIndex = rand_r(&job->seed) % 38;  // Random spin
		RouletteNumber spinResult = job->wheel[spinIndex];

		// Check for black color bet
		if (spinResult.color == BLACK) {
			(job->result)++;
		}
	}
	return NULL;
}


int main(void) {

	job_struct* jobs = malloc(NUM_EVALUATIONS * sizeof(job_struct));
	if (jobs == NULL) {
		fprintf(stderr, "Could not allocate memory for jobs.\n");
		exit(EXIT_FAILURE);
	}
	RouletteNumber wheel[38];
	initializeWheel(wheel);

#ifdef THREAD_POOL
	thread_pool my_pool;
	pool_create(&my_pool, POOL_SIZE);
	printf("Creating thread pool with size %d.\n", POOL_SIZE);

	job_id job_ids[NUM_EVALUATIONS];
#else
	pthread_t threads[NUM_EVALUATIONS];
#endif
	for (size_t i = 0; i < NUM_EVALUATIONS; i++) {
		jobs[i] = (job_struct){ .seed = i, .result = 0, .wheel=wheel };
#ifdef THREAD_POOL
		job_ids[i] = pool_submit(&my_pool, simulateBetsOnBlack, jobs + i);
#else
		if (pthread_create(&threads[i], NULL, simulateBetsOnBlack, jobs + i) != 0) {
			perror("Creating Threads");
			return EXIT_FAILURE;
		}
#endif
	}

	uint64_t min_result = UINT64_MAX;
	uint64_t max_result = 0;
	for (size_t i = 0; i < NUM_EVALUATIONS; i++) {
#ifdef THREAD_POOL
		pool_await(job_ids[i]);
#else
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Joining Threads");
			return EXIT_FAILURE;
		}
#endif
		unsigned job_result = jobs[i].result;
		min_result = job_result < min_result ? job_result : min_result;
		max_result = job_result > max_result ? job_result : max_result;
	}

#ifdef THREAD_POOL
	pool_destroy(&my_pool);
#endif
	free(jobs);
	int black_pockets = 18;
	int total_pockets = 38;
	float probability = ((float)black_pockets / total_pockets)*NUM_SPINS;
	printf("Betting on Black numbers won maximal %ld times and minimum %ld times of expected %.2f times\n",max_result ,min_result, probability );
}
