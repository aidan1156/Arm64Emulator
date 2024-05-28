#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MACHINE_H  // Header guard to prevent multiple inclusions
#define MACHINE_H
#include "./machine.h"
#endif

// load the program into main memory
void loadProgram(char *file, struct Machine* machine);

// fetch the instruction at the program counter from memory
uint32_t fetchInstruction(struct Machine* machine);