
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./dataProcessingReg.h"
#include "../utilities.h"

uint32_t dataProcessingRegAddSub(int opcode, char* rdStr, char* rnStr, char* op2Str) {
    // add(s) x19, x10, x15
    // sub(s) x19, x10, x15
    // add(s) x19, x20, x5, lsr #0x1f
    // sub(s) x19, x20, x4, lsr #0x1f
    
    char* rmStr = malloc(strlen(op2Str) * sizeof(char));
    char* shiftStr = malloc(strlen(op2Str) * sizeof(char));
    char* operandStr = malloc(strlen(op2Str) * sizeof(char));
    if (sscanf(op2Str, "%s, %s %s", rmStr, shiftStr, operandStr) == 1) {
        // idk what shift to put if there is no shift
        // going to put lsl, shift = 0
        shiftStr = "lsl";
        operandStr = "#0";
    }

    int sf, rd, rn, rm, operand;
    parseRegister(rdStr, &sf, &rd);
    parseRegister(rnStr, &sf, &rn);
    parseRegister(rmStr, &sf, &rm);
    operand = parseToInt(operandStr);

    
    int M = 0;
    int opr;
    if (strcmp(shiftStr, "lsl") == 0) {
        opr = 0x8; //0b1000
    } else if (strcmp(shiftStr, "lsr") == 0) {
        opr = 0xA; //0b1010
    } else if (strcmp(shiftStr, "asr") == 0) {
        opr = 0xC; // 0b1100
    }

    uint32_t result = 0;
    result += rd & 0x1f; // rd
    result += (rn & 0x1f) << 5; // rn
    result += (operand & 0x3f) << 10; // operand
    result += (rm & 0x1f) << 16; // rm
    result += (opr & 0xf) << 21; // opr
    result += (5 & 0x7) << 25; // 0b101 const
    result += (M & 0x1) << 28; // M = 0
    result += (opcode & 0x3) << 29; // opc
    result += (sf & 0x1) << 31; // sf

    return result;
}

uint32_t dataProcessingCmpCmn(int opcode, char* instruction) {
    // cmp rn, <op2>
    // opcode will show if it is cmp(subs) or cmn(adds)

    char* cmpStr = malloc(strlen(instruction) * sizeof(char));
    char* rnStr = malloc(strlen(instruction) * sizeof(char));
    char* op2Str = malloc(strlen(instruction) * sizeof(char));
    scanf("%s %s, %s", cmpStr, rnStr, op2Str);

    char* rdStr = malloc(4 * sizeof(char));
    strcpy(rdStr, "rzr");

    uint32_t result = dataProcessingRegAddSub(opcode, rdStr, rnStr, op2Str);

    free(rdStr);
    free(cmpStr);
    free(rnStr);
    free(op2Str);

    return result;
}

uint32_t dataProcessingNeg(int opcode, char* instruction) {
    // neg(s) rd, <op2>
    // opcode will show if it is neg(sub) or negs(subs)

    char* negStr = malloc(strlen(instruction) * sizeof(char));
    char* rdStr = malloc(strlen(instruction) * sizeof(char));
    char* op2Str = malloc(strlen(instruction) * sizeof(char));
    scanf("%s %s, %s", negStr, rdStr, op2Str);

    char* rnStr = malloc(4 * sizeof(char));
    strcpy(rnStr, "rzr");

    uint32_t result = dataProcessingRegAddSub(opcode, rdStr, rnStr, op2Str);

    free(rnStr);
    free(negStr);
    free(rdStr);
    free(op2Str);

    return result;
}


uint32_t dataProcessingRegLogic(int opcode, char* instruction) {
    return 0;
}

uint32_t dataProcessingRegMultiply(int opcode, char* instruction) {
    return 0;
}