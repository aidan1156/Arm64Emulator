
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct Map {
    uint64_t* values;
    char** keys;
    int maxSize;
} Map;


struct Map* createMap(int initialSize);

void resizeMap(struct Map* map);

void insertMap(struct Map* map, char* key, uint64_t value);

uint64_t getMap(struct Map* map, char* key);

void printMap(struct Map* map);


