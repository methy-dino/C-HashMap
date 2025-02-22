#define MAP_NOSUCH 1
int mapErr = 0;
// very shitty default hash function that should return on anything.
unsigned int defHash(void* key);
// function to hash null terminated strings.
unsigned int strHash(void* key);
// default free implementation
void defaultFree(Entry* entry);
