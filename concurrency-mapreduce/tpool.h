#ifndef __tpool__h
#define __tpool__h

#include <stdio.h>

typedef void* (*thread_func_t)(void *);

typedef struct _work_t {
  thread_func_t work;
  void * args;
} work_t;

static void create_work(thread_func_t func, void * args);
static void destroy_work(work_t work);

typedef struct _work_queue_t {
  work_t * head;
  work_t * tail;
} work_queue_t;

static void get_work();
static void destroy_work_queue();

typedef struct _tpool_t {
  work_queue_t * work_queue;
} tpool_t;

void tpool_create_thread_pool(size_t num_threads);
void tpool_add_work(tpool_t * tpool, thread_func_t work, void * args);
void tpool_wait(tpool_t * tpool);

#endif

