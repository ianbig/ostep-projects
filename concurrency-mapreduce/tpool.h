#ifndef __tpool__h
#define __tpool__h

#include <stdio.h>
#include <pthread.h>

typedef void* (*thread_func_t)(void *);

typedef struct _work_t {
  thread_func_t task;
  void * args;
  struct _work_t * next;
} work_t;

static work_t* work_create(thread_func_t func, void * args);
#ifndef DEBUG
static void work_destroy(work_t * work);
#else
void work_destroy(work_t * work);
#endif

/** 
 * queue to store pending work, if a thread want to get a work, called work_queue_get()
 * as it would return a new work
 * 
 * @thread_safety: No
*/
typedef struct _work_queue_t {
  work_t * head;
  work_t * tail;
  size_t queue_sz;
} work_queue_t;

static void work_queue_init(work_queue_t * work_queue);
static void work_queue_destroy(work_queue_t * work_queue);

#ifndef DEBUG
static work_t* work_queue_get(work_queue_t * work_queue);
#else
work_t* work_queue_get(work_queue_t * work_queue);
#endif

void work_queue_print(work_queue_t * work_queue);

typedef struct _tpool_t {
  work_queue_t work_queue;
  size_t working_thread_count;
  pthread_cond_t exit_cond;
  pthread_mutex_t tpool_lck;
} tpool_t;

tpool_t * tpool_create_thread_pool(size_t num_threads);
int tpool_add_work(tpool_t * tpool, thread_func_t work, void * args);
void tpool_wait(tpool_t * tpool);
void tpool_destroy(tpool_t * tpool);
static void * worker(void * args);

#endif

