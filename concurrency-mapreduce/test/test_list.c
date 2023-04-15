#include "hashmap.h"

int main() {
    linked_list_t * list = list_create();
    char * keys[] = {"Ian", "is", "awesome", "hahah", "love"};
    char * values[] = {"1", "20", "30", "40", "50"};

    for (int i = 0; i < 5; i++) {
        hash_item_t * item = hash_item_create(keys[i], &values[i], 1);
        list_append(list, item);
    }

    while (1) {
        hash_item_t * item = list_pop(list);
        if (item == NULL) {
            break;
        }

        printf("get item key %s, values: [ ", item->key);
        for (int i = 0; i < item->value->sz; i++) {
            printf("%d, ", item->value[i]);
        }
        printf("]\n");
    }

    list_print(list);
    

    hash_item_t * item = hash_item_create(keys[3], &values[3], 1);
    list_append(list, item);

    item = hash_item_create(keys[0], &values[0], 1);
    list_append(list, item);
    list_destroy(list);
}