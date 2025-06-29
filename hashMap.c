#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
int mapErr = 0;
#define MAP_NOSUCH 1
#define MALLOC_FAIL 2
typedef struct {
    void* key;
    void* value;
} Entry;
typedef struct node {
	void* key;
	void* value;
	struct node* next;
} node;
node* b_node(const void* key, const void* val){
	node* ret;
	if ((ret = malloc(sizeof(node))) == NULL){
		return ret;
	}
	ret->key = (void*)key;
	ret->value = (void*)val;
	ret->next = NULL;
	return ret;
}
int add_handle(Entry* entry, const void* key, const void* val, int (*cmp)(const void* a, const void*b)){
	node* nd;	
	if (entry->key != NULL){
			if (entry->value){
				if (cmp(key, entry->key) == 0){
					entry->value = (void*)val;
				} else {
					nd = b_node(entry->key, entry->value);
					if (nd == NULL){
						mapErr = MALLOC_FAIL;
						return 1;
					}
					entry->value = NULL;		
					entry->key = nd;
					nd = b_node(key, val);
					if (nd == NULL){
						mapErr = MALLOC_FAIL;
						return 1;
					}
					((node*)entry->key)->next = nd;
					return ((node*)entry->key)->next == NULL;
				}
			} else {
				node* curr;
				int result;
				curr = entry->key;
				while ((result = cmp(key, curr->key)) != 0 && curr->next){
					curr = curr->next;
				}
				if (result == 0){
					curr->value = (void*)val;
				} else {
					curr->next = b_node(key, val);
					return curr->next == NULL;
				}
			}
		} else {
			entry->key = (void*)key;
			entry->value = (void*)val;
		}
	return 0;
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
    Entry* newE;
		newL = map->length+inc;
		newE = malloc(sizeof(Entry)* newL);
		if (newE == NULL){
			mapErr = MALLOC_FAIL;
			return 1;
		}
    /* sanitizing data, should be faster than calling calloc().*/
		for (i = 0; i < newL; i++){
        newE[i].key = NULL;
    }
    for (i = 0; i < map->length; i++){
			if (map->entries[i].key != NULL){
				if (map->entries[i].value == NULL){
					node* curr = map->entries[i].key;
					node* tmp = NULL;
					while (curr){
						j = map->hashf(curr->key) % newL;
						add_handle(&newE[j], curr->key, curr->value, map->compare);
						tmp = curr;
						curr = curr->next;
						free(tmp);
					}
				} else {
					j = map->hashf(map->entries[i].key) % newL;
					add_handle(&newE[j], map->entries[i].key, map->entries[i].value, map->compare);
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
		index = add_handle(&map->entries[index], key, val, map->compare); 
    map->occupied++;
		return index;
}
int removeKey(HashMap* map, const void* key){
	size_t i;
	i = map->hashf(key) % map->length;
	if (map->entries[i].key != NULL){
		if (map->entries[i].value){
			if (map->compare(key, map->entries[i].key) == 0){
				map->free(map->entries[i].key, map->entries[i].value);
				map->entries[i].value = NULL;
				map->entries[i].key = NULL;
				map->occupied--;
			} else {
				return 1;
			}
		} else {
			node* curr;
			node* prev;
			int result;
			curr = map->entries[i].key;
			prev = NULL;
			while ((result = map->compare(key, curr->key)) != 0 && curr->next){
				prev = curr;
				curr = curr->next;
			}
			if (result == 0){
				if (prev) {
				prev->next = curr->next;
				} else {
					if (curr->next){
						map->entries[i].key = curr->next;
					} else {
						map->entries[i].value = curr->value;
						map->entries[i].key = curr->key;
					}
				}
				map->free(curr->key, curr->value);
				free(curr);
				map->occupied--;
			} else {
				return 1;
			}
		}
	} else {
		return 1;
	}    
	return 0;
}
void* getValue(const HashMap* map, const void* key){
  size_t index;
	index = map->hashf(key) % map->length;
	if (map->entries[index].key == NULL){
		return NULL;
	}
	if (map->entries[index].value == NULL){
		node* curr = map->entries[index].key;
		while (curr && map->compare(key, curr->key) != 0){
			curr = curr->next;
		}
		if (curr){
			return curr->value;
		} else {
			return NULL;
		}
	}
  return map->entries[index].value;
}
size_t hasKey(const HashMap* map, const void* key){
	return getValue(map, key) != NULL;
}
void clearMap(HashMap* map){
  size_t i;  
	for (i = 0; i < map->length; i++){
		if (map->entries[i].key != NULL){
			if (map->entries[i].value == NULL){
				node* curr = map->entries[i].key;
				node* tmp = NULL;
					while (curr){
						map->free(curr->key, curr->value);
						tmp = curr;
						curr = curr->next;
						free(tmp);
					}	
			} else {
				map->free(map->entries[i].key, map->entries[i].value);
			}
		}
		map->entries[i].key = NULL;
		map->entries[i].value = NULL;
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
	size_t charSize;
  size_t i;
	i = 0;
	charSize = sizeof(char) * 8;
	while (((char*)key)[i] != '\0'){
		/* the bit shift by charSize is to grant compatibility with other charset, such as UTF-16.*/ 
		value = ((value << charSize) | ((char*)key)[i]) & 65033;
		i++;
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
void debugPrintMap(const HashMap* map, void (*printEntry)(const Entry*), int verbosity){
	printf("-  -  -  -\n");
	if (verbosity > 1){
		printf("hashmap debug info for map at: %p, which has it's entries at:%p\n", (void*)map, (void*)&map->entries);
	} else {
		printf("hashmap debug info:\n");
	}
	if (verbosity > 0) {
		printf("it has %lu entries, with %lu pairs stored in them.\n", (unsigned long) map->length, (unsigned long)map->occupied);
	}
	if (printEntry != NULL) {
		size_t i;
		size_t decimal;
		printf("this is it's entry data:\n");
		for (i = 0; i < map->length; i++){
			if (map->entries[i].key != NULL){
				if (map->entries[i].value == NULL){
					node* curr = map->entries[i].key;
					decimal = 1;
					while (curr){
						printf("- - - - Entry %lu.%lu - - - -\n", (unsigned long)i, (unsigned long)decimal);
						printEntry((Entry*)curr);
						curr = curr->next;
						decimal++;
					}
				} else {
					printf("- - - - Entry %lu.0 - - - -\n", (unsigned long)i);
					printEntry(&map->entries[i]);
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
