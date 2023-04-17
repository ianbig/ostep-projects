#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "mapreduce.h"
#include "tpool.h"
#include "hashmap.h"

extern __thread hashmap_t * tls_hashmap;

void MR_Emit(char *key, char *value) {
    int values[1] = {0};
    long int converted = strtol(value, NULL, 10);
    if (converted == LONG_MAX || converted == LONG_MIN) {
        return;
    }

    values[0] = converted;
    hashmap_insert(tls_hashmap, key, values, 1);
}

unsigned long MR_DefaultHashPartition(char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash;
}

static void  MR_start_mapper(int num_mappers, Mapper map, int argc, char * argv[]) {
    tpool_t * threads_pool = tpool_create_thread_pool(num_mappers);
    for (int i = 1; i < argc; i++) {
        char * filename = argv[i];
        tpool_add_work(threads_pool, (thread_func_t)map, filename);
    }

    tpool_start(threads_pool);
    tpool_wait(threads_pool);
    tpool_destroy(threads_pool);
}

static void MR_reducer_init(size_t num_reducers) {
    im.hashmap = hashmap_create(num_reducers, MR_DefaultHashPartition);
    im.lck = malloc(sizeof(*(im.lck)) * num_reducers);
    im.storage_sz = num_reducers;
}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition) {
        MR_reducer_init(num_reducers);
        MR_start_mapper(num_mappers, map, argc, argv);
        // MR_start_reducer(reduce);
}