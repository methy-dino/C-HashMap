#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
int mapErr = 0;
#define MAP_NOSUCH 1
typedef struct {
    void* key;
    void* value;
} Entry;
typedef struct {
    Entry* entries;
    unsigned int (*hashf)(void*);
    // used for comparison between keys, if they are equal, it should return 0, if different, it should be != 0;
    int (*compare)(void*, void*);
    // used to free data storage.    
    void (*free)(Entry*);
    unsigned int length;
    unsigned int occupied;
} HashMap;
HashMap* createMap(int length, unsigned int (*hash)(void*), int(*compare)(void*,void*),void (*free)(Entry*));
void rehash(HashMap* map);
void growMap(HashMap* map, unsigned int inc);
void addPair(HashMap* map, void* key, void* val);
unsigned int hasKey(HashMap* map, void* key);
unsigned int removeKey(HashMap* map, void* key);
void* getValue(HashMap* map, void* key);
void clearMap(HashMap* map);
void discardMap(HashMap* map);
// very shitty default hash function that should return on anything.
unsigned int defHash(void* key);
// function to hash null terminated strings.
unsigned int strHash(void* key);
void defaultFree(Entry* entry);
void debugPrintMap(HashMap* map, (void)(printEntry)(Entry*), int verbosity);

