#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MACHINE_C  // Header guard to prevent multiple inclusions
#define MACHINE_C
#include "./machine.c"
#endif

int main(uint32_t instruction) {
    int rd = instruction & 0x1F;
    int rn = (instruction >> 5) & 0x1F;
    int operand = (instruction >> 10) & 0x3F;
    int rm = (instruction >> 16) & 0x1F;
    int opr = (instruction >> 21) & 0xF;
    int M = (instruction >> 28) & 0x1;
    int opc = (instruction >> 29) & 0x3;
    int sf = (instruction >> 31) & 0x1;



    if ((M == 0) & ((opr & 0x9) == 8)) { // M == 0 and opr == 1xx0
        // Arithmetic instr
    } else if ((M == 0) & ((opr & 0x8) == 0)) { // M == 0 and opr == 0xxx
        // Bit-Logic
    } else if ((M == 1) & ((opr & 0xF) == 8)) {// M == 1 and opr == 1000
        // Multiply
    }
}

void arithAndLogic(void) {
    // TO BE IMPLEMENTED
}

void multi(void) {
    // TO BE IMPLEMENTED
}
