#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// sign extend a 32 bit number to 64 bit
int64_t extendTo64Bit(int64_t a) {
    if ((a & 0x80000000) == 0) {
        return a & 0xffffffff; // set upper 32 bits to 0 as a is >= 0
    } 
    return a | 0xffffffff00000000; // otherwise set upper 32 bits to 1 as a is negative
}


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