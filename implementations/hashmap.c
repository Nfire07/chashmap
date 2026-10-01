#include "../headers/hashmap.h"

/*
    allocates the map
    @param none
    @return HashMap*
*/
HashMap* init_map(){
    HashMap* hmap = (HashMap*) malloc(sizeof(HashMap));
    if(!hmap) return NULL;
    hmap->psize = INIT_SIZE;
    hmap->lsize=0;

    hmap->map = (Entry*)calloc(hmap->psize,sizeof(Entry));
    if(!hmap->map){
        free(hmap);
        return NULL;
    }

    return hmap;
}

/*
    put a value identified by a key into a hashmap
    @param hmap
    @param key is the key of the Entry
    @param value is the value of the Entry
    @param esize is the sizeof the value 
    @return none
*/
void put(HashMap* hmap,const char* key,void* value,size_t esize){
    if (!hmap || !key || !value) return;

    size_t index = calc_index(hmap,key);
    size_t start = index;

    while (hmap->map[index].key != NULL) {
        if (strcmp(hmap->map[index].key, key) == 0) {
            free(hmap->map[index].value); 
            hmap->map[index].value = malloc(esize);
            if (!hmap->map[index].value) return; 
            memcpy(hmap->map[index].value, value, esize);
            hmap->map[index].esize = esize;
            return;
        }        
        index = calc_next_index(hmap,index);
        if (index == start){
            return;
        }
    }

    hmap->map[index].key = strdup(key); 
    hmap->map[index].value = malloc(esize);
    if (!hmap->map[index].value) return;
    memcpy(hmap->map[index].value, value, esize);
    hmap->map[index].esize = esize;
    hmap->lsize++;

}

/*
    get a value given a key
    @param hmap
    @param key is the key of the value
    @return value
*/
void* get(HashMap* hmap, const char* key) {
    if (!hmap || !key) return NULL;

    size_t index = calc_index(hmap, key);
    size_t start = index;

    while (hmap->map[index].key != NULL) {
        if (strcmp(hmap->map[index].key, key) == 0) {
            return hmap->map[index].value;
        }

        index = calc_next_index(hmap, index);
        if (index == start) {
            return NULL;
        }
    }

    return NULL;
}


/*
    deep free of the map
    @param hmap
    @param free_func based on the element of the map
    @return none
*/
void free_map(HashMap* hmap,void(*free_func)(void*)){
    if(!hmap) return;
    if(hmap->map){
        for(size_t i=0;i<hmap->psize;i++){
            if(hmap->map[i].key){
                free(hmap->map[i].key);
                if(hmap->map[i].value)
                    free_func(hmap->map[i].value);
            }
        }
        free(hmap->map);
    }
    free(hmap);
}

/*
    calculate hash index based on psize
    @param hmap
    @param key
    @return index of the element described by the key
*/
static size_t calc_index(HashMap* hmap, const char* key) {
    if (!hmap || !key) return 0; 
    size_t raw_hash = hash_func(key);
    return raw_hash % hmap->psize;
}

static size_t calc_next_index(HashMap* hmap,size_t index){
    return (index+1) %  hmap->psize;
}

/*
    hash function(algorithm djb2)
    @param char* key
    @return index of the hashmap
*/
unsigned long hash_func(char* key){
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
}

