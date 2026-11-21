#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 16

typedef struct {
    char* key;
    void* value;
    size_t esize;
} Entry;

typedef struct Bucket {
    Entry* entry;
    struct Bucket* next;
} Bucket;

typedef struct {
    Bucket** buckets;
    size_t capacity;
    size_t size;
}HashMap_t;

typedef HashMap_t* HashMap;


HashMap init_map(void);
void free_map(HashMap* map);
bool map_set(HashMap map, const char* key, const void* value, size_t esize);
void* map_get(HashMap map, const char* key);





#endif
