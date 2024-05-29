#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


// load the program into main memory
void loadProgram(char *file, struct Machine* machine);

// fetch the instruction at the program counter from memory
<<<<<<< HEAD
uint32_t fetchInstruction(struct Machine* machine);
=======
uint32_t fetchInstruction(struct Machine* machine);
>>>>>>> setting-up-cpu
