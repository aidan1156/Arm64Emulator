
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./dataProcessingReg.h"

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
    if (cmpstr(shiftStr, "lsl") == 0) {
        opr = 0x8; //0b1000
    } else if (cmpstr(shiftStr, "lsr") == 0) {
        opr = 0xA; //0b1010
    } else if (cmpstr(shiftStr, "asr") == 0) {
        opr = 0xC; // 0b1100
    }

    uint32_t result = 0;
    result += rd & 0x1f; // rd
    result += (rn & 0x1f) << 5; // rn

    result += 2 << 23; // opi
    result += 4 << 26; // constant 0b100
    result += (opcode & 0x3) << 29; // opc
    result += (sf & 0x1) << 31; // sf



}

uint32_t dataProcessingRegCmpCmn(int opcode, char* instruction) {

}


uint32_t dataProcessingRegLogic(int opcode, char* instruction) {

}

uint32_t dataProcessingRegMultiply(int opcode, char* instruction) {

}