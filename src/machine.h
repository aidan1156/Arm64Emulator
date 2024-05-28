#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_REGISTERS 31
#define MEMORY_SIZE (2 << 20) // 2MB = 2 * 2^20

struct Pstate {
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
    struct Pstate PSTATE;
    uint8_t memory[MEMORY_SIZE];
};

void initialiseMachine(struct Machine* machine);
void printMachine(struct Machine* machine, char* path);

#endif // MACHINE_H
