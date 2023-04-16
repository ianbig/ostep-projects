#include <stdlib.h>
#include <assert.h>

#include "tpool.h"
#include "hashmap.h"

 __thread hashmap_t * tls_hashmap = NULL;

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

    return work;
}

#ifndef DEBUG
static 
#endif
void work_destroy(work_t * work) {
    free(work);
    work = NULL;
}

/**
 * get work from work queue
*/
#ifndef DEBUG
static 
#endif
work_t* work_queue_get(work_queue_t * work_queue) {
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
            char * arg = ((char*)(ptr->args));
            printf("task addr: %p, args: %s --> ", ptr->task, arg);
        } else {
            printf("Dummy Node --> ");
        }
        
        ptr = ptr->next;
    }
    printf("NULL\n");
}

/**
 * thread pool worker
 * @return pointer tpye, if success, it would return NULL
*/
static void * worker(void * tpool_args) {
    tpool_t * tpool = tpool_args;
    if (tpool == NULL) {
        return NULL;
    }
    
    tls_hashmap =  hashmap_create(tpool->working_thread_count, tpool_hashfunc);
    assert(tls_hashmap);

    pthread_mutex_lock(&(tpool->tpool_lck));
    while (!tpool->start) {
        pthread_cond_wait(&(tpool->start_cond), &(tpool->tpool_lck));
    }

    while (tpool->work_queue.queue_sz != 0) {
        work_t * work_to_process = work_queue_get(&(tpool->work_queue));
        pthread_mutex_unlock(&(tpool->tpool_lck));

        if (work_to_process == NULL) { 
            pthread_mutex_lock(&(tpool->tpool_lck));
            break; 
        }
        work_to_process->task(work_to_process->args);
        #ifdef DEBUG
        printf("filenames: %s\n", (char*)work_to_process->args);
        hashmap_print(tls_hashmap);
        #endif
        work_destroy(work_to_process);
        
        pthread_mutex_lock(&(tpool->tpool_lck));
        tpool->work_queue.queue_sz--;
    }

    tpool->working_thread_count--;
    pthread_cond_signal(&(tpool->exit_cond));
    pthread_mutex_unlock(&(tpool->tpool_lck));

    // TODO: push the hashmap value to the intermediate data strucutre
    hashmap_destroy(tls_hashmap);
    return NULL;
}

unsigned long tpool_hashfunc(char * key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash;
}

/**
 * Create working threads specified by num_threads
 * @return: object of tpool_t
*/
tpool_t * tpool_create_thread_pool(size_t num_threads) {
    tpool_t * tpool = malloc(sizeof(*tpool));
    assert(tpool != NULL);

    work_queue_init(&(tpool->work_queue));
    tpool->working_thread_count = num_threads;
    pthread_mutex_init(&(tpool->tpool_lck), NULL);
    pthread_cond_init(&(tpool->exit_cond), NULL);
    pthread_cond_init(&(tpool->start_cond), NULL);
    
    tpool->start = FALSE;

    pthread_t thread;
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&thread, NULL, worker, tpool);
        pthread_detach(thread);
    }

    return tpool;
}

void tpool_start(tpool_t * tpool) {
    pthread_mutex_lock(&(tpool->tpool_lck));
    tpool->start = TRUE;
    pthread_cond_broadcast(&(tpool->start_cond));
    pthread_mutex_unlock(&(tpool->tpool_lck));
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
    if (work == NULL) {return -1; }

    pthread_mutex_lock(&(tpool->tpool_lck));
    tpool->work_queue.tail->next = work;
    tpool->work_queue.tail = work;
    tpool->work_queue.queue_sz++;
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