
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./dataProcessingImm.h"
#include "./dataProcessingReg.h"
#include "./dataProcessingArith.h"


uint32_t dataProcessingArithmetic(int opcode, char* instruction) {
    // Imm Examples:
    // add(s) x19, x10, #10
    // add(s) x19, x10, #10 lsl #12

    // Reg Examples:
    // add(s) x19, x10, x15
    // add(s) x19, x20, x5, lsr #0x1f

    char* opcodeStr = malloc(strlen(instruction) * sizeof(char));
    char* rdStr = malloc(strlen(instruction) * sizeof(char));
    char* rnStr = malloc(strlen(instruction) * sizeof(char));
    char* op2Str = malloc(strlen(instruction) * sizeof(char));
    sscanf(instruction, "%s %s %s %s", opcodeStr, rdStr, rnStr, op2Str);

    // freeing unncessary mallocs
    free(opcodeStr);
    free(rdStr);
    free(rnStr);
    
    uint32_t result;
    if (op2Str[0] == '#') {
        result = dataProcessingImmArithmetic(opcode, instruction);
    } else {
        result = dataProcessingRegAddSub(opcode, instruction);
    }

    free(op2Str);
    return result;
}