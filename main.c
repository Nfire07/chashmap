#include <stdio.h>
#include "headers/hashmap.h"

void print_func(void* value){
    int vint = *(int*)value;
    printf("%d",vint);
}

int main(void){
    HashMap* hmap = init_map();

    char* names = {"a","b","a"};

    for(int i=0;i<3;i++){
        if(put(hmap,&names[i],(int)0,sizeof(int))){
            put(hmap,&names[i],get(hmap,&names[i])+1,sizeof(int));
        }
    }

    print_map(hmap,print_func);

    free_map(hmap,free);
    return 0;
}