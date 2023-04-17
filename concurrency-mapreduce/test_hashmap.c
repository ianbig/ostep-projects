#include "hashmap.h"

unsigned long MR_DefaultHashPartition(char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash;
}

int main() {
    hashmap_t * map = hashmap_create(10, MR_DefaultHashPartition);
    char * filename[] = {"a.txt", "b.txt", "c.txt", "d.txt", "a.txt", "d.txt"};
    int num[][1] = {
        { 10 },
        { 9 },
        { 2 },
        { 1 },
        { 20 },
        { 3 },
    };

    for (int i = 0; i < 6; i++) {
        hashmap_insert(map, filename[i], num[i], 1);
    }

    hashmap_print(map);

    hashmap_destroy(map);
}