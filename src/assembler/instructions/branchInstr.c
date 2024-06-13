
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utilities.h"
#include "../maps.h"
#include "./branchInstr.h"

uint32_t branchInstruction(int opcode, char* instruction, int curaddress, char cond[2], Map* labelmap) {
    int32_t result = 0x14000000;
    int maxStrLength = (strlen(instruction) + 1) * sizeof(char);
    char* branchstr = malloc(maxStrLength);
    char* simm19str = malloc(maxStrLength);
    char* simm26str = malloc(maxStrLength);


    if (opcode == 0) { // branch unconditionally
        result = result | 0;

        char* branchName = malloc(strlen(instruction) * sizeof(char));
        sscanf(instruction, " b %s", branchName);
        int simm26 = (getMap(labelmap, branchName) - curaddress) / 4;
        simm26 = simm26 & 0x03ffffff;
        free(branchName);

        result = result | simm26;

    } else if (opcode == 1) { // branch conditionally
        result = result | 0x50000000;
        //convert the condition
        if (strcmp(cond, "eq") == 0) {
            result = result | 0x00000000;
        } else if (strcmp(cond, "ne") == 0) {
            result = result | 0x00000001;
        } else if (strcmp(cond, "ge") == 0) {
            result = result | 0x0000000a;
        } else if (strcmp(cond, "lt") == 0) {
            result = result | 0x0000000b;
        } else if (strcmp(cond, "gt") == 0) {
            result = result | 0x0000000c;
        } else if (strcmp(cond, "le") == 0) {
            result = result | 0x0000000d;
        } else if (strcmp(cond, "al") == 0) {
            result = result | 0x0000000e;
        } 

        
        char* branchName = malloc(strlen(instruction) * sizeof(char));
        char* temp = malloc(strlen(instruction) * sizeof(char));
        sscanf(instruction, " b.%s %s", temp, branchName);
        int simm19 = (getMap(labelmap, branchName) - curaddress) / 4;
        
        simm19 = simm19 & 0x0007ffff;
        free(temp);
        free(branchName);
        
        result = result | (simm19 << 5);

    } else if (opcode == 2) { // branch register
        result = result | 0xd61f0000;
        int* reg = malloc(5);
        int* sf = malloc(1);
        char regStr[4];

        sscanf(instruction, " br %s", regStr);

        parseRegister(regStr, sf, reg);
        result = result | (*reg << 5);
        free(reg);
        free(sf);
    }

    free(branchstr);
    free(simm19str);
    free(simm26str);

    return result;
}
