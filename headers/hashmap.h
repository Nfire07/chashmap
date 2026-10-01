#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
    INIT_SIZE is a small prime number 
    that resembles the psize at init_map()
*/
#define INIT_SIZE 53

typedef struct{
    char* key;
    void* value;
    size_t esize;
}Entry;

typedef struct{
    Entry* map;
    size_t lsize;
    size_t psize;
}HashMap;


HashMap* init_map();
unsigned long hash_func(char* key);
void free_map(HashMap* hmap,void(*free_func)(void*));
static size_t calc_index(HashMap* hmap, const char* key);
static size_t calc_next_index(HashMap* hmap,size_t index);
void* get(HashMap* hmap, const char* key);


#endif
