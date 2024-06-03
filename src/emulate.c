#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./emulator/machine.h"
#include "./emulator/memory.h"

#include "./emulator/instructions/dataProcessingImm.h"
#include "./emulator/instructions/dataProcessingReg.h"
#include "./emulator/instructions/sdt.h"
#include "./emulator/instructions/branchInstr.h"

int main(int argc, char **argv) {
    struct Machine machine;
    initialiseMachine(&machine);
    loadProgram(argv[1], &machine);

    uint32_t currentInstruction;
    int op0;
    bool exit = false;
    while (!exit) {
        currentInstruction = fetchInstruction(&machine);
        bool branch = false;
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
            executeSdt(&machine, currentInstruction);
        } else if ((op0 & 0xe) == 10) { // 0xe = 0b1110
            // it is Branches
            branch = branchInstruction(&machine, currentInstruction);
        }

        exit = currentInstruction == 0x8a000000;
        
        if (!exit && !branch) {
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
