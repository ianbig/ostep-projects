#include "mapreduce.h"

void MR_Emit(char *key, char *value) {
    // TODO: stub function
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    // TODO: stub function
    return 0;
}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition) {
        // TODO: stub function
}