
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>


#ifndef UTILITIES_C
#define UTILITIES_C
#include "./utilities.h"


// print binary number
void printBinary(uint32_t n, int nbits) {
    uint32_t mask = 1 << (nbits - 1);
    for (int i=0; i<nbits; i++) {
        if (i % 8 == 0) {
            putchar(' ');
        }
        if ((n & mask) == 0) {
            putchar('0');
        }
        else {
            putchar('1');
        }
        mask = mask >> 1;
    }
    putchar('\n');
}

// if the instruction is a label
bool isLabel(char* instruction) {
    int length = strlen(instruction);
    
    // remove \n
    while (length > 0 && isspace(instruction[length - 1])) {
        length--;
    }

    return (length > 0 && instruction[length - 1] == ':');
}

// if the instruction represents a .int directive
bool isIntDirective(char* instruction) {
    // Skip leading whitespace
    while (isspace((unsigned char)*instruction)) {
        instruction++;
    }
    return strncmp(instruction, ".int", 4) == 0;
}

// parse an immediate value to an int so 0x10 = 16 and 16 = 16
int64_t parseToInt(char* number) {
    uint64_t result;
    // delete the first char if its '#'
    if (number[0] == '#') {
        number = &number[1];
    }
    // delete the final char is its a ','
    if (number[strlen(number) - 1] == ',') {
        number[strlen(number) - 1] = '\0';
    }

    if (number[1] == 'x') {
        sscanf(number, "0x%lx", &result);
    } else {
        sscanf(number, "%lu", &result);
    }
    return result;
}

// parse a string representing a register into its addressing mode (32/64) and its register number (with 1111... for zr)
void parseRegister(char* string, int* sf, int* reg) {
    *sf = 0;
    if (string[0] == 'x') {
        *sf = 1;
    }

    // if the string has a trailing , delete it
    if (string[strlen(string) - 1] == ',') {
        string[strlen(string) - 1] = '\0';
    }

    if (strcmp(&string[1], "zr") == 0) {
        // encodes the zero register
        *reg = *reg | ((short) 0xffffffffffffffff); // reg = 0xff...
    } else {
        *reg = atoi(&string[1]);
    }
}

void replaceCommaWithSpace(char* instruction, char* output) {
    int j = 0 ;

    // printf("will replace the commas: %s\n", instruction);

    for (int i = 0; instruction[i] != '\0'; i++) {
        if (instruction[i] == ',') {
            output[j++] = ' ';
        } else {
            output[j++] = instruction[i];
        }
    }
    output[j] = '\0';

    // printf("we replaced the commas: %s\n", output);
}


#endif