
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./dataProcessingReg.h"
#include "../utilities.h"

int shiftOpc(char* shiftStr) {
    int opc;
    if (strcmp(shiftStr, "lsl") == 0) {
        opc = 0x8; //0b1000
    } else if (strcmp(shiftStr, "lsr") == 0) {
        opc = 0xA; //0b1010
    } else if (strcmp(shiftStr, "asr") == 0) {
        opc = 0xC; // 0b1100
    } else if (strcmp(shiftStr, "ror") == 0) {
        opc = 0xE; // 0b1110
    }
    return opc;
}

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
    int opr = (shiftOpc(shiftStr) << 1) | 0x8;

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

    // to find sf
    int sf, rn;
    parseRegister(rnStr, &sf, &rn);

    char* rdStr = malloc(4 * sizeof(char));
    if (sf == 1) {
        strcpy(rdStr, "xzr");
    } else {
        strcpy(rdStr, "wzr");
    }

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

    // to find sf
    int sf, rd;
    parseRegister(rdStr, &sf, &rd);

    char* rnStr = malloc(4 * sizeof(char));
    if (sf == 1) {
        strcpy(rnStr, "xzr");
    } else {
        strcpy(rnStr, "wzr");
    }

    uint32_t result = dataProcessingRegAddSub(opcode, rdStr, rnStr, op2Str);

    free(rnStr);
    free(negStr);
    free(rdStr);
    free(op2Str);

    return result;
}


uint32_t dataProcessingRegLogic(int opcode, char* instruction, int N) {
    char* logicStr = malloc(strlen(instruction) * sizeof(char));
    char* rdStr = malloc(strlen(instruction) * sizeof(char));
    char* rnStr = malloc(strlen(instruction) * sizeof(char));
    char* op2Str = malloc(strlen(instruction) * sizeof(char));

    scanf("%s %s, %s, %s", logicStr, rdStr, rnStr, op2Str);
    
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
    int opr = (shiftOpr(shiftStr) << 1) | N;


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

    free(logicStr);
    free(rdStr);
    free(rnStr);
    free(op2Str);
    free(rmStr);
    free(shiftStr);
    free(operandStr);

    return result;
}

uint32_t dataProcessingTstMvnMov(char* instruction) {
    // tst rn, <op2>   ==   ands rzr, rn, <op2>
    // mvn rd, <op2>   ==   orn rd, rzr, <op2>
    // mov rd, rm      ==   orr rd, rzr, <op2>
    char* logicStr = malloc(strlen(instruction) * sizeof(char));
    char* op1Str = malloc(strlen(instruction) * sizeof(char));
    char* op2Str = malloc(strlen(instruction) * sizeof(char));
    scanf("%s %s, %s", logicStr, op1Str, op2Str);

    char* newInstruction = malloc((strlen(instruction) + 10) * sizeof(char));

    // to find sf
    int sf, op1;
    parseRegister(op1Str, &sf, &op1);

    char* zrStr = malloc(4 * sizeof(char));
    if (sf == 1) {
        strcpy(zrStr, "xzr");
    } else {
        strcpy(zrStr, "wzr");
    }

    int opcode, N;

    if (strcmp(logicStr, "tst") == 0) {
        strcat(newInstruction, "ands ");
        strcat(newInstruction, zrStr);
        strcat(newInstruction, ", ");
        strcat(newInstruction, op1Str);
        strcat(newInstruction, ", ");
        strcat(newInstruction, op2Str);
        opcode = 3;
        N = 0;

    } else if (strcmp(logicStr, "mvn") == 0) {
        strcat(newInstruction, "orn ");
        strcat(newInstruction, op1Str);
        strcat(newInstruction, ", ");
        strcat(newInstruction, zrStr);
        strcat(newInstruction, ", ");
        strcat(newInstruction, op2Str);
        opcode = 1;
        N = 1;

    } else if (strcmp(logicStr, "mov") == 0) {
        strcat(newInstruction, "orr ");
        strcat(newInstruction, op1Str);
        strcat(newInstruction, ", ");
        strcat(newInstruction, zrStr);
        strcat(newInstruction, ", ");
        strcat(newInstruction, op2Str);
        opcode = 1;
        N = 0;
    }

    uint32_t result = dataProcessingRegLogic(opcode, instruction, N);

    free(zrStr);
    free(newInstruction);
    free(logicStr);
    free(op1Str);
    free(op2Str);

    return result;
}

uint32_t dataProcessingRegMultiply(int x, char* instruction) {
    char* mulStr = malloc(strlen(instruction) * sizeof(char));
    char* rdStr = malloc(strlen(instruction) * sizeof(char));
    char* rnStr = malloc(strlen(instruction) * sizeof(char));
    char* rmStr = malloc(strlen(instruction) * sizeof(char));
    char* raStr = malloc(strlen(instruction) * sizeof(char));

    scanf("%s %s, %s, %s, %s", mulStr, rdStr, rnStr, rmStr, raStr);

    int sf, rd, rn, rm, ra;
    parseRegister(rdStr, &sf, &rd);
    parseRegister(rnStr, &sf, &rn);
    parseRegister(rmStr, &sf, &rm);
    parseRegister(raStr, &sf, &ra);

    uint32_t result = 0;
    result += rd & 0x1f; // rd
    result += (rn & 0x1f) << 5; // rn
    result += (ra & 0x1f) << 10; // ra
    result += (x & 0x1) << 15; // x : which multiply mode
    result += (rm & 0x1f) << 16; // rm
    result += (0x0d8) << 21; // const 0b0011011000
    result += (sf & 0x1) << 31; // sf

    free(mulStr);
    free(rdStr);
    free(rnStr);
    free(rmStr);
    free(raStr);

    return result;
}

uint32_t dataProcessingMulMneg(int x, char* instruction) {
    return 0; // to be implemented
}