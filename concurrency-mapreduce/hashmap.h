#ifndef __hashmap_h__
#define __hashmap_h__

#include <stdio.h>

typedef int (*HashFunc)(void * key);

typedef struct _hash_item_t {
  void * key;
  void * value;
} hash_item_t;

typedef struct _linked_list_t {
  hash_item_t * head;
  hash_item_t * tail;
} linked_list_t;

linked_list_t * list_create(linked_list_t * list);
void list_append(linked_list_t * list, hash_item_t * item_to_append);
hash_item_t * list_pop(linked_list_t * list);

typedef struct _hashmap_t {
  size_t bucket_sz;
  linked_list_t * hash_table;
  HashFunc hash_func;
} hashmap_t;

hashmap_t * hashmap_create(size_t bucket_sz, HashFunc hash_func);
void hashmap_destroy(hashmap_t * hashmap);
void hashmap_insert(char * key, void * value);
void * hashmap_get(char * key);

static int hash(void * key);


#endif