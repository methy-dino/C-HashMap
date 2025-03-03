#include "hashMap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void printEnt(Entry* ent){
	printf("details of entry at %p\n", (void*) ent);
	printf("it's key is: %s\n", ent->key);
	printf("it's value is: %s\n", ent->value);
}
int strcmpWrap(void* a, void* b){
    return strcmp((char*)a,(char*)b);
}
int unitTests(){
    for (int i = 0; i < 2000; i++){
        HashMap* map = createMap(6, &strHash, &strcmpWrap, &defaultFree);
        //debugPrintMap(map, &printEnt, 2);
        char* str1 =(char*) malloc(2);
        str1[0] = 'a';
        str1[1] = '\0';
        char* str2 =(char*) malloc(2);
        str2[0] = '1';
        str2[1] = '\0';
        char* str3 =(char*) malloc(2);
        str3[0] = 'b';
        str3[1] = '\0';
        char* str4 =(char*) malloc(2);
        str4[0] = '2';
        str4[1] = '\0';
        addPair(map, str1, str2);
        addPair(map, str3, str4);
        char* str5 =(char*) malloc(2);
        str5[0] = 'a';
        str5[1] = '\0';
        debugPrintMap(map, &printEnt, 2);
       discardMap(map);
    }
    printf("memleak pass\n");
}
