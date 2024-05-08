#include "thread_pool.h"

void pool_create(thread_pool* pool, size_t size){
#error "TODO"
}

job_id pool_submit(thread_pool* pool, job_function start_routine, job_arg arg) {
#error "TODO"
}

void pool_await(job_id id) {
#error "TODO"
}

void pool_destroy(thread_pool* pool) {
#error "TODO"
}
