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
    int8_t memory[MEMORY_SIZE];
    int64_t registers[NUM_REGISTERS];
    struct Pstate PSTATE;
    int64_t ZR;
    int64_t PC;
    int64_t SP;
};

// ensure all memory segments are clear and have the correct value for a machine
void initialiseMachine(struct Machine* machine);

// print the machines contents to a file path or default to stdout
void printMachine(struct Machine* machine, char* path);
