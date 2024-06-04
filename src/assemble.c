
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./assembler/utilities.h"
#include "./assembler/fileIO.h"

#include "./assembler/instructions/dataProcessingImm.h"
#include "./assembler/instructions/dataProcessingReg.h"
#include "./assembler/instructions/dataProcessingArith.h"


// find all the labels in the program and map them to their respecitve address
void findLabels(Map* map, char* path) {
    FILE *file = fopen(path, "rb");
    char* currentInstr = readLine(file);
    int currentAddress = 0;
    while (currentInstr != NULL) {
        // if the final character in the instruction is a ':' then its a label
        if (currentInstr[strlen(currentInstr) - 1] == ':') {
            // remove the ':' so we have just the label
            currentInstr[strlen(currentInstr) - 1] = '\0';
            insertMap(map, currentInstr, currentAddress);
        } else if (strcmp(currentInstr, "")) { // otherwise increment the address
            currentAddress += 4;
        }
        currentInstr = readLine(file);
    }

    fclose(file);
}

uint32_t assembleInstruction(char* opcode, char* instruction, uint64_t address) {
    uint32_t result;
    if (strcmp(opcode, "add") == 0) {
        result = dataProcessingArithmetic(0, instruction);
    } else if (strcmp(opcode, "adds") == 0) {
        result = dataProcessingArithmetic(1, instruction);
    } else if (strcmp(opcode, "sub") == 0) {
        result = dataProcessingArithmetic(2, instruction);
    } else if (strcmp(opcode, "subs") == 0) {
        result = dataProcessingArithmetic(3, instruction);
    } else if (strcmp(opcode, "cmp") == 0) {
        result = dataProcessingCmpCmn(3, instruction);
    } else if (strcmp(opcode, "cmn") == 0) {
        result = dataProcessingCmpCmn(1, instruction);
    } else if (strcmp(opcode, "neg") == 0) {
        result = dataProcessingNeg(2, instruction);
    } else if (strcmp(opcode, "negs") == 0) {
        result = dataProcessingNeg(3, instruction);
    } else {
        fprintf(stderr, "unknown opcode\n");
    }

    return result;
}


int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "too few arguments supplied\n");
    }
    // find the address of each label and store it to a map
    Map* map = createMap(64);
    findLabels(map, argv[1]);

    // open the input and output file
    FILE* inputFile = fopen(argv[1], "rb");
    FILE* outputFile = fopen(argv[2], "wb");

    char* instruction = readLine(inputFile);
    int address = 0;
    uint32_t binaryInstruction;
    while (instruction != NULL) {
        if (!isLabel(instruction)) {
            if (isIntDirective(instruction)) {
                char* number = malloc(strlen(instruction) + 1);
                sscanf(instruction, ".int %s", number);
                binaryInstruction = parseToInt(number);
                free(number);
            } else {
                char* opcode = malloc(strlen(instruction) + 1);
                sscanf(instruction, "%s", opcode);
                printf("%s\n", opcode);
                
                binaryInstruction = assembleInstruction(opcode, instruction, address);

                free(opcode);
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
