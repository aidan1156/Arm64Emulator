#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./assembler/maps.h"


char* readLine(FILE *file) {
    int maxSize = 50; // max nume of elements the buffer can hold, do not do less than 3!!!!!
    char* currentInstr = malloc(maxSize * sizeof(char)); // we dont know the max length of a label or line so malloc it
    currentInstr[0] = '\0';

    int c = fgetc(file);
    while (c != EOF) {
        if (c == '\n') {
            return currentInstr;
        } else {
            // the instruction is FAT so make our buffer fatter
            if (strlen(currentInstr) + 1 >= maxSize) {
                maxSize *= 2;
                char* tmp = malloc(maxSize * sizeof(char));
                strcpy(tmp, currentInstr);
                free(currentInstr);
                currentInstr = tmp;
            }

            // add the character to the buffer
            currentInstr[strlen(currentInstr) + 1] = '\0';
            currentInstr[strlen(currentInstr)] = c;
        }
        c = fgetc(file);
    }

    return NULL;
}


void findLabels(Map* map, char* path) {
    FILE *file = fopen(path, "r");
    char* currentInstr = readLine(file);
    int currentAddress = 0;
    while (currentInstr != NULL) {
        // if the final character in the instruction is a ':' then its a label
        if (currentInstr[strlen(currentInstr) - 1] == ':') {
            // remove the ':' so we have just the label
            currentInstr[strlen(currentInstr) - 1] = '\0';
            insertMap(map, currentInstr, currentAddress + 4);
        } else if (strcmp(currentInstr, "")) { // otherwise increment the address
            currentAddress += 4;
        }
        currentInstr = readLine(file);
    }

    fclose(file);
}


int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "too few arguments supplied\n");
    }
    // find the address
    Map* map = createMap(64);
    findLabels(map, argv[1]);
    printMap(map);

    return EXIT_SUCCESS;
}
