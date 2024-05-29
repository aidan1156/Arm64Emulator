<<<<<<< HEAD
#ifndef MACHINE_H
#define MACHINE_H

=======
#include <stdlib.h>
#include <stdio.h>
>>>>>>> setting-up-cpu
#include <stdint.h>
#include <stdbool.h>

#define NUM_REGISTERS 31
<<<<<<< HEAD
#define MEMORY_SIZE (2 << 20) // 2MB = 2 * 2^20
=======
#define MEMORY_SIZE 2 << 20
>>>>>>> setting-up-cpu

struct Pstate {
    bool N;
    bool Z;
    bool C;
    bool V;
};

struct Machine {
<<<<<<< HEAD
    uint64_t registers[NUM_REGISTERS];
    uint64_t ZR;
    uint64_t PC;
    uint64_t SP;
    struct Pstate PSTATE;
    uint8_t memory[MEMORY_SIZE];
};

void initialiseMachine(struct Machine* machine);
void printMachine(struct Machine* machine, char* path);

#endif // MACHINE_H
=======
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
>>>>>>> setting-up-cpu
