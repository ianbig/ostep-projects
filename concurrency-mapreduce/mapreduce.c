#include "mapreduce.h"
#include "tpool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void MR_Emit(char *key, char *value) {
    // TODO: stub function
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    // TODO: stub function
    return 0;
}

static void start_mapper(int num_mappers, Mapper map, int argc, char * argv[]) {
    tpool_t * threads_pool = tpool_create_thread_pool(num_mappers);
    for (int i = 1; i < argc; i++) {
        char * filename = argv[i];
        tpool_add_work(threads_pool, (thread_func_t)map, filename);
    }

    tpool_wait(threads_pool);
    free(threads_pool);
}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition) {
        start_mapper(num_mappers, map, argc, argv);
        // start_shuffler();
        // start_reducer();
}