#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_REGISTERS 31
#define MEMORY_SIZE 2 << 20

struct Pstate {
    bool N;
    bool Z;
    bool C;
    bool V;
};

struct Machine {
    uint8_t memory[MEMORY_SIZE];
    uint64_t registers[NUM_REGISTERS];
    struct Pstate PSTATE;
    uint64_t ZR;
    uint64_t PC;
    uint64_t SP;
};

// ensure all memory segments are clear and have the correct value for a machine
void initialiseMachine(struct Machine* machine);

// print the machines contents to a file path or default to stdout
void printMachine(struct Machine* machine, char* path);