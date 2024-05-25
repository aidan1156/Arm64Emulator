#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./emulator/machine.c"
#include "./emulator/memory.c"


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
