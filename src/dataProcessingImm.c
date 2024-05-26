#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MACHINE_C  // Header guard to prevent multiple inclusions
#define MACHINE_C
#include "./machine.c"
#endif


void computeArithmeticOperation(struct Machine* machine, uint64_t a, uint64_t b, short opc, short sf, short rd) {
    uint64_t result;
    switch (opc) {
        case 0:
            result = a + b;
            break;
        case 1:
            result = a + b;
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
            machine -> PSTATE.N = (result < 0);
            machine -> PSTATE.Z = (result == 0);
            machine -> PSTATE.C = false;
            machine -> PSTATE.V = (a > 0 && b < 0 && result < 0) || (a < 0 && b > 0 && result > 0);
            break;
    };

    // ensure it fits into 32 bit
    if (sf == 0) {
        result = result & 0xffffffff;
    }

    // if it encodes the zero register do nothing
    if (rd != 0x1f) {
        machine -> registers[rd] = result;
    }
}


static void arithmeticInstruction(struct Machine* machine, short rd, int operand, short opc, short sf) {
    short sh = (operand >> 17) & 0x1;
    int imm12 = (operand >> 5) & 0xfff;
    int rn = operand & 0x1f;

    if (sh) {
        imm12 = imm12 << 12;
    }

    uint64_t regValue = machine -> registers[rn];
    if (sf == 0) { // if we are using 32 bit addressing only get the bottom 32 bits
        regValue = (regValue << 32) >> 32; // this preserves sign of the lower number so 0011 << 2 = 1100 then 1100 >> 2 = 1111 (equivalent to 11)
    }

    computeArithmeticOperation(machine, regValue, imm12, opc, sf, rd);
}

static void wideMoveInstruction(struct Machine* machine, short rd, int operand, short opc, short sf) {
    uint64_t imm16 = operand & 0x7FFF; // ensure it is 15 bits
    short hw = (operand >> 16) & 0x3; // ensure it is 2 bits

    if (hw > 1 && sf == 0) {
        fprintf(stderr, "hw must be 0 or 1 for 32 bit addressing\n");
    }

    imm16 <<= hw * 16;

    switch (opc) {
        case 0:
            machine -> registers[rd] = ~imm16;
            if (sf == 0) {
                // clear upper 32 bits
                machine -> registers[rd] = ~imm16 & 0xffffffff;
            }
            break;
        case 2:
            machine -> registers[rd] = imm16;
            break;
        case 3:
            uint64_t mask = 0xffff;
            mask <<= hw * 16;
            mask = ~mask;
            // set bits in the range of the imm16 to 0 then add imm16
            machine -> registers[rd] = ((machine -> registers[rd]) & mask) + imm16;
            break;
    }
}

void dataProcessingImmediate(struct Machine* machine, uint32_t instr) {
    short rd = instr & 0x1f;
    int operand = (instr >> 5) & 0x1ffff; // 17 bits long
    short opi = (instr >> 23) & 0x7; // 3 bits long
    short opc = (instr >> 29) & 0x3; // 2 bits long
    short sf = (instr >> 31) & 0x1; // 1 bit long

    if (opi == 2) {
        arithmeticInstruction(machine, rd, operand, opc, sf);
    } else if (opi == 5) {
        wideMoveInstruction(machine, rd, operand, opc, sf);
    }
}


int main(void) {
    struct Machine machine;
    initialiseMachine(&machine);
    dataProcessingImmediate(&machine, 0x91000822);
    printMachine(&machine, NULL);
}