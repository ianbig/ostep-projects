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
    char * filename[] = {"a.txt", "b.txt", "c.txt", "d.txt"};
    int num[][1] = {
        { 10 },
        { 9 },
        { 2 },
        { 1 },
    };

    for (int i = 0; i < 4; i++) {
        hashmap_insert(map, filename[i], num[i], 1);
    }

    for (int i = 0; i < 4; i++) {
        linked_list_t * list = hashmap_get(map, filename[i]);
        for (int j = 0; j < list->sz; j++) {
            hash_item_t * item = list_get(list, j);
            printf("item keys: %s, value: [ ", item->key);
            value_t * val = item->value;
            for (int k = 0; k < val->sz; k++) {
                printf("%d, ", value_get(val, k));
            }
            printf("]\n");
        }
    }

    hashmap_destroy(map);
}