# C-HashMap
A simple hashmap for the C language.
## How to use.
The hashmap is pretty simple, you need to Call ```createMap```, and supply functions for hashing, comparison and freeing your data structure memory.\
The built-in functions will handle the freeing, growing, rehashing by using your function.
### function documentation

### hashMap.c

#### createMap(int length, unsigned int (*hash)(void*), int(*compare)(void*,void*), void (*free)(Entry))
returns a HashMap* equipped with the specified capacity and functions.

#### rehash(HashMap* map)
this function is used to redo the hashes on a map, it is not called automatically.

#### growMap(HashMap* map, unsigned int inc)
this function grows the given map by the increment.

#### int addPair(HashMap* map, void* key, void* val)
this function adds a pair to a map, taking a key, and a value, returning 1 if the key was already in the map, returning 0 otherwise.

#### int removeKey(HashMap* map, void* key)
removes the specified key, freeing the memory using the void(*free)(Entry) function and flagging the key pointer of it's entry as NULL, permitting reassigning to the slot.\
returns 0 if key was deleted, and returns 1 if an equal key was not found using int(compare)(void*,void*).
#### void* getValue(HashMap* map, void* key)
returns the value associated with the key (as a void*).\
if an equal key was not found with int(compare)(void*,void*), returns (void*) NULL, and sets mapErr to MAP_NOSUCH.
#### void clearMap(HashMap* map)
frees all the entries of a HashMap, but keeps the map allocated.

#### discardMap(HashMap* map)
frees all memory in the map, including all assigned key value pairs.frees all memory in the map, including all assigned key value pairs.

### defaults.c

#### int mapErr
global variable to handle map errors.

#### unsigned int defHash(void* key)
very shitty default hash function that should return on anything.

#### unsigned int strHash(void* key)
function to hash null terminated strings.

#### void defaultFree(Entry* entry)
default free implementation
