
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

    if (opcode == 0) { // branch unconditionally
        printf("in uncond");
        result = result | 0;
        int simm26 = (getMap(labelmap, &instruction[2]) - curaddress) / 4;
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

        int simm19 = (getMap(labelmap, &instruction[5]) - curaddress) / 4;
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

    return result;
}
