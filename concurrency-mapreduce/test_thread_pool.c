#include "tpool.h"
#include <assert.h>


void * work(void * args) {
    return NULL;
}

int main() {
    tpool_t *tpool = tpool_create_thread_pool(10);
    char * args[10] = {
            "0", "1", "2", "3", "4", "5",
            "6", "7", "8", "9"
    };

    for (int i = 0; i < 10; i++) {
        assert(tpool_add_work(tpool, work, args[i]) == 0);
    }

    work_queue_print(&(tpool->work_queue)); // expect 10 work
    tpool_start(tpool);
    tpool_wait(tpool);
    work_queue_print(&(tpool->work_queue)); // expect 0 work
    tpool_destroy(tpool);
}