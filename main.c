#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashMap.h"
#include "tests.h"
/* file with an example of how to use the library.*/
size_t strHash2(void* key){
    unsigned int code = 0;
    unsigned int i = 0;
    while (((char*)key)[i] != '\0'){
        code += ((char*)key)[i];
        i++;
    }
    return code;
}
int isSame(void* a, void* b){
    return strcmp((char*)a, (char*)b);
}

void freeStr(Entry* entry){
	/*use a function like this to clear your custom inserted data structure
	since it can contain other pointers inside it.*/
	free(entry->key);
	free(entry->value);
}
int main(int argc, char *argv[]){
    HashMap* a = createMap(2, &strHash, &isSame, &freeStr);
		printf("aa\n");
    /* THIS IS A WRONG WAY TO ADD PAIRS, SINCE YOU SHOULD ALLOC THEN ON THE HEAP*/
    addPair(a,(void*)"a",(void*)"b");
    addPair(a,(void*)"c",(void*)"b");
    addPair(a,(void*)"d",(void*)"eeeee");
		printf("bb\n");
    printf("%s\n",  (char*)getValue(a,"d"));
    /*test for no collisions*/
    int i = 0;
		for (i = 0; i <  200; i++){
        HashMap* map = createMap(6, &strHash2, &isSame, &freeStr);

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
        printf("%s",  (char*)getValue(map, ss));
    }
    unitTests();
    return 0;
}
