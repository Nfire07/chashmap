#include "../headers/hashmap.h"

static size_t calc_index(HashMap* hmap, const char* key);
static unsigned long hash_func(const char* key);
static size_t calc_next_index(HashMap* hmap,size_t index);
static int resize(HashMap* hmap);
static void insert_entry(HashMap* hmap,Entry entry);

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
    if the key is already present its value is replaced
    @param hmap
    @param key is the key of the Entry
    @param value is the value of the Entry
    @param esize is the sizeof the value 
    @return true if the key was already defined, false otherwise
*/
bool put(HashMap* hmap,const char* key,void* value,size_t esize){
    if (!hmap || !key || !value) return false;

    size_t index = calc_index(hmap,key);
    size_t start = index;

    while (hmap->map[index].key != NULL) {
        if (strcmp(hmap->map[index].key, key) == 0) {
            void* new_value = (void*) malloc(esize);
            if (!new_value) return false;
            memcpy(new_value, value, esize);
            free(hmap->map[index].value);
            hmap->map[index].value = new_value;
            hmap->map[index].esize = esize;
            return true;
        }        
        index = calc_next_index(hmap,index);
        if (index == start){
            return false;
        }
    }

    if ((hmap->lsize + 1) * 100 >= hmap->psize * LOAD_FACTOR) {
        if (!resize(hmap)) return false;
        index = calc_index(hmap,key);
        while (hmap->map[index].key != NULL) {
            index = calc_next_index(hmap,index);
        }
    }

    char* new_key = strdup(key);
    if (!new_key) return false;
    void* new_value = (void*) malloc(esize);
    if (!new_value){
        free(new_key);
        return false;
    }
    memcpy(new_value, value, esize);

    hmap->map[index].key = new_key;
    hmap->map[index].value = new_value;
    hmap->map[index].esize = esize;
    hmap->lsize++;

    return false;
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
    print the map as a markdown table
    every value is printed by print_func, so the caller decides how
    the values stored in the map are rendered
    @param hmap
    @param print_func prints a single value, NULL to print the raw pointer
    @return none
*/
void print_map(HashMap* hmap,void(*print_func)(void*)){
    if(!hmap) return;

    printf("| hash | key | value |\n");
    printf("| ---- | --- | ----- |\n");

    for(size_t i = 0; i < hmap->psize; i++){
        if(!hmap->map[i].key) continue;
        printf("| %lu | %s | ",
               (unsigned long) hash_func(hmap->map[i].key),
               hmap->map[i].key);
        if(print_func) print_func(hmap->map[i].value);
        else printf("%p", hmap->map[i].value);
        printf(" |\n");
    }
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
    hash function(algorithm djb2)
    @param char* key
    @return index of the hashmap
*/
static unsigned long hash_func(const char* key){
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
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

/*
    calculate next index based on psize
    @param hmap
    @param key
    @return index of the next element
*/
static size_t calc_next_index(HashMap* hmap,size_t index){
    return (index+1) %  hmap->psize;
}

/*
    check if a number is prime
    @param num
    @return 1 if num is prime, 0 otherwise
*/
static int is_prime(size_t num){
    if(num < 2) return 0;
    if(num < 4) return 1;
    if(num % 2 == 0) return 0;
    for(size_t i = 3; i * i <= num; i += 2)
        if(num % i == 0) return 0;
    return 1;
}

/*
    find the smallest prime greater than num
    @param num
    @return the next prime after num
*/
static size_t next_prime(size_t num){
    size_t candidate = num + 1;
    while(!is_prime(candidate)) candidate++;
    return candidate;
}

/*
    double the size of the map and reinsert every entry
    the keys and the values are moved, not copied
    @param hmap
    @return 1 on success, 0 if the new map could not be allocated
*/
static int resize(HashMap* hmap){
    if(!hmap) return 0;

    size_t new_psize = next_prime(hmap->psize * 2);
    Entry* new_map = (Entry*)calloc(new_psize,sizeof(Entry));
    if(!new_map) return 0;

    Entry* old_map = hmap->map;
    size_t old_psize = hmap->psize;

    hmap->map = new_map;
    hmap->psize = new_psize;
    hmap->lsize = 0;

    for(size_t i = 0; i < old_psize; i++){
        if(old_map[i].key) insert_entry(hmap, old_map[i]);
    }

    free(old_map);
    return 1;
}

/*
    place an already allocated Entry into the map
    used by resize to move the entries
    @param hmap
    @param entry is the Entry to move
    @return none
*/
static void insert_entry(HashMap* hmap,Entry entry){
    size_t index = calc_index(hmap,entry.key);
    size_t start = index;

    while (hmap->map[index].key != NULL) {
        index = calc_next_index(hmap,index);
        if (index == start){
            free(entry.key);
            free(entry.value);
            return;
        }
    }

    hmap->map[index] = entry;
    hmap->lsize++;
}



