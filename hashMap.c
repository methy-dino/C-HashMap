#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
int mapErr = 0;
#define MAP_NOSUCH 1
#define MALLOC_FAIL 2
typedef struct entry {
	void* key;
	void* value;
	struct entry* next;
} Entry;
#define BUILD_ENTRY(keys, vals, ret) { \
	if ((ret = malloc(sizeof(Entry))) != NULL){ \
	(ret)->key = (void*)keys; \
	(ret)->value = (void*)vals; \
	(ret)->next = NULL; \
	} \
}
typedef struct {
    Entry* entries;
    size_t (*hashf)(const void*);
    /* used for comparison between keys, if they are equal, it should return 0, if different, it should be != 0*/
    int (*compare)(const void*, const void*);
    /* used to free data storage.*/    
    void (*free)(void* key, void* val);
    size_t length;
    size_t occupied;
} HashMap;
HashMap* createMap(const size_t length, size_t (*hash)(const void*), int(*compare)(const void*, const void*),void (*freefn)(void*, void*)){
    HashMap* ret;
    Entry* entries;
    size_t i;
		ret = malloc(sizeof(HashMap));
		if (ret == NULL){
			mapErr = MALLOC_FAIL;
			return ret;
		}
		entries = malloc(sizeof(Entry) * length);
		if (entries == NULL){
			mapErr = MALLOC_FAIL;
			free(ret);
			return NULL;
		}
    /* sanitizing data, should be faster than calling calloc().*/
		for (i = 0; i < length; i++){
        entries[i].key = NULL;
        entries[i].next = NULL;
    }
    ret->entries = entries;
    ret->hashf = hash;
    ret->free = freefn;
    ret->compare = compare;
    ret->length = length;
    ret->occupied = 0;
    return ret;
}
int growMap(HashMap* map, const size_t inc){
	size_t i;
	size_t j;
	size_t newL;
	Entry* newE,*ent, *tmp;
	newL = map->length+inc;
	newE = malloc(sizeof(Entry)* newL);
	if (newE == NULL){
		mapErr = MALLOC_FAIL;
		return 1;
	}
	/* sanitizing data, should be faster than calling calloc().*/
	for (i = 0; i < newL; i++){
		newE[i].key = NULL;
		newE[i].next = NULL;
	}
	for (i = 0; i < map->length; i++){
		ent = &map->entries[i];
		if (ent->key != NULL){
			j = map->hashf(ent->key) % newL;
			tmp = &newE[j];
			if (tmp->key != NULL){
				while (tmp->next){
					tmp = tmp->next;
				}
				BUILD_ENTRY(ent->key, ent->value, tmp);
			} else {
				tmp->value = ent->value;
				tmp->key = ent->key;
			}
			tmp->key = ent->key;
			tmp->value = ent->value;
			ent = ent->next;
			while (ent){
				j = map->hashf(tmp->key) % newL;
				tmp = &newE[j];
				if (tmp->key != NULL){
					while (tmp->next){
						tmp = tmp->next;
					}
					/* recycle if not a root node in newE */
					tmp->next = ent;
				} else {
					tmp->value = ent->value;
					tmp->key = ent->key;
					free(ent);
				}
			}
		}
	}
	free(map->entries);
	map->entries = newE;
	map->length = newL;
	return 0;
}
int addPair(HashMap* map, const void* key, const void* val){
	size_t index; 
	Entry* ent;
	if (map->occupied >= (3 * map->length) / 4){
		unsigned char rep;
		rep = 0;
		while (growMap(map, map->length) && rep < 3){
			rep++;
		}
		if (rep == 3){
			return 1;
		} else {
			mapErr = 0;
		}
	}
	index = map->hashf(key) % map->length;
//	printf("%u, %s key %s val\n", index, key, val);
	ent = &map->entries[index];
	if (ent->next || ent->key){
		while (ent->next && map->compare(ent->key, key)){
			ent = ent->next;
		}
		if (map->compare(ent->key, key)){
			BUILD_ENTRY(key, val, ent->next);
			map->occupied++;
		} else {
			ent->value = (void*) val;
			return -1;
		}
	} else {
		ent->key = (void*)key;
		ent->value = (void*)val;
		map->occupied++;
	}
	return 0;
}
int removeKey(HashMap* map, const void* key){
	size_t i;
	Entry* ent, *prev;
	i = map->hashf(key) % map->length;
	ent = &map->entries[i];
	if (map->compare(ent->key, key)){ 
		do {
			prev = ent;
			ent = ent->next;
		} while(ent && map->compare(ent->key, key));
		if (ent){
			map->occupied--;
			if (map->free){
				map->free(ent->key, ent->value);
			} else {
				free(ent->key);
				free(ent->value);
			}
			prev->next = ent->next; 
			free(ent);
		} else {
			mapErr = MAP_NOSUCH;
			return 1;
		}
	} else {
		if (map->free){
			map->free(ent->key, ent->value);
		} else {
			free(ent->key);
			free(ent->value);
		}
		map->occupied--;
		ent->key = NULL;
	}
	return 0;
}
void* getValue(const HashMap* map, const void* key){
  size_t index;
	Entry* curr;
	index = map->hashf(key) % map->length;
	//printf("%u\n", index);
	curr = &map->entries[index];
	while (curr != NULL && curr->key != NULL && map->compare(key, curr->key) != 0){
		curr = curr->next;
	}
	if (curr && curr->key){
	//	printf("returned curr\n");
		return curr->value;
	} else {
//		printf("returned null\n");
		return NULL;
	}
}
size_t hasKey(const HashMap* map, const void* key){
	return getValue(map, key) != NULL;
}
void clearMap(HashMap* map){
  size_t i; 
	Entry* ent, *tmp;
	for (i = 0; i < map->length; i++){
		ent = &map->entries[i];
			if (map->free && ent->key != NULL) {
				map->free(ent->key, ent->value);
			} else if (ent->key != NULL) {
				free(ent->key);
				free(ent->value);
			}
			ent = ent->next;
			while (ent){
				if (map->free){
					map->free(ent->key, ent->value);
				} else {
					free(ent->key);
					free(ent->value);
				}
				tmp = ent;
				ent = ent->next;
				free(tmp);
			}	
		map->entries[i].key = NULL;
	}
	map->occupied = 0;
}
void discardMap(HashMap* map){
	clearMap(map);
	free(map->entries);
	free(map);
}
/* very shitty default hash function that should return on anything.*/
size_t defHash(const void* key){
	return ((char*)key)[0];
}

/* function to hash null terminated strings.*/
size_t strHash(const void* key){
	size_t value;
	value = 0;
	while ((*(char*)key) != '\0'){
		value = ((value << 2) | (*(char*)key)) & 0x1A;
		key++;
	}
	return value;
}
int strcmpWrap(const void* strA, const void* strB){
	if (strB == NULL || strA == NULL){
		return 1;
	}
	return strcmp(strA, strB);
}
void defaultFree(void* key, void* value){
	free(key);
	free(value);
}
/*verbosity indicates what to print.
 * 0 entry print for all entries
 * 1 prints hashmap entry fill rate.
 * 2 also prints hashmap address and entry adress
*/
// TODO: FINISH PRINTER.
void debugPrintMap(const HashMap* map, void (*printEntry)(const Entry*), int verbosity){
	printf("-  -  -  -\n");
	if (verbosity > 1){
		printf("hashmap debug info for map at: %p, which has it's entries at:%p\n", (void*)map, (void*)&map->entries);
	} else {
		printf("hashmap debug info:\n");
	}
	if (verbosity > 0) {
		printf("it has %lu entries, with %lu pairs stored in them.\n", (unsigned long) map->length, (unsigned long) map->occupied);
	}
	if (printEntry != NULL) {
		size_t i;
		size_t decimal;
		printf("this is it's entry data:\n");
		for (i = 0; i < map->length; i++){
			if (map->entries[i].key != NULL){
					Entry* curr = &map->entries[i];
					decimal = 0;
					while (curr != NULL){
						printf("- - - - Entry %lu.%lu - - - -\n", (unsigned long)i, (unsigned long)decimal);
						printEntry(curr);
						curr = curr->next;
						decimal++;
					}
			} else {
        printf("NULL ENTRY\n");        
      }
		}
	}
}
/* there's not much I can do about printing, since there can be near infinite pairs, uh have this generic function.*/
void addressPrint(const Entry* pair){
	printf("key at %p and val at %p", pair->key, pair->value);
}
