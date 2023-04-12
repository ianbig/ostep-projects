#include "mapreduce.h"
#include <stdlib.h>

void mapper(char * filename) {

}

char * getter(char *key, int partition_number) {
    return NULL;
}

void reducer(char *key, Getter get_func, int partition_number) {

}

int main(int argc, char ** argv) {
    MR_Run(argc, argv, mapper, 1, reducer, 1, MR_DefaultHashPartition);
}