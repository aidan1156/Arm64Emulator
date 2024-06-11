
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "./maps.h"

#ifndef MAPS_C
#define MAPS_C


// creates a dictionary/map like data structure
struct Map* createMap(int initialSize) {
    // allocate memory for the map structure
    struct Map* result = malloc(sizeof(struct Map));
    result -> maxSize = initialSize;
    result -> keys = malloc(result -> maxSize * sizeof(char*));
    assert(result -> keys != NULL);
    result -> values = malloc(result -> maxSize * sizeof(uint64_t));
    assert(result -> values != NULL);

    // initialise all keys to null
    for (int i=0; i<result -> maxSize; i++) {
        result -> keys[i] = NULL;
    }

    return result;
}

void resizeMap(Map* map) {
    int newSize = (map -> maxSize) * 2;

    char** newKeys = malloc(newSize * sizeof(char*));
    uint64_t* newValues = malloc(newSize * sizeof(uint64_t));

    // copy existing keys and values to new array
    for (int i=0; i<newSize; i++) {
        if (i < map -> maxSize) {
            newKeys[i] = map -> keys[i];
            newValues[i] = map -> values[i];
        } else {
            newKeys[i] = NULL;
        }
    }

    // free old keys and values array
    free(map -> keys);
    free(map -> values);

    map -> maxSize = newSize;
    map -> keys = newKeys;
    map -> values = newValues;
}

void insertMap(Map* map, char* key, uint64_t value) {
    for (int i=0; i<map -> maxSize; i++) {
        // key if key is empty or key matches
        if (map -> keys[i] == NULL || !strcmp(map -> keys[i], key)) {
            map -> values[i] = value;
            map -> keys[i] = key;
            return;
        }
    }

    // resize if no empty slot was found
    resizeMap(map);
    insertMap(map, key, value);
}

uint64_t getMap(Map* map, char* key) {
    for (int i=0; i<map -> maxSize; i++) {
        if (map -> keys[i] == NULL) {
            return -1; // no value
        } else if (!strcmp(map -> keys[i], key)) {
            return map -> values[i];
        }
    }
    return -1; // key not found
}

void printMap(Map* map) {
    printf("{\n");
    for (int i=0; i<map -> maxSize; i++) {
        if (map -> keys[i] != NULL) {
            printf("    %s: %ld,\n", map -> keys[i], map -> values[i]);
        }
    }
    printf("}\n");
}

void freeMap(Map *map) {
    assert(map != NULL);
    free(map->values);
    free(map->keys);
    free(map);
}


#endif