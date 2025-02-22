#define MAP_NOSUCH 1
int mapErr = 0;
// very shitty default hash function that should return on anything.
unsigned int defHash(void* key){
	return (int) ((char*)key)[0];
}

// function to hash null terminated strings.
unsigned int strHash(void* key){
	unsigned int value;
	unsigned int charSize = sizeof(char);
	charSize = charSize * 8;
	while (((char*)key)[i] != '\0'){
		// the bit shift by charSize is to grant compatibility with other charset, such as UTF-16. 
		value = ((value << charSize) | ((char*)key)[i]) & 65033;
		i++;
	}
	return value;
}

void defaultFree(Entry* entry){
	free(entry->key);
	free(entry->value);
}
