#include "hashMap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void printEnt(const Entry* ent){	
	printf("details of entry at %p\n", (void*) ent);
	printf("it's key is: %s\n", (char*) ent->key);
	printf("it's value is: %s\n", (char*) ent->value);
}
int unitTests(){
	int i;
	char* str1;
	char* str2;
	char* str3;
	char* str4;
	char* str5;
	char* str6;
	HashMap* map = createMap(1, &strHash, &strcmpWrap, &defaultFree);
	str1 =(char*) malloc(2);
	str1[0] = 'a';
	str1[1] = '\0';
	str2 =(char*) malloc(2);
	str2[0] = '1';
	str2[1] = '\0';
	str3 =(char*) malloc(2);
	str3[0] = 'b';
	str3[1] = '\0';
	str4 =(char*) malloc(2);
	str4[0] = '2';
	str4[1] = '\0';
	str5 = (char*) malloc(2);
	str5[0] = 'e';
	str5[1] = '\0';
	str6 = (char*) malloc(2);
	str6[0] = 'e';
	str6[1] = '\0';
	addPair(map, str1, str2);
	addPair(map, str3, str4);
	addPair(map, str5, str6);
	debugPrintMap(map, &printEnt, 2);
	removeKey(map, str5);
	printf("%p %s teste\n", getValue(map, str3), (char*)getValue(map, str3));
	discardMap(map);
    printf("memleak pass\n");
		return 1;
}
