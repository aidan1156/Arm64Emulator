#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Function prototypes for calculating area
double calculate_area_rectangle(double length, double width);
double calculate_area_circle(double radius);



#define NUM_REGISTERS 31
#define MEMORY_SIZE 2 << 21

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