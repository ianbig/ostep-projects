#include "tpool.h"
#include <assert.h>

/**
 * This test is soley for the funcionality of work_queue_t.
 * To enable this test, you have to remove static property of the following function:
 *  work_queue_get()
 *  work_destroy()
*/

void * work(void * args) {
    return NULL;
}

int main() {
    tpool_t *tpool = tpool_create_thread_pool(0);
    char * args[10] = {
            "0", "1", "2", "3", "4", "5",
            "6", "7", "8", "9"
    };

    for (int i = 0; i < 10; i++) {
        assert(tpool_add_work(tpool, work, args[i]) == 0);
    }

    work_queue_print(&(tpool->work_queue));

    for (int i = 0; i < 10; i++) {
        work_t * work = work_queue_get(&(tpool->work_queue));
        printf("Get work info, task: %p, args: %s\n", work->task, (char*)work->args);
        work_destroy(work);
    }

    tpool_wait(tpool);
    tpool_destroy(tpool);
}