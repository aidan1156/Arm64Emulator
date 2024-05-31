#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../machine.h"

#include "./branchInstr.h"

bool branchInstruction(struct Machine* machine, uint32_t instr) {
    short flag = instr >> 30; //first two bits determine type of branch
    if (flag == 0){ //unconditional branch
        int simm26 = instr & 0x3ffffff;

        int offset;
        if (simm26 >> 25) { //check sign for extension to 64bits
            offset = 0xfffffffff0000000 | (simm26 << 2);
        } else {
            offset = (simm26 << 2);
        }
        machine -> PC += offset;
        
    } else if (flag == 3){ //register branch
        short xn = (instr >> 5) & 0x1f;

        machine -> PC = machine -> registers[xn];

    } else { //conditional branch
        int simm19 = (instr >> 5) & 0x7ffff;
        short cond = instr & 0xf;

        int offset;
        if (simm19 >> 18) { //check sign for extension to 64bits
            offset = 0xffffffffffe0003 | (simm19 << 2);
        } else {
            offset = (simm19 << 2);
        }

        bool cond_satisfied;
        switch (cond) {
            case 0:
                cond_satisfied = machine -> PSTATE.Z == 1;
                break;
            case 1:
                cond_satisfied = machine -> PSTATE.Z == 0;
                break;
            case 10: 
                cond_satisfied = machine -> PSTATE.N == machine -> PSTATE.V;
                break;
            case 11:
                cond_satisfied = machine -> PSTATE.N != machine -> PSTATE.V;
                break;
            case 12:
                cond_satisfied = (machine -> PSTATE.Z == 0) && (machine -> PSTATE.N == machine -> PSTATE.V);
                break;
            case 13:
                cond_satisfied = !((machine -> PSTATE.Z == 0) && (machine -> PSTATE.N == machine -> PSTATE.V));
                break;
            case 14:
                cond_satisfied = true;
                break;
        }

        if (cond_satisfied) {
            machine -> PC += offset;
        } else {
            return false; // didn't successfully branch
        }
    }
    return true; //successfully moved to branch
}

