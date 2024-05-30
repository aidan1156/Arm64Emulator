#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// sign extend a 32 bit number to 64 bit
int64_t extendTo64Bit(int64_t a);

// print binary number
void printBinary(uint32_t n, int nbits);