#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "./machine.h"

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

// print the machines contents to a file path or default to stdout
void printMachine(struct Machine* machine, char* path) {
    FILE *file;

    if (path == NULL) { //default to stdout if nothing provided
        file = stdout;
    } else {
        file = fopen(path, "w");
    }

    fprintf(file, "Registers:\n");
    for (int i=0; i<31; i++) {
        fprintf(file, "X%02d    = %016lx\n", i, machine -> registers[i]);
    }
    fprintf(file, "PC     = %016lx\n", machine -> PC);
    fprintf(file, "PSTATE : ");
    (machine -> PSTATE.N) ? fprintf(file, "N") : fprintf(file, "-");
    (machine -> PSTATE.Z) ? fprintf(file, "Z") : fprintf(file, "-");
    (machine -> PSTATE.C) ? fprintf(file, "C") : fprintf(file, "-");
    (machine -> PSTATE.V) ? fprintf(file, "V") : fprintf(file, "-");

    fprintf(file, "\nNon-Zero Memory:\n");

    uint32_t value;
    for (int i=0; i<MEMORY_SIZE; i += 4) {
        value = 0;
        for (int n=0; n<4;n++) {
            value += ((machine -> memory[i+n]) & 0xff) << (8 * n);
        }
        if (value != 0) {
            fprintf(file, "0X%08x : %08x\n", i, value);
        }
    }

    fflush(file);

    if (path != NULL) {
        fclose(file);
    }
}
