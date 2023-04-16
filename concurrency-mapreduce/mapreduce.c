#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "mapreduce.h"
#include "tpool.h"
#include "hashmap.h"

void MR_Emit(char *key, char *value) {
    int values[1] = {0};
    values[0] = strtol(value, NULL, 10);
    if (value[0] == LONG_MAX || value[0] == LONG_MIN) {
        return;
    }

    hashmap_insert(tls_hashmap, key, values, 1);
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

static void start_mapper(int num_mappers, Mapper map, int argc, char * argv[]) {
    tpool_t * threads_pool = tpool_create_thread_pool(num_mappers);
    for (int i = 1; i < argc; i++) {
        char * filename = argv[i];
        tpool_add_work(threads_pool, (thread_func_t)map, filename);
    }
    tpool_start(threads_pool);
    tpool_wait(threads_pool);
    tpool_destroy(threads_pool);
}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition) {
        start_mapper(num_mappers, map, argc, argv);
        // start_reducer();
}