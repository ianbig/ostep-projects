#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

value_t * value_create() {
    value_t * values_created = malloc(sizeof(*values_created));
    if (values_created == NULL) { return NULL; }

    values_created->sz = 0;
    values_created->head= malloc(sizeof(*(values_created->head)));
    if (values_created == NULL) { return NULL; }

    values_created->head->value = -1;
    values_created->head->next = NULL;
    values_created->tail = values_created->head;
    return values_created;
}

void value_destroy(value_t * value) {
    if (value == NULL) { return; }
    value_item_t * ptr = value->head;
    while (ptr != NULL) {
        value_item_t * to_remove = ptr;
        ptr = ptr->next;
        free(to_remove);
    }
    free(value);
}

int value_append(value_t * value, int val) {
    if (value == NULL)  { return -1; }

    value_item_t * item = malloc(sizeof(*item));
    item->value = val;
    item->next = NULL;

    value->tail->next = item;
    value->tail = item;

    value->sz++;
    return 0;
}

int value_get(value_t * value, size_t index) {
    if (index >= value->sz) {
        return -1;
    }

    int i = 0;
    value_item_t * ptr = value->head->next;
    while (i != index) {
        ptr = ptr->next;
        i++;
    }
    return ptr->value;
}

int value_pop(value_t * value) {
    if (value == NULL || !value->sz) { return -1; }

    value_item_t * to_remove = value->head->next;
    value->head->next = to_remove->next;
    int ret = to_remove->value;
    to_remove->next = NULL;
    free(to_remove);
    value->sz--;
    return ret;
}

void value_print(value_t * value) {
    value_item_t * ptr = value->head;
    while (ptr != NULL) {
        if (ptr->value == -1) {
            printf("Dummy Node --> ");
        } else {
            printf(" %d -->", ptr->value);
        }
        ptr = ptr->next;
    }
    printf("NULL\n");
}

#ifndef DEBUG
static
#endif 
hash_item_t * hash_item_create(char * key, int * values, size_t values_sz) {
    hash_item_t * item = malloc(sizeof(*item));
    if (item == NULL) { return NULL; }

    item->key = malloc(sizeof(*(item->key) * strlen(key)) + 1);
    if (item->key == NULL) { return NULL; }
    strncpy(item->key, key, strlen(key));
    item->key[strlen(key)] = '\0';


    item->value = value_create();
    if (item->value == NULL) { return NULL; }
    for (int i = 0; i < values_sz; i++) {
        value_append(item->value, values[i]);
    }

    return item;
}

#ifndef DEBUG
static 
#endif
void hash_item_destroy(hash_item_t * item) {
    if (item == NULL) { return; }
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
    list->sz++;

    return 0;
}

hash_item_t * list_pop(linked_list_t * list) {
    if (list == NULL) {
        return NULL;
    }

    hash_item_t * item_to_remove = list->head->next;
    if (item_to_remove == NULL) {
        return NULL;
    }

    list->head->next = item_to_remove->next;
    item_to_remove->next = NULL;
    list->sz--;
    return item_to_remove;
}

hash_item_t * list_get(linked_list_t * list, size_t index) {
    if (index >= list->sz) {
        return NULL;
    }

    int i = 0;
    hash_item_t * ptr = list->head->next;
    while (i != index) {
        ptr = ptr->next;
        i++;
    }

    return ptr;
}

void list_print(linked_list_t * list) {
    hash_item_t * ptr = list->head;
    while (ptr != NULL) {
        if (ptr->key == NULL) {
            printf("Dummy Node\n");
        } else {
            printf("%s: ", ptr->key);
            value_print(ptr->value);
        }
        ptr = ptr->next;
    }
    printf("End of list\n");
}

void list_destroy(linked_list_t * list) {
    if (list == NULL) { return; }

    hash_item_t * ptr = list->head;
    while (ptr != NULL) {
        hash_item_t * to_remove = ptr;
        ptr = ptr->next;
        hash_item_destroy(to_remove);
    }
    free(list);
}

hashmap_t * hashmap_create(size_t bucket_sz, HashFunc hash_func) {
    hashmap_t * hashmap = malloc(sizeof(*hashmap));
    if ( hashmap == NULL ) { return NULL; }
    hashmap->bucket_sz = bucket_sz;
    hashmap->hash_func = hash_func;
    hashmap->hash_table = malloc(sizeof(*(hashmap->hash_table)) * bucket_sz);
    for (int i = 0; i < bucket_sz; i++) {
        hashmap->hash_table[i] = list_create();
        if (hashmap->hash_table[i] == NULL) {
            return hashmap;
        }
    }

    return hashmap;
}

void hashmap_destroy(hashmap_t * hashmap) {
    for (int i = 0; i < hashmap->bucket_sz; i++) {
        list_destroy(hashmap->hash_table[i]);
    }
    free(hashmap->hash_table);
    hashmap->bucket_sz = - 1;
    hashmap->hash_func = NULL;
    free(hashmap);
}

void hashmap_insert(hashmap_t * hashmap, char * key, int * value, size_t value_sz) {
    int bucket_index = hashmap->hash_func(key) % hashmap->bucket_sz;
    hash_item_t * item = hash_item_create(key, value, value_sz);
    list_append(hashmap->hash_table[bucket_index], item);
}

linked_list_t * hashmap_get(hashmap_t * hashmap, char * key) {
    int bucket_index = hashmap->hash_func(key) % hashmap->bucket_sz;
    return hashmap->hash_table[bucket_index];
}