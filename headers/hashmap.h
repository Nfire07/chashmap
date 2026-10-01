#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*
    INIT_SIZE is a small prime number 
    that resembles the psize at init_map()
*/
#define INIT_SIZE 53
/*
    LOAD_FACTOR is the percentage of slots that can be
    occupied before the map doubles its psize
*/
#define LOAD_FACTOR 75

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
void free_map(HashMap* hmap,void(*free_func)(void*));
void* get(HashMap* hmap, const char* key);
bool put(HashMap* hmap,const char* key,void* value,size_t esize);
void print_map(HashMap* hmap,void(*print_func)(void*));



#endif
