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
        op2 = rm << operand;

    } else if (shift == 1) { // 01 lsr : logical shift right
        unsigned int op2 = rm >> operand;
        return op2;

    } else if (shift == 2) { // 10 asr : arithmetic shift right
        op2 = rm >> operand;

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
        op2 = shifted | rot_bits;
    }
    return op2;
}

void logic(struct Machine *machine, int opc, int rd, int rn, int op2, int sf, int rmAddress) {
    uint64_t result;
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
            //update condition flags
            if (sf == 0) {
                result = result & 0xffffffff;
                sign_bit = result >> 31; // taking first bit for signbit
                sign_bit = sign_bit & 0x1; // ensuring it's only 1 bit long
            } else {
                sign_bit = result >> 63;
                sign_bit = sign_bit & 0x1;
            }
            // updating PSTATE flags
            machine -> PSTATE.N = sign_bit;
            if (result == 0) {
                machine -> PSTATE.Z = 1;
            }
            machine -> PSTATE.C = 0;
            machine -> PSTATE.V = 0;
    }

    if (sf == 0) {
        result = result & 0xffffffff; // removing upper 32 bits
    }
    machine -> registers[rmAddress] = result;

}

void multiply(struct Machine *machine, int ra, int rn, int rm, int rdAddress, int sf, int x) {
    uint64_t result;
    switch (x) {
            case 0:
                result = ra + (rn * rm);
                if (sf == 0) {
                    result = result & 0xffffffff;
                }
                machine -> registers[rdAddress] = result;
            case 1:
                result = ra - (rn * rm);
                if (sf == 0) {
                    result = result & 0xffffffff;
                }
                machine -> registers[rdAddress] = result;
        }
}

void dataProcessingRegister(struct Machine *machine, uint32_t instruction) {
    int rdAddress = machine -> registers[instruction & 0x1F];

    int rnAddress = (instruction >> 5) & 0x1F;
    int rn;
    if (rnAddress == 0b11111) {
        rn = 0;
    } else {
        rn = machine -> registers[rnAddress];
    }

    int operand = (instruction >> 10) & 0x3F;

    int rmAddress = (instruction >> 16) & 0x1F;
    int rm;
    if (rmAddress == 0b11111) {
        rm = 0;
    } else {
        rm = machine -> registers[rmAddress];
    }

    int opr = (instruction >> 21) & 0xF;
    int M = (instruction >> 28) & 0x1;
    int opc = (instruction >> 29) & 0x3;
    int sf = (instruction >> 31) & 0x1;


    if ((M == 0) & (((opr & 0x9) == 8) | ((opr & 0x8) == 0))) {
        // M == 0 and (opr == 1xx0 or opr == 0xxx)
        // Arithmetic instr & bit-logic
        if (rdAddress == 0b11111) {
            return;
            // if rd is zero register, nothing to store
        }
        int shift = ((opr >> 1) & 0x3); // 0x3 == 0b0011
        int op2 = shifting(shift, sf, rm, operand, opr);

        // logic instructions
        if ((opr & 0x8) == 0) {
            int N = opr & 0x1;
            if (N == 1) {
                // negated op2
                logic(machine, opc, rdAddress, rn, ~op2, sf, rm);
            } else {
                logic(machine, opc, rdAddress, rn, op2, sf, rm);
            }
        } else if ((opr & 0x9) == 8) {
            // Arithmetic instruction
            // IMPLEMENTATION NEEDED, USE CODE FROM IMMEDIATE OPERAND
        }
        
    } else if ((M == 1) & ((opr & 0xF) == 8)) {// M == 1 and opr == 1000
        // Multiply
        int x = (operand >> 5) & 0x1; 
        int raAddress = operand & 0x1F;
        int ra;
        if (raAddress == 0b11111) {
            ra = 0;
            
        } else {
            ra = machine -> registers[raAddress];
        }
        multiply(machine, ra, rn, rm, rdAddress, sf, x);
    }
}
