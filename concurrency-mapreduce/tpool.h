#ifndef __tpool__h
#define __tpool__h

#include <stdio.h>
#include <pthread.h>

typedef void* (*thread_func_t)(void *);

typedef struct _work_t {
  thread_func_t work;
  void * args;
  struct _work_t * next;
} work_t;

static void work_create(thread_func_t func, void * args);
static void work_destroy(work_t work);

typedef struct _work_queue_t {
  work_t * head;
  work_t * tail;
  pthread_mutex_t worker_queue_lck;
} work_queue_t;

static void work_queue_get(work_queue_t * work_queue);

typedef struct _tpool_t {
  work_queue_t * work_queue;
  size_t working_thread_count;
} tpool_t;

tpool_t * tpool_create_thread_pool(size_t num_threads);
void tpool_add_work(tpool_t * tpool, thread_func_t work, void * args);
void tpool_wait(tpool_t * tpool);

#endif

