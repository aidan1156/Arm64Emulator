
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./assembler/utilities.h"
#include "./assembler/fileIO.h"

#include "./assembler/instructions/sdt.h"
#include "./assembler/instructions/dataProcessingImm.h"
#include "./assembler/instructions/dataProcessingReg.h"
#include "./assembler/instructions/dataProcessingArith.h"
#include "./assembler/instructions/branchInstr.h"


// find all the labels in the program and map them to their respecitve address
void findLabels(Map* map, char* path) {
    FILE *file = fopen(path, "rb");
    char* currentInstr = readLine(file);
    int currentAddress = 0;
    while (currentInstr != NULL) {
        if (isLabel(currentInstr)) {
            int length = strlen(currentInstr);
            // remove \n
            while (length > 0 && isspace(currentInstr[length - 1])) {
                length--;
            }
            // remove the ':' so we have just the label
            currentInstr[length - 1] = '\0';
            insertMap(map, currentInstr, currentAddress);
        } else if (strcmp(currentInstr, "\n") != 0 && strcmp(currentInstr, "") != 0) { // otherwise increment the address
            currentAddress += 4;
        }
        currentInstr = readLine(file);
    }
    fclose(file);
}

uint32_t assembleInstruction(char* opcode, char* instruction, Map* labelmap, uint64_t address) {
    uint32_t result;
    if (strcmp(opcode, "add") == 0) {
        result = dataProcessingArithmetic(0, instruction);
    } else if (strcmp(opcode, "adds") == 0) {
        result = dataProcessingArithmetic(1, instruction);
    } else if (strcmp(opcode, "sub") == 0) {
        result = dataProcessingArithmetic(2, instruction);
    } else if (strcmp(opcode, "subs") == 0) {
        result = dataProcessingArithmetic(3, instruction);
    } else if (strcmp(opcode, "ldr") == 0) {
        result = singleDataTransfer(1, instruction, address, labelmap);
    } else if (strcmp(opcode, "str") == 0) {
        result = singleDataTransfer(0, instruction, address, labelmap);
    } else if (strcmp(opcode, "cmp") == 0) {        
        result = dataProcessingCmpCmn(3, instruction);
    } else if (strcmp(opcode, "cmn") == 0) {
        result = dataProcessingCmpCmn(1, instruction);
    } else if (strcmp(opcode, "neg") == 0) {
        result = dataProcessingNeg(2, instruction);
    } else if (strcmp(opcode, "negs") == 0) {
        result = dataProcessingNeg(3, instruction);
    } else if (strcmp(opcode, "and") == 0) {
        result = dataProcessingRegLogic(0, instruction, 0);
    } else if (strcmp(opcode, "bic") == 0) {
        result = dataProcessingRegLogic(0, instruction, 1);
    } else if (strcmp(opcode, "orr") == 0) {
        result = dataProcessingRegLogic(1, instruction, 0);
    } else if (strcmp(opcode, "orn") == 0) {
        result = dataProcessingRegLogic(1, instruction, 1);
    } else if (strcmp(opcode, "eor") == 0) {
        result = dataProcessingRegLogic(2, instruction, 0);
    } else if (strcmp(opcode, "eon") == 0) {
        result = dataProcessingRegLogic(2, instruction, 1);
    } else if (strcmp(opcode, "ands") == 0) {
        result = dataProcessingRegLogic(3, instruction, 0);
    } else if (strcmp(opcode, "bics") == 0) {
        result = dataProcessingRegLogic(3, instruction, 1);
    } else if (strcmp(opcode, "tst") == 0) {
        result = dataProcessingTstMvnMov(instruction);
    } else if (strcmp(opcode, "mvn") == 0) {
        result = dataProcessingTstMvnMov(instruction);
    } else if (strcmp(opcode, "mov") == 0) {
        result = dataProcessingTstMvnMov(instruction);
    } else if (strcmp(opcode, "madd") == 0) {
        result = dataProcessingRegMultiply(0, instruction);
    } else if (strcmp(opcode, "msub") == 0) {
        result = dataProcessingRegMultiply(1, instruction);
    } else if (strcmp(opcode, "mul") == 0) {
        result = dataProcessingRegMultiply(0, instruction);
    } else if (strcmp(opcode, "mneg") == 0) {
        result = dataProcessingRegMultiply(1, instruction);
    } else if (strcmp(opcode, "movn") == 0) {
        result = wideMoveInstruction(0, instruction);
    } else if (strcmp(opcode, "movk") == 0) {
        result = wideMoveInstruction(1, instruction);
    } else if (strcmp(opcode, "movz") == 0) {
        result = wideMoveInstruction(2, instruction);
    } else if (strcmp(opcode, "b") == 0) {
        result = branchInstruction(0, instruction, address, NULL, labelmap);
    } else if (strncmp(opcode, "b.", 2) == 0) {
        char cond[2];
        strncpy(cond, opcode + 2, 2);
        result = branchInstruction(1, instruction, address, cond, labelmap);
    } else if (strcmp(opcode, "br") == 0) {
        result = branchInstruction(2, instruction, address, NULL, labelmap);
    } else {
        fprintf(stderr, "unknown opcode\n");
    }

    return result;
}


int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "too few arguments supplied\n");
        exit(1);
    }

    // open the input and output file
    FILE* inputFile = fopen(argv[1], "rb");
    assert(inputFile != NULL);
    FILE* outputFile = fopen(argv[2], "wb");
    assert(outputFile != NULL);

    // find the address of each label and store it to a map
    Map* labelMap = createMap(64);
    findLabels(labelMap, argv[1]);

    char* instruction = readLine(inputFile);
    int address = 0;
    uint32_t binaryInstruction;

    while (instruction != NULL) {        
        if (!isLabel(instruction)) {
            if (isIntDirective(instruction)) {
                // + 1 to include terminator
                char* number = malloc(strlen(instruction) + 1);
                sscanf(instruction, " .int %s", number);
                binaryInstruction = parseToInt(number);
                free(number);
            } else if (strcmp(instruction, "\n") == 0){
                free(instruction);
                instruction = readLine(inputFile);
                continue;
            } else {
                char* opcode = malloc(strlen(instruction) + 1);
                sscanf(instruction, "%s", opcode);

                // format instruction to be used with sscanf
                char* instructionReform = malloc((strlen(instruction) + 1));
                replaceCommaWithSpace(instruction, instructionReform);
                
                binaryInstruction = assembleInstruction(opcode, instructionReform, labelMap, address);

                free(opcode);
                free(instructionReform);
            }
            if (strcmp(instruction, "\n") != 0 && strcmp(instruction, "")) {
                address += 4;
            }

            writeInstruction(outputFile, binaryInstruction);
        }

        free(instruction);
        instruction = readLine(inputFile);

    }

    freeMap(labelMap);
    free(instruction);

    fclose(inputFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}
