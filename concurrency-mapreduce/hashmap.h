#ifndef __hashmap_h__
#define __hashmap_h__

#include <stdio.h>

typedef int (*HashFunc)(void * key);
typedef struct _value_t {
  int * values;
  size_t sz;
} value_t;

value_t * value_create(int * values, size_t values_sz);
int value_append(value_t * value, int val);
int value_pop(value_t * value);
void value_destroy(value_t * value);
void value_print(value_t * value);

typedef struct _hash_item_t {
  char * key;
  value_t * value;
  struct _hash_item_t * next;
} hash_item_t;

#ifndef DEBUG
static hash_item_t * hash_item_create(char * key, int * values, size_t values_sz);
#else
hash_item_t * hash_item_create(char * key, int * values, size_t values_sz);
#endif

#ifndef DEBUG
static void hash_item_destroy(hash_item_t * item);
#else
void hash_item_destroy(hash_item_t * item);
#endif

typedef struct _linked_list_t {
  hash_item_t * head;
  hash_item_t * tail;
  size_t sz;
} linked_list_t;

linked_list_t * list_create();
int list_append(linked_list_t * list, hash_item_t * item_to_append);
void list_print(linked_list_t * list);
void list_destroy(linked_list_t * list);
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