

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./dataProcessingImm.h"

uint32_t dataProcessingImmArithmetic(int opcode, char* instruction) {
    char* rdStr = malloc(strlen(instruction) * sizeof(char));
    char* rnStr = malloc(strlen(instruction) * sizeof(char));
    char* immStr = malloc(strlen(instruction) * sizeof(char));
    char* shStr = malloc(strlen(instruction) * sizeof(char));
    char* opcodeStr = malloc(strlen(instruction) * sizeof(char));

    // parse the instruction and if it doesnt have the optional shift set
    // set it to #0 (default value)
    if (sscanf(instruction, "%s %s %s %s lsl %s", opcodeStr, rdStr, rnStr, immStr, shStr) == 4) {
        strcpy(shStr, "#0");
    }

    int imm = parseToInt(immStr);
    int sf, rd, rn;
    parseRegister(rdStr, &sf, &rd);
    parseRegister(rnStr, &sf, &rn);
    int sh = (parseToInt(shStr) == 0) ? 0 : 1;

    // now we have the individual components of the instr

    uint32_t result = 0;
    result += rd & 0x1f; // rd
    result += (rn & 0x1f) << 5; // rn
    result += (imm & 0xfff) << 10; // imm12
    result += (sh & 0x1) << 22; // sh
    result += 2 << 23; // opi
    result += 4 << 26; // constant 0b100
    result += (opcode & 0x3) << 29; // opc
    result += (sf & 0x1) << 31; // sf

    free(rdStr);
    free(rnStr);
    free(immStr);
    free(shStr);
    free(opcodeStr);

    return result;
}