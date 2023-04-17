#ifndef __mapreduce_h__
#define __mapreduce_h__

#include "hashmap.h"

// Different function pointer types used by MR
typedef char *(*Getter)(char *key, int partition_number);
typedef void (*Mapper)(char *file_name);
typedef void (*Reducer)(char *key, Getter get_func, int partition_number);
typedef unsigned long (*Partitioner)(char *key);

typedef struct _MR_Intermediate_Storage_t {
	hashmap_t * hashmap;
	pthread_mutex_t * lck;
	size_t storage_sz;
} MR_Intermediate_Storage_t;

MR_Intermediate_Storage_t im;

// External functions: these are what you must define
void MR_Emit(char *key, char *value);

unsigned long MR_DefaultHashPartition(char *key);

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition);

static void MR_start_mapper(int num_mappers, Mapper map, int argc, char * argv[]);
static void MR_reducer_init(size_t num_reducers);


#endif // __mapreduce_h__
