

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "./dataProcessingImm.h"

uint32_t dataProcessingImmArithmetic(int opcode, char* instruction) {
    replaceCommaWithSpace(instruction, instruction);

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

uint32_t wideMoveInstruction(int opcode, char* instruction) {
    uint32_t result = 0;
    char* rd = malloc(strlen(instruction) * sizeof(char));
    char* mov = malloc(strlen(instruction) * sizeof(char));
    char* imm = malloc(strlen(instruction) * sizeof(char));
    int* sh = malloc(sizeof(int));

    int ret = sscanf(instruction, "%s %s #%s lsl #%d", mov, rd, imm, sh);

    printf("\nmov: %s\nrd : %s\nimm: %s\nsh: %d\n", mov, rd, imm, *sh);
    printf("%c-%c-%c-%c\n", imm[0], imm[1], imm[2], imm[3]);

    uint32_t hexImm = 0;
    int decImm = 0;
    bool isHex = false;
    printf("%s", imm);
    if (strncmp(imm, "0x", 2) == 0) {
        printf("help \n");
        sscanf(imm, "%x", &hexImm);
        isHex = true;
    } else {
        decImm = atoi(imm);
    }
 
    //determine sf and rd values
    int* reg = malloc(5);
    int* sf = malloc(1);
    parseRegister(rd, sf, reg);
    result = result | (*sf << 31);
    result = result | (*reg);
    free(reg);
    free(sf);

    //determine opc and opi
    if (opcode == 0) { //movn
        result = result | 0x12800000;
    } else if (opcode == 1) { //movk
        result = result | 0x72800000;
    } else if (opcode == 2) { //movz
        result = result | 0x52800000;
    }

    //determine operand
    if (ret == 4) {
        result = result | ((*sh / 16) << 21);
    }
    if (isHex) {
        result = result | (hexImm << 5);
    } else {
        result = result | (decImm << 5);
    }
    

    free(mov);
    free(rd);
    free(imm);
    free(sh);
    return result;   
}
