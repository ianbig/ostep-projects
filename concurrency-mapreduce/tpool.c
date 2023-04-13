#include "tpool.h"

static void work_create(thread_func_t func, void * args) {

}

static void work_destroy(work_t work) {

}

static void work_queue_get(work_queue_t * work_queue) {

}

tpool_t * tpool_create_thread_pool(size_t num_threads) {
    return NULL;
}

void tpool_add_work(tpool_t * tpool, thread_func_t work, void * args) {

}

void tpool_wait(tpool_t * tpool) {

}