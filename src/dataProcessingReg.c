#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MACHINE_C  // Header guard to prevent multiple inclusions
#define MACHINE_C
#include "./machine.c"
#endif

int shifting( int shift, int sf, int rm, int operand, int opr ) {
    int op2;
    if (shift == 0) { // 00 lsl : logical shift left
        int op2 = rm << operand;

    } else if (shift == 1) { // 01 lsr : logical shift right
        unsigned int op2 = rm >> operand;

    } else if (shift == 2) { // 10 asr : arithmetic shift right
         int op2 = rm >> operand;

    } else if ((shift == 3) & ((opr & 0x8) == 0)) { // 11 ror : rotate right
        // bit logic only 
        // shift to right by operand
        int shifted = rm >> operand;
        int rot_bits;

        // rotate the bits respect to bit-width of registers
        if (sf == 1) {
            rot_bits = rm << (64 - operand);
        } else {
            rot_bits = rm << (32 - operand);
        }
        
        // combining the shifted bits & rotated bits
        int op2 = shifted | rot_bits;
    }
    return op2;
}

void logic(struct Machine *machine, int opc, int rd, int rn, int op2, int sf) {
    uint16_t result;
    int sign_bit;

    switch (opc) {
        case 0:
            result = rn & op2;
        case 1:
            result = rn | op2;
        case 2:
            result = rn ^ op2;
        case 3:
            result = rn & op2;
    }

    if (sf == 0) {
        result = result & 0xffffffff; // removing upper 32 bits
        
        sign_bit = result >> 31; // taking first bit for signbit
        sign_bit = sign_bit & 0x1; // ensuring it's only 1 bit long
    } else {
        sign_bit = result >> 63;
        sign_bit = sign_bit & 0x1;
    }

    machine -> registers[rn] = result;

    // updating PSTATE flags
    machine -> PSTATE.N = sign_bit;
    if (result == 0) {
        machine -> PSTATE.Z = 1;
    }
    machine -> PSTATE.C = 0;
    machine -> PSTATE.V = 0;
}

int main(uint32_t instruction, struct Machine *machine) {
    int rd = instruction & 0x1F;
    int rn = (instruction >> 5) & 0x1F;
    int operand = (instruction >> 10) & 0x3F;
    int rm = (instruction >> 16) & 0x1F;
    int opr = (instruction >> 21) & 0xF;
    int M = (instruction >> 28) & 0x1;
    int opc = (instruction >> 29) & 0x3;
    int sf = (instruction >> 31) & 0x1;



    if ((M == 0) & (((opr & 0x9) == 8) | ((opr & 0x8) == 0))) {
        // M == 0 and (opr == 1xx0 or opr == 0xxx)
        // Arithmetic instr & bit-logic
        int shift = ((opr >> 1) & 0x3); // 0x3 == 0b0011
        int op2 = shifting(shift, sf, rm, operand, opr);

        // logic instructions
        if ((opr & 0x8) == 0) {
            
            int N = opr & 0x1;
            if (N == 1) {
                // negated op2
                logic(&machine, opc, rd, rn, ~op2, sf);
            } else {
                logic(&machine, opc, rd, rn, op2, sf);
            }
        } else if ((opr & 0x9) == 8) {
            // Arithmetic instruction
            // IMPLEMENTATION NEEDED, USE CODE FROM IMMEDIATE OPERAND
        }
        
    } else if ((M == 1) & ((opr & 0xF) == 8)) {// M == 1 and opr == 1000
        // Multiply
    }
}


