#include <stdio.h>
#include "headers/hashmap.h"

typedef struct{
    int eta;
    char sesso;
}Persona;

void print_persona(void* persona){
    Persona* p = persona;

    printf("p->eta: %d\n",p->eta);
    printf("p->sesso: %c\n",p->sesso);
}

int main(void){
    HashMap map = init_map();
    Persona p = {67,'M'};

    if(map_set(map,"Stringa","alberto",sizeof("alberto"))){
        printf("Inserimento della'stringa avvenuto con successo!\n");
    }else{
        printf("Inserimento della'stringa fallito con successo!\n");
    }

    if(map_set(map,"Persona",&p,sizeof(p))){
        printf("Inserimento della persona avvenuto con successo!\n");
    }else{
        printf("Inserimento della persona avvenuto con successo!\n");
    }


    printf("Stringa prelevata: %s\n",(char*)map_get(map,"Stringa"));
    print_persona(map_get(map, "Persona"));


    free_map(&map);

    return 0;
}