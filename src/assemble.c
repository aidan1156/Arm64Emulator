#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./assembler/maps.h"

void findLabels(Map* map, char* path) {
    FILE *file = fopen(path, "r");

    int c = fgetc(file);
    int maxSize = 50; // max nume of elements the buffer can hold, do not do less than 3!!!!!
    char* currentInstr = malloc(maxSize * sizeof(char)); // we dont know the max length of a label or line so malloc it
    currentInstr[0] = '\0';
    int currentAddress = 0;
    while (c != EOF) {
        if (c == '\n') {
            // if the final character in the instruction is a ':' then its a label
            if (currentInstr[strlen(currentInstr) - 1] == ':') {
                // remove the ':' so we have just the label
                currentInstr[strlen(currentInstr) - 1] = '\0';
                // copy the label into something that wont get overwritten
                char* label = malloc((strlen(currentInstr) + 1) * sizeof(char));
                strcpy(label, currentInstr);
                insertMap(map, label, currentAddress + 4);
            } else { // otherwise increment the address
                currentAddress += 4;
            }
            currentInstr[0] = '\0'; // clear the buffer
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
}

int main(int argc, char **argv) {
    // find the address
    Map* map = createMap(64);
    findLabels(map, argv[1]);
    printMap(map);

    return EXIT_SUCCESS;
}
