
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "./utilities.h"


// if the instruction is a label
bool isLabel(char* instruction) {
    return instruction[strlen(instruction) - 1] == ':';
}

// if the instruction represents a .int directive
bool isIntDirective(char* instruction) {
    return instruction[0] == '.';
}

// parse an immediate value to an int so 0x10 = 16 and 16 = 16
int64_t parseToInt(char* number) {
    uint64_t result;
    if (number[1] == 'x') {
        sscanf(number, "0x%lx", &result);
    } else {
        sscanf(number, "%lu", &result);
    }
    return result;
}

// parse a string representing a register into its addressing mode (32/64) and its register number (with 1111... for zr)
void parseRegister(char* string, short* sf, short* reg) {
    *sf = 0;
    if (string[0] == 'x') {
        *sf = 1;
    }
    if (strcmp(&string[1], "zr") == 0) {
        // encodes the zero register
        *reg = *reg | ((short) 0xffffffffffffffff); // reg = 0xff...
    } else {
        *reg = atoi(&string[1]);
    }
}