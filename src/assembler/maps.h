
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MAPS_H
#define MAPS_H

typedef struct Map {
    uint64_t* values;
    char** keys;
    int maxSize;
} Map;


struct Map* createMap(int initialSize);

void resizeMap(Map* map);

void insertMap(Map* map, char* key, uint64_t value);

uint64_t getMap(Map* map, char* key);

void printMap(Map* map);

#endif
