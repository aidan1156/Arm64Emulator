#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MACHINE_C  // Header guard to prevent multiple inclusions
#define MACHINE_C
#include "./machine.c"
#endif


void dataProcessingImmediate(struct Machine* machine, uint32_t instr) {
    short rd = instr & 0x1f;
    int operand = (instr >> 4) & 0x1ffff; // 17 bits long
    short opi = (instr >> 22) & 0x7; //3 bits long
    short opc = (instr >> 28) & 0x3; //2 bits long
    short sf = (instr >> 30) & 0x1; //1 bit long

    if (opi == 2) {
        short sh = (operand >> 21) & 0x1;
        int imm12 = (operand >> 9) & 0xfff;
        int rn = operand & 0x1f;

        if (sh) {
            imm12 = imm12 << 12;
        }

        uint64_t regValue = machine -> registers[rn];
        if (sf == 0) { // if we are using 32 bit addressing only get the bottom 32 bits
            regValue = (regValue << 32) >> 32; // this preserves sign of the lower number
        }


        uint64_t result;
        switch (opc) {
            case 0:
                result = regValue + imm12;
                break;
            case 1:
                result = regValue + imm12;
                machine -> PSTATE.N = (result < 0);
                machine -> PSTATE.Z = (result == 0);
                machine -> PSTATE.C = false;
                machine -> PSTATE.V = (regValue > 0 && imm12 > 0 && result < 0) || (regValue < 0 && imm12 < 0 && result > 0);
                break;
            case 2:
                result = regValue - imm12;
                break;
            case 3:
                result = regValue - imm12;
                machine -> PSTATE.N = (result < 0);
                machine -> PSTATE.Z = (result == 0);
                machine -> PSTATE.C = false;
                machine -> PSTATE.V = (regValue > 0 && imm12 < 0 && result < 0) || (regValue < 0 && imm12 > 0 && result > 0);
                break;
        };

        // if it encodes the zero register do nothing
        if (rd != 0x1f) {
            machine -> registers[rd] = result;
        }
    }
}


int main(void) {
    struct Machine machine;
    initialiseMachine(&machine);
    // dataProcessingImmediate()0 10 100 010 000110111000110110 10110
}