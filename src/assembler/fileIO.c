
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


// read one line from a file
// NOTE: result must be free()'d once it is no longer needed
char* readLine(FILE *file) {
    int maxSize = 50; // max nume of elements the buffer can hold, do not do less than 3!!!!!
    char* currentInstr = malloc(maxSize * sizeof(char)); // we dont know the max length of a label or line so malloc it
    currentInstr[0] = '\0';

    int c = fgetc(file);
    while (c != EOF) {
        if (c == '\n' && currentInstr[0] != '\0') {
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

// write an encoded binary instruction to the output file
void writeInstruction(FILE* file, uint64_t instruction) {
    for (int i=0; i<4; i++) {
        fputc((instruction >> (i * 8)) & 0xff, file);
    }
}