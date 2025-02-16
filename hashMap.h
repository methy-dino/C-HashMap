#include <stdlib.h>
#include <stdio.h>
typedef struct {
    void* key;
    void* value;
} Entry;
typedef struct {
    Entry* entries;
    unsigned int (*hashf)(void*);
    // used for comparison between keys, aka are they the same keys?
    int (*compare)(void*, void*);
    // used to free data storage.    
    void (*free)(Entry);
    unsigned int length;
    unsigned int occupied;
} HashMap;
HashMap* createMap(int length, unsigned int (*hash)(void*), int(*compare)(void*,void*), void (*free)(Entry));
void rehash(HashMap* map);
void growMap(HashMap* map, unsigned int inc);
/*assumes heap allocation of contents.
*data WILL most likely CORRUPT if it is allocated on the stack, and used after the scope it was introduced in ends.
*/
void addPair(HashMap* map, void* key, void* val);
// returns 0 if it fails, returns 1 if it doesn't.
int removeKey(HashMap* map, void* key);
/*returns the value associated with the key (as a void*).
*if there is no such key, returns (void*) NULL*/
void* getValue(HashMap* map, void* key);
void clearMap(HashMap* map);
/*frees all memory in the map, including all assigned key value pairs.*/
void discardMap(HashMap* map);
