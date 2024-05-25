#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MACHINE_C  // Header guard to prevent multiple inclusions
#define MACHINE_C
#include "./machine.c"
#endif


// load the program into main memory
void loadProgram(char *file, struct Machine *machine) {
    FILE *input;
    input = fopen(file, "rb");

    fread(machine -> memory, MEMORY_SIZE, 1, input);

    fclose(input);
}

// fetch the instruction at the program counter from memory
uint32_t fetchInstruction(struct Machine *machine) {
    uint32_t result = 0;
    unsigned int value = 0;
    // each instruction is 4 bytes
    // load in 1 byte at a time
    for (int i=0; i<4; i++) {
        value = machine -> memory[(machine -> PC) + i];
        result += value << (i * 8);
    }
    return result;
}