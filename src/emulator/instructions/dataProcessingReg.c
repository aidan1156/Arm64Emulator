#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../machine.h"
#include "./dataProcessingReg.h"

// to use computeArithmetic function
#include "./dataProcessingImm.h"



int64_t shifting( int shift, int sf, int64_t rm, int operand, int opr ) {
    int64_t op2;
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

void logic(struct Machine *machine, int opc, int64_t rn, int64_t op2, int sf, int rdAddress) {
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
    machine -> registers[rdAddress] = result;

}

void multiply(struct Machine *machine, int64_t ra, int64_t rn, int64_t rm, int rdAddress, int sf, int x) {
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
    int64_t rn;
    if (rnAddress == 0x1F) {
        rn = 0;
    } else {
        rn = machine -> registers[rnAddress];
    }

    int operand = (instruction >> 10) & 0x3F;

    int rmAddress = (instruction >> 16) & 0x1F;
    int64_t rm;
    if (rmAddress == 0x1F) {
        rm = 0;
    } else {
        rm = machine -> registers[rmAddress];
    }
    printf("rn: %ld\n", rn);
    printf("rm: %ld\n", rm);
    printf("operand: %d\n", operand);

    int opr = (instruction >> 21) & 0xF;
    int M = (instruction >> 28) & 0x1;
    int opc = (instruction >> 29) & 0x3;
    int sf = (instruction >> 31) & 0x1;


    if ((M == 0) & (((opr & 0x9) == 8) | ((opr & 0x8) == 0))) {
        // M == 0 and (opr == 1xx0 or opr == 0xxx)
        // Arithmetic instr & bit-logic
        if (rdAddress == 0x1F) {
            return;
            // if rd is zero register, nothing to store
            printf("rd is zero register\n");
        }
        
        int shift = ((opr >> 1) & 0x3); // 0x3 == 0b0011
        int64_t op2 = shifting(shift, sf, rm, operand, opr);
        printf("op2: %ld\n", op2);

        // logic instructions
        if ((opr & 0x8) == 0) {
            printf("logic instruction being emulated!\n");
            int N = opr & 0x1;
            if (N == 1) {
                // negated op2
                logic(machine, opc, rn, ~op2, sf, rdAddress);
            } else {
                logic(machine, opc, rn, op2, sf, rdAddress);
            }
        } else if ((opr & 0x9) == 8) {
            computeArithmeticOperation(machine, rn, op2, opc, sf, rdAddress);
            // Arithmetic instruction - using code from dataProcessingImm
        }
        
    } else if ((M == 1) & ((opr & 0xF) == 8)) {// M == 1 and opr == 1000
        // Multiply
        int x = (operand >> 5) & 0x1; 
        int raAddress = operand & 0x1F;
        int64_t ra;
        if (raAddress == 0x1F) {
            ra = 0;
            
        } else {
            ra = machine -> registers[raAddress];
        }
        multiply(machine, ra, rn, rm, rdAddress, sf, x);
    }
}
