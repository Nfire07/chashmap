#include "../headers/hashmap.h"

static size_t hash(const char* key, size_t capacity) {
    size_t hash = 5381;
    int c;

    while((c = (unsigned char)*key++))
        hash = ((hash << 5) + hash) + c;

    return hash % capacity;
}

HashMap init_map(){
    /* Allocation of the map type */
    HashMap map = malloc(sizeof(HashMap_t));
    if(map==NULL){
        return NULL;
    }
    /* Allocation of the linked list of the buckets */
    map->buckets = calloc(INITIAL_CAPACITY,sizeof(Bucket*));
    if(map->buckets==NULL){
        free(map);
        return NULL;
    }

    map->capacity=INITIAL_CAPACITY;
    map->size=0;

    return map;
}

static void free_bucket(Bucket* bucket){
    if(bucket==NULL) return;

    Bucket* temp = bucket;

    while(temp!=NULL){
        bucket=temp;
        bucket=(bucket)->next;

        free(temp->entry->key);
        free(temp->entry->value);
        free(temp->entry);
        free(temp);
        temp=bucket;
    }

}

void free_map(HashMap* map) {
    size_t i;

    if(map == NULL || *map == NULL)
        return;

    for(i = 0; i < (*map)->capacity; i++)
        free_bucket((*map)->buckets[i]);

    free((*map)->buckets);
    free(*map);

    *map = NULL;
}

bool map_set(HashMap map, const char* key, const void* value, size_t esize){
    size_t index;
    Bucket* temp;
    Entry* entry;
    void* new_value;
    char* new_key;

    /* Obtain hash index */

    index = hash(key, map->capacity);

    if(map == NULL || key == NULL ||
       value == NULL || esize == 0)
        return false;

    /* Search for the element inside the map */

    for(temp = map->buckets[index]; temp != NULL;temp = temp->next) {
        if(strcmp(temp->entry->key, key) == 0) {

            /* if the value is already in the map change it */

            new_value = malloc(esize);
            if(new_value == NULL)
                return false;

            memcpy(new_value, value, esize);

            free(temp->entry->value);
            temp->entry->value = new_value;
            temp->entry->esize = esize;

            return true;
        }
    }

    /* if nothing it's found, then allocate a new entry */

    entry = malloc(sizeof(Entry));
    if(entry == NULL)
        return false;

    new_key = malloc(strlen(key) + 1);
    if(new_key == NULL) {
        free(entry);
        return false;
    }

    strcpy(new_key, key);

    entry->value = malloc(esize);
    if(entry->value == NULL) {
        free(new_key);
        free(entry);
        return false;
    }

    memcpy(entry->value, value, esize);

    entry->key = new_key;
    entry->esize = esize;

    temp = malloc(sizeof(Bucket));
    if(temp == NULL) {
        free(entry->value);
        free(entry->key);
        free(entry);
        return false;
    }

    temp->entry = entry;
    temp->next = map->buckets[index];

    map->buckets[index] = temp;
    map->size++;

    return true;

}

void* map_get(HashMap map,const char* key){
    Bucket* temp;
    size_t index = hash(key, map->capacity);

    if(map == NULL || key == NULL)
        return NULL;


    for(temp = map->buckets[index]; temp != NULL;temp = temp->next) {
        if(strcmp(temp->entry->key, key) == 0) {
            return temp->entry->value;
        }
    }
    
    return NULL;
}