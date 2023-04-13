#include "mapreduce.h"
#include <pthread.h>

void MR_Emit(char *key, char *value) {
    // TODO: stub function
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    // TODO: stub function
    return 0;
}

static void sched(Mapper map, int num_mappers) {

}

void start_mapper() {
    // TODO: stub function
}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition) {

        regsiter_mapper(map, num_mappers); // keep mapper thread information
        register_reduer(reduce, num_reducers);

        start_mapper();
        start_shuffler();
        start_reducer();
}