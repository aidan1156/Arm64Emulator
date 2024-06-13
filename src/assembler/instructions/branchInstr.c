
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
        // printf("in uncond");
        result = result | 0;
        sscanf(instruction, " b %s", simm26str);
        int simm26 = (getMap(labelmap, simm26str) - curaddress) / 4;
        // printf("simm26: %d", simm26);
        result = result | simm26;

    } else if (opcode == 1) { // branch conditionally
        result = result | 0x50000000;
        // printf("in cond: %s", cond);
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

        sscanf(instruction, "%s %s", branchstr, simm19str);
        int simm19 = ((getMap(labelmap, simm19str) - curaddress) / 4);
        result = result | (simm19 << 5);

    } else if (opcode == 2) { // branch register
        // printf("in reg");
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
