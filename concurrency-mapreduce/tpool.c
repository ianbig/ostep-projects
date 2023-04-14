#include <stdlib.h>
#include <assert.h>
#include "tpool.h"

/**
 * create work object with passed in function, and arguments
 * @return work_t object
*/
static work_t * work_create(thread_func_t task, void * args) {
    if (task == NULL) {
        return NULL;
    }

    work_t * work = malloc(sizeof(*work));
    work->task = task;
    work->args = args;
    work->next = NULL;
}

static void work_destroy(work_t * work) {
    free(work);
    work = NULL;
}

/**
 * get work from work queue
*/
static work_t* work_queue_get(work_queue_t * work_queue) {
    if (work_queue->head->next == NULL) {
        return NULL;
    }

    work_t * to_remove_work = work_queue->head->next;
    work_queue->head->next = to_remove_work->next;

    return to_remove_work;
}

/**
 * Initialize work queue
*/
static void work_queue_init(work_queue_t * work_queue) {
    work_queue->head = malloc(sizeof(*(work_queue->head)));
    work_queue->head->next = NULL;
    work_queue->tail = work_queue->head;

    work_queue->queue_sz = 0;
}

/**
 * Destroy work Queue
*/
void work_queue_destroy(work_queue_t * work_queue) {
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
void work_queue_print(work_queue_t * work_queue) {
    work_t * ptr = work_queue->head;
    while (ptr != NULL) {
        if (ptr->args != NULL) {
            int arg = *((int*)(ptr->args));
            printf("task addr: %p, args: %d --> ", ptr->task, arg);
        } else {
            printf("Dummy Node --> ");
        }
        
        ptr = ptr->next;
    }
    printf("NULL\n");
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
 * @return: object of tpool_t
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

/**
 * add new work to work queue in thread pool
 * @return: -1 means undefined behavior, 0 means add to queue successfully
*/
int tpool_add_work(tpool_t * tpool, thread_func_t task, void * args) {
    if (tpool == NULL) {
        return -1;
    }

    work_t * work = work_create(task, args);
    assert(work != NULL);

    pthread_mutex_lock(&(tpool->tpool_lck));
    tpool->work_queue.tail->next = work;
    tpool->work_queue.tail = work;
    pthread_mutex_unlock(&(tpool->tpool_lck));

    return 0;
}

/**
 * Waiting the work in working queue is empty
*/
void tpool_wait(tpool_t * tpool) {
    pthread_mutex_lock(&(tpool->tpool_lck));
    while (tpool->working_thread_count != 0) {
        pthread_cond_wait(&(tpool->exit_cond), &(tpool->tpool_lck));
    }
    pthread_mutex_unlock(&(tpool->tpool_lck));
}

/**
 * recylc thread pool resource
*/
void tpool_destroy(tpool_t * tpool) {
    work_queue_destroy(&(tpool->work_queue));
    free(tpool);
    tpool = NULL;
}