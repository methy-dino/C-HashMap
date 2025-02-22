#include <stdlib.h>
#include <stdio.h>
#include "defaults.h"
typedef struct {
    void* key;
    void* value;
} Entry;
typedef struct {
    Entry* entries;
		// used to hash the value of your entry.
    unsigned int (*hashf)(void*);
    // used for comparison between keys, aka are they the same keys?
    int (*compare)(void*, void*);
    // used to free data pointed by the void* in entries.
    void (*free)(Entry*);
    unsigned int length;
    unsigned int occupied;
} HashMap;

HashMap* createMap(int length, unsigned int (*hash)(void*), int(*compare)(void*,void*),void (*free)(Entry)){
    HashMap* ret = malloc(sizeof(HashMap));
    Entry* entries = malloc(sizeof(Entry) * length);
    // sanitizing data, should be faster than calling calloc().
    for (unsigned int i = 0; i < length; i++){
        entries[i].key = NULL;
    }
    ret->entries = entries;
    ret->hashf = hash;
    ret->free = free;
    ret->compare = compare;
    ret->length = length;
    ret->occupied = 0;
    return ret;
}
void rehash(HashMap* map){
	// cannot do in-place, since it will probably collide with things that may move after
	Entry* newE = malloc(sizeof(Entry)* map->length);
	for (unsigned int i = 0; i < map->length; i++){
        	newE[i].key = NULL;
   	}
	for (unsigned int i = 0; i < map->length; i++){
		if (map->entries[i].key != NULL){
           		j = map->hashf(map->entries[i].key) % newL;
            		while (newE[j].key != NULL){
                		if (j == map->length){
                    			j=0;
                		}
                		j++;
            		}
            //printf("key at %d  moved to %d\n",i,j);
            newE[j].key = map->entries[i].key;
            newE[j].value = map->entries[i].value;
       		 }
    	}
	free(map->entries);
	map->entries = newE;
}
void growMap(HashMap* map, unsigned int inc){
    unsigned int newL = map->length+inc;
    Entry* newE = malloc(sizeof(Entry)* newL);
    // sanitizing data, should be faster than calling calloc().
    for (unsigned int i = 0; i < newL; i++){
        newE[i].key = NULL;
    }
    unsigned int j = 0;
    for (unsigned int i = 0; i < map->length; i++){
        if (map->entries[i].key != NULL){
            j = map->hashf(map->entries[i].key) % newL;
            while (newE[j].key != NULL){
                if (j == newL){
                    j=0;
                }
                j++;
            }
            //printf("key at %d  moved to %d\n",i,j);
            newE[j].key = map->entries[i].key;
            newE[j].value = map->entries[i].value;
        }
    }
    free(map->entries);
    map->entries = newE;
    map->length = newL;
}
void addPair(HashMap* map, void* key, void* val){
    // if (map->occupied > 3 / 4 * map->length)
    if (4 * map->occupied > 3 * map->length-1){
        //printf("growing map\n");
        growMap(map, map->length);
    }
    int index = map->hashf(key) % map->length;
    while (map->entries[index].key != NULL && map->entries[index].key != key){
        index++;
        if (index == map->length){
            index = 0;
        }
    }
    //printf("added key at index %d\n", index);
    map->entries[index].key = key;
    map->entries[index].value = val;
    map->occupied++;
}
int removeKey(HashMap* map, void* key){
    int index = map->hashf(key) % map->length;
    int start = index;
    while (map->compare(map->entries[index].key, key) != 0){
        index++;
        if (index == map->length){
            index = 0;
        }
        if (index == start){
            //printf("Invalid or nonexistant key");
            return 1;
        }
    }
    map->occupied--;
    map->free(&map->entries[index]);
    map->entries[index].key = NULL;
    return 0;
}
void* getValue(HashMap* map, void* key){
    int index = map->hashf(key) % map->length;
    int start = index;
    while (map->compare(map->entries[index].key, key) != 0){
        index++;
        if (index == map->length){
            index = 0;
        }
        if (index == start){
						mapErr = MAP_NOSUCH;
            return (void*)NULL;
        }
    }
    return map->entries[index].value;
}
void clearMap(HashMap* map){
    for (unsigned int i = 0; i < map->length; i++){
        if (map->entries[i].key != NULL){
            map->free(&map->entries[i]);
        }
    }
}
void discardMap(HashMap* map){
    for (unsigned int i = 0; i < map->length; i++){
        if (map->entries[i].key != NULL){
            map->free(&map->entries[i]);
        }
    }
    free(map->entries);
    free(map);
}
