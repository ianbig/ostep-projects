#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

value_t * value_create(int * values, size_t values_sz) {
    value_t * values_created = malloc(sizeof(*values_created) * 1);
    if (values_created == NULL) { return NULL; }

    values_created->sz = values_sz;
    values_created->values = malloc(sizeof(*(values_created->values)) * values_sz);
    if (values_created->values == NULL) { return NULL; }
    for (int i = 0; i < values_sz; i++) {
        values_created->values[i] = values[i];
    }

    return values_created;
}

void value_destroy(value_t * value) {
    free(value->values);
    free(value);
}

#ifndef DEBUG
static hash_item_t * hash_item_create(char * key, int * values, size_t values_sz) {
    hash_item_t * item = malloc(sizeof(*item));
    if (item == NULL) { return NULL; }

    item->key = malloc(sizeof(*(item->key) * strlen(key)) + 1);
    if (item->key == NULL) { return NULL; }
    strncpy(item->key, key, strlen(key));
    item->key[strlen(key)] = '\0';


    item->value = value_create(values, values_sz);
    if (item->value == NULL) { return NULL; }

    return item;
}
#else
hash_item_t * hash_item_create(char * key, int * values, size_t values_sz) {
    return hash_item_create(key, values, values_sz);
}
#endif

static void hash_item_destroy(hash_item_t * item) {
    free(item->key);
    value_destroy(item->value);
    free(item);
}

linked_list_t * list_create() {
    linked_list_t * list = NULL;
    list = malloc(sizeof(*list));
    if (list == NULL) {
        return NULL;
    }

    list->head = malloc(sizeof(*(list->head)));
    list->head->key = NULL;
    list->head->value = NULL;
    list->head->next = NULL;
    list->tail = list->head;
    list->sz = 0;

    return list;
}

/**
 * @return -1 means error
*/
int list_append(linked_list_t * list, hash_item_t * item_to_append) {
    if (list == NULL) {
        return -1;
    }

    list->tail->next = item_to_append;
    list->tail = item_to_append;
    list->tail = NULL;

    return 0;
}

hash_item_t * list_pop(linked_list_t * list) {
    if (list == NULL) {
        return NULL;
    }

    hash_item_t * item_to_remove = list->head->next;
    list->head->next = item_to_remove->next;
    item_to_remove->next = NULL;
    return item_to_remove;
}

hashmap_t * hashmap_create(size_t bucket_sz, HashFunc hash_func) {
    return NULL;
}

void hashmap_destroy(hashmap_t * hashmap) {

}

void hashmap_insert(char * key, void * value) {

}

void * hashmap_get(char * key) {
    return NULL;
}