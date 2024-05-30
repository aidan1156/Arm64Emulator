
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "./assembler/maps.h"


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


// find all the labels in the program and map them to their respecitve address
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

// if the instruction is a label
bool isLabel(char* instruction) {
    return instruction[strlen(instruction) - 1] == ':';
}

// if the instruction represents a .int directive
bool isIntDirective(char* instruction) {
    return instruction[0] == '.';
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "too few arguments supplied\n");
    }
    // find the address of each label
    Map* map = createMap(64);
    findLabels(map, argv[1]);

    // open the input file
    FILE* inputFile = fopen(argv[1], "r");
    FILE* outputFile = fopen(argv[2], "w");

    char* instruction = readLine(inputFile);
    int address = 0;
    uint64_t binaryInstruction;
    while (instruction != NULL) {
        if (!isLabel(instruction)) {
            if (isIntDirective(instruction)) {
                binaryInstruction = 0xbabe0bee;
            } else {
                char* opcode = malloc(strlen(instruction) + 1);
                sscanf(instruction, "%s", opcode);
                printf("%s\n", opcode);
                if (!strcmp(opcode, "cmp")) {
                    binaryInstruction = 0x12345678;
                } else {
                    binaryInstruction = 0x90abcdef;
                }
            }
            address += 4;

            writeInstruction(outputFile, binaryInstruction);
        }

        free(instruction);
        instruction = readLine(inputFile);
    }

    free(instruction);

    fclose(inputFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}
