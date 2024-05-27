#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MACHINE_C  // Header guard to prevent multiple inclusions
#define MACHINE_C
#include "./machine.c"
#endif


// sign extend a 32 bit number to 64 bit
int64_t extendTo64Bit(int64_t a) {
    if ((a & 0x80000000) == 0) {
        return a & 0xffffffff; // set upper 32 bits to 0 as a is >= 0
    } 
    return a | 0xffffffff00000000; // otherwise set upper 32 bits to 1 as a is negative
}

void computeArithmeticOperation(struct Machine* machine, int64_t a, int64_t b, short opc, short sf, short rd) {
    int64_t result = 0;
    int64_t mask = 0xffffffffffffffff;
    if (sf == 0) {
        mask = 0xffffffff;
    }
    switch (opc) {
        case 0:
            result = a + b;
            break;
        case 1:
            result = a + b;
            if (sf == 0) {
                result = extendTo64Bit(result);
            }
            machine -> PSTATE.N = (result < 0);
            machine -> PSTATE.Z = (result == 0);
            machine -> PSTATE.C = false;
            machine -> PSTATE.V = (a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0);
            break;
        case 2:
            result = a - b;
            break;
        case 3:
            result = a - b;
            if (sf == 0) {
                result = extendTo64Bit(result);
            }
            machine -> PSTATE.N = (result < 0);
            machine -> PSTATE.Z = (result == 0);
            machine -> PSTATE.C = false;
            machine -> PSTATE.V = (a > 0 && b < 0 && result < 0) || (a < 0 && b > 0 && result > 0);
            break;
    };

    result = result & mask;
    // if it encodes the zero register do nothing
    if (rd != 0x1f) {
        machine -> registers[rd] = result;
    }
}


static void arithmeticInstruction(struct Machine* machine, short rd, int operand, short opc, short sf) {
    short sh = (operand >> 17) & 0x1;
    unsigned int imm12 = (operand >> 5) & 0xfff;
    int rn = operand & 0x1f;

    if (sh) {
        imm12 = imm12 << 12;
    }

    int64_t regValue = machine -> registers[rn];
    if (sf == 0) {
        regValue = extendTo64Bit(regValue);
    }

    computeArithmeticOperation(machine, regValue, imm12, opc, sf, rd);
}

static void wideMoveInstruction(struct Machine* machine, short rd, int operand, short opc, short sf) {
    uint64_t imm16 = operand & 0x7FFF; // ensure it is 15 bits
    short hw = (operand >> 16) & 0x3; // ensure it is 2 bits
    imm16 <<= hw * 16;

    uint64_t sizeMask = 0xffffffffffffffff;
    if (sf == 0) {
        sizeMask = 0xffffffff;
    }

    if (hw > 1 && sf == 0) {
        fprintf(stderr, "hw must be 0 or 1 for 32 bit addressing\n");
    }


    switch (opc) {
        case 0:
            machine -> registers[rd] = ~imm16;
            if (sf == 0) {
                // clear upper 32 bits
                machine -> registers[rd] = ~imm16 & sizeMask;
            }
            break;
        case 2:
            machine -> registers[rd] = imm16 & sizeMask;
            break;
        case 3:
            uint64_t mask = 0xffff;
            mask <<= hw * 16;
            mask = ~mask;
            // set bits in the range of the imm16 to 0 then add imm16
            machine -> registers[rd] = (((machine -> registers[rd]) & mask) + imm16) & sizeMask;
            break;
    }
}

void dataProcessingImmediate(struct Machine* machine, uint32_t instr) {
    short rd = instr & 0x1f;
    int operand = (instr >> 5) & 0x3ffff; // 18 bits long
    short opi = (instr >> 23) & 0x7; // 3 bits long
    short opc = (instr >> 29) & 0x3; // 2 bits long
    short sf = (instr >> 31) & 0x1; // 1 bit long

    if (opi == 2) {
        arithmeticInstruction(machine, rd, operand, opc, sf);
    } else if (opi == 5) {
        wideMoveInstruction(machine, rd, operand, opc, sf);
    } else {
        fprintf(stderr, "Unsupported opi value of %d\n", opi);
    }
}