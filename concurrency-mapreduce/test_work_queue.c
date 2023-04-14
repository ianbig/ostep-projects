#include "tpool.h"

/**
 * This test is soley for the funcionality of work_queue_t.
 * To enable this test, you have to remove static property of the following function:
 *  work_queue_get()
 *  work_destroy()
*/

void * work(void * args) {

}

int main() {
    tpool_t *tpool = tpool_create_thread_pool(1);
    int args[10] = {0};

    for (int i = 0; i < 10; i++) {
        args[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        tpool_add_work(tpool, work, &args[i]);
    }

    work_queue_print(&(tpool->work_queue));

    for (int i = 0; i < 10; i++) {
        work_t * work = work_queue_get(&(tpool->work_queue));
        int args = *((int*)work->args);
        printf("Get work info, task: %p, args: %d\n", work->task, args);
        work_destroy(work);
    }

    work_queue_print(&(tpool->work_queue));
    tpool_wait(tpool);
    tpool_destroy(tpool);
}