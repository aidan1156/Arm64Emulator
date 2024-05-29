#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./emulator/machine.h"
#include "./emulator/memory.h"

#include "./emulator/instructions/dataProcessingImm.h"
#include "./emulator/instructions/dataProcessingReg.h"

// print binary number
void printBinary(uint32_t n, int nbits) {
    uint32_t mask = 1 << (nbits - 1);
    for (int i=0; i<nbits; i++) {
        if (i % 8 == 0) {
            putchar(' ');
        }
        if ((n & mask) == 0) {
            putchar('0');
        }
        else {
            putchar('1');
        }
        mask = mask >> 1;
    }
    putchar('\n');
}

int main(int argc, char **argv) {
    struct Machine machine;
    initialiseMachine(&machine);
    loadProgram(argv[1], &machine);

    uint32_t currentInstruction;
    int op0;
    bool exit = false;
    while (!exit) {
        currentInstruction = fetchInstruction(&machine);

        //determine what of the 4 types the instr is
        op0 = (currentInstruction >> 25) & 0xf;
        // create a mask so we ignore dont cares
        // then compare to the binary value of a certain type
        if ((op0 & 0xe) == 8) { // 0xd = 0b1110
            // it is Data Processing (Immediate)
            dataProcessingImmediate(&machine, currentInstruction);
        } else if ((op0 & 0x7) == 5) { // 0x7 = 0b0111
            // it is Data Processing (Register) 
            dataProcessingRegister(&machine, currentInstruction);
        } else if ((op0 & 0x5) == 4) { // 0x5 = 0b0101
            // it is Loads and Stores
        } else if ((op0 & 0xe) == 10) { // 0xe = 0b1110
            // it is Branches
        }

        exit = currentInstruction == 0x8a000000;

        if (!exit) {
            machine.PC += 4;
        }
    }

    char* path = NULL;
    if (argc > 2) {
        path = argv[2];
    }
    printMachine(&machine, path);

    return EXIT_SUCCESS;
}
