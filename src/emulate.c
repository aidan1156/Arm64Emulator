#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_REGISTERS 31
#define MEMORY_SIZE 2 << 21

struct pstate {
    bool N;
    bool Z;
    bool C;
    bool V;
};

struct Machine {
    uint64_t registers[NUM_REGISTERS];
    uint64_t ZR;
    uint64_t PC;
    uint64_t SP;
    struct pstate PSTATE;
    uint8_t memory[MEMORY_SIZE];
};


// print binary number
void printBinary(uint32_t n, int nbits) {
    uint32_t mask = 1 << (nbits - 1);;
    for (int i=0; i<nbits; i++) {
        if (i % 8 == 0) {
            putchar(' ');
        }
        if ((n & mask) == 0 ) {
            putchar('0');
        }
        else {
            putchar('1');
        }
        mask = mask >> 1;
    }
    putchar('\n');
}


// ensure all memory segments are clear and have the correct value for a machine
void initialiseMachine(struct Machine* machine) {
    for (int i=0; i<NUM_REGISTERS; i++) {
        machine -> registers[i] = 0;
    }
    machine -> ZR = 0;
    machine -> PC = 0;
    machine -> SP = 0;
    machine -> PSTATE.N = false;
    machine -> PSTATE.C = false;
    machine -> PSTATE.V = false;
    machine -> PSTATE.Z = true;

    for (int i=0; i<MEMORY_SIZE; i++) {
        machine -> memory[i] = 0;
    }
}

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
    // each instruction is 4 bytes so load in 1 byte at a time
    for (int i=0; i<4; i++) {
        value = machine -> memory[(machine -> PC) + i];
        result += value << (i * 8);
    }
    return result;
}



int main(int argc, char **argv) {
    struct Machine machine;
    initialiseMachine(&machine);

    loadProgram(argv[1], &machine);

    uint32_t currentInstruction;
    for (int i=0;i<10;i++){
        currentInstruction = fetchInstruction(&machine);
        printBinary(currentInstruction, 32);

        machine.PC += 4;
    }

    return EXIT_SUCCESS;
}
