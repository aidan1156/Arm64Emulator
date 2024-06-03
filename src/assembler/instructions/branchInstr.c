

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../maps.h"
#include <branchInstr.h>

uint32_t branchInstruction(int opcode, char* instruction, int curaddress, char cond[2], Map* labelmap) {
    int32_t result = 0x14000000;

    if (opcode == 0) { // branch unconditionally
        result = result | 0;
        int simm26 = getMap(labelmap, instruction) - curaddress;
        result = result | simm26;

    } else if (opcode == 1) { // branch conditionally
        result = result | 0x50000000;

        if (cond == "eq") {
            result = result | 0x00000000;
        } else if (cond == "ne") {
            result = result | 0x00000001;
        } else if (cond == "ge") {
            result = result | 0x0000000a;
        } else if (cond == "lt") {
            result = result | 0x0000000b;
        } else if (cond == "gt") {
            result = result | 0x0000000c;
        } else if (cond == "le") {
            result = result | 0x0000000d;
        } else if (cond == "al") {
            result = result | 0x0000000e;
        } 

        int simm19 = getMap(labelmap, instruction) - curaddress;
        result = result | (simm19 << 4);

    } else if (opcode == 2) { // branch register
        result = result | 0xd0000000;
        result = result | (parseRegister(&instruction, labelmap) << 5);
    }
    printf("%x", result);
    return result;
}
