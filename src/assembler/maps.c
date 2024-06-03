
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./maps.h"

#ifndef MAPS_C
#define MAPS_C


// creates a dictionary/map like data structure
struct Map* createMap(int initialSize) {
    struct Map* result = malloc(sizeof(struct Map));
    result -> maxSize = initialSize;
    result -> keys = malloc(result -> maxSize * sizeof(char*));
    result -> values = malloc(result -> maxSize * sizeof(uint64_t));

    for (int i=0; i<result -> maxSize; i++) {
        result -> keys[i] = NULL;
    }

    return result;
}

void resizeMap(Map* map) {
    int newSize = (map -> maxSize) * 2;

    char** newKeys = malloc(newSize * sizeof(char*));
    uint64_t* newValues = malloc(newSize * sizeof(uint64_t));

    for (int i=0; i<newSize; i++) {
        if (i < map -> maxSize) {
            newKeys[i] = map -> keys[i];
            newValues[i] = map -> values[i];
        } else {
            newKeys[i] = NULL;
        }
    }

    free(map -> keys);
    free(map -> values);

    map -> maxSize = newSize;
    map -> keys = newKeys;
    map -> values = newValues;
}

void insertMap(Map* map, char* key, uint64_t value) {
    for (int i=0; i<map -> maxSize; i++) {
        if (map -> keys[i] == NULL || !strcmp(map -> keys[i], key)) {
            map -> values[i] = value;
            map -> keys[i] = key;
            return;
        }
    }

    resizeMap(map);
    insertMap(map, key, value);
}

uint64_t getMap(Map* map, char* key) {
    for (int i=0; i<map -> maxSize; i++) {
        if (map -> keys[i] == NULL) {
            return -1;
        } else if (!strcmp(map -> keys[i], key)) {
            return map -> values[i];
        }
    }
    return -1;
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


#endif