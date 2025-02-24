#include "hashMap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int strcmpWrap(void* a, void* b){
    return strcmp((char*)a,(char*)b);
}
int unitTests(){
    for (int i = 0; i < 2000; i++){
        HashMap* map = createMap(6, &strHash, &strcmpWrap, &defaultFree);

        char* str1 =(char*) malloc(2);
        str1[0] = 'a';
        str1[1] = '\0';
        char* str2 =(char*) malloc(2);
        str2[0] = '1';
        str2[1] = '\0';
        char* str3 =(char*) malloc(2);
        str3[0] = 'b';
        str2[1] = '\0';
        char* str4 =(char*) malloc(2);
        str4[0] = '2';
        str4[1] = '\0';
        addPair(map, str1, str2);
        addPair(map, str3, str4);
        char* str5 =(char*) malloc(2);
        str5[0] = 'a';
        str5[1] = '\0';
        char ss[] = "a";
       discardMap(map);
    }
    printf("memleak pass\n");
    
}
