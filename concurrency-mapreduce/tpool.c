#include <stdlib.h>
#include <assert.h>
#include "tpool.h"

static void work_create(thread_func_t func, void * args) {

}

static void work_destroy(work_t work) {

}

static void work_queue_get(work_queue_t * work_queue) {

}

static void work_queue_init(work_queue_t * work_queue) {
    // TODO: add dummy node
    work_queue->head = malloc(sizeof(*(work_queue->head)));
    work_queue->tail = malloc(sizeof(*(work_queue->tail)));
    work_queue->head->next = work_queue->tail;
    work_queue->tail->next = NULL;
    work_queue->queue_sz = 0;
}

static void work_queue_destroy(work_queue_t * work_queue) {
    work_t * ptr = work_queue->head;
    while (ptr != NULL) {
        work_t * to_remove = ptr;
        ptr = ptr->next;
        free(to_remove);
    }
}

/**
 * print out worke queue info
*/
static void print_work_queue(work_queue_t * work_queue) {

}

static void * worker(void * tpool_args) {
    tpool_t * tpool = tpool_args;
    pthread_mutex_lock(&(tpool->tpool_lck));
    while (tpool->work_queue.queue_sz != 0) {

    }
    tpool->working_thread_count--;
    pthread_cond_signal(&(tpool->exit_cond));
    pthread_mutex_unlock(&(tpool->tpool_lck));
    return NULL;
}

/**
 * Create working threads specified by num_threads
*/
tpool_t * tpool_create_thread_pool(size_t num_threads) {
    tpool_t * tpool = malloc(sizeof(*tpool));
    assert(tpool != NULL);

    if (num_threads == 0) {
        num_threads = 1;
    }

    work_queue_init(&(tpool->work_queue));
    tpool->working_thread_count = num_threads;
    pthread_mutex_init(&(tpool->tpool_lck), NULL);
    pthread_cond_init(&(tpool->exit_cond), NULL);

    pthread_t thread;
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&thread, NULL, worker, tpool);
        pthread_detach(thread);
    }

    return tpool;
}

void tpool_add_work(tpool_t * tpool, thread_func_t work, void * args) {

}

/**
 * Waiting the work in working queue is empty
*/
void tpool_wait(tpool_t * tpool) {
    pthread_mutex_lock(&(tpool->tpool_lck));
    while (tpool->working_thread_count != 0) {
        pthread_cond_wait(&(tpool->exit_cond), &(tpool->tpool_lck));
    }
    work_queue_destroy(&(tpool->work_queue));
    pthread_mutex_unlock(&(tpool->tpool_lck));
}