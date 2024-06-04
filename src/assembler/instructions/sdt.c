#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "sdt.h"
#include "../../emulator/utilities.h"

void parseAddress(char* addressPt1, char* addressPt2, int* sf, int* rt, int* xn, int* xm,
 int64_t* offset, bool* isU, bool* isPostIndex, bool* isPreIndex,
  bool* isLit, bool* isReg) {
    char* immStr = malloc((strlen(addressPt2) + 1) * sizeof(char));

    // identify if post-index, pre-index, unsigned offset, register offset, or literal
    // returns NULL - if ch is not present in s - or
    //  returns a pointer to the left-most occurrence of ch in s
    char* closeBracketPt1 = strchr(addressPt1, ']');
    char* closeBracketPt2 = strchr(addressPt2, ']');
    char* commaPt1 = strchr(addressPt1, ',');
    char* exclamPt2 = strchr(addressPt2, '!');
    char* hashPt2 = strchr(addressPt2, '#');


    if (closeBracketPt1) { 
        // Post-index
        *isPostIndex = 1;

        // extract the base register, xn
        *closeBracketPt1 = '\0';
        parseRegister(addressPt1 + 1, sf, xn);

        // get offset
        *offset = parseToInt(addressPt2);

    } else {
        // extract the base register, xn
        *commaPt1 = '\0';
        parseRegister(addressPt1 + 1, sf, xn);
        

        if (exclamPt2) {
            // Pre-index
            *isPreIndex = 1;

            // get offset
            *closeBracketPt2 = '\0';
            strcpy(immStr, addressPt2);
            *offset = parseToInt(immStr);
            
        } else if (hashPt2) {
            // unsigned offset
            *isU = 1;

            // get offset
            *closeBracketPt2 = '\0';
            strcpy(immStr, addressPt2);
            *offset = parseToInt(immStr);


        } else {
            // register offset
            *isReg = 1;

            // extract xm register
            *closeBracketPt2 = '\0';
            parseRegister(addressPt2, sf, xm);

        }
    }
}

uint32_t singleDataTransfer(int isLoad, char* instruction) {
    int maxStrLength = (strlen(instruction) + 1) * sizeof(char); // incl null term
    char opcode[4]; // = malloc(maxStrLength);
    char rtStr[4];
    char* addressPt1 = malloc(maxStrLength);
    char* addressPt2 = malloc(maxStrLength);
    char* literalStr = malloc(maxStrLength);

    int sf = 0, rt;
    int xn = -1, xm = -1;
    int64_t offset = 0;

    bool isU = 0, isPostIndex = 0, isPreIndex = 0, isLit = 0, isReg = 0;

    uint32_t binInstruction = 0;


    // parse the instruction into opcode and operands string
    if (sscanf(instruction, "%s %s %s %s", opcode, rtStr, addressPt1, addressPt2) == 4) {
        parseAddress(addressPt1, addressPt2, &sf, &rt, &xn, &xm, &offset, &isU,
        &isPostIndex, &isPreIndex, &isLit, &isReg);
    } else if (sscanf(instruction, "%s %s %s", opcode, rtStr, literalStr) == 3) {
        isLit = 1;
    }

    
    parseRegister(rtStr, &sf, &rt);
    

    printf("Instruction: %s\n", instruction);
    printf("Is Load?: %d", isLoad);
    printf("Opcode: %s\n", opcode);
    // printf("We gon chop this address up %s %s\n", addressPt1, addressPt2);
    printf("Sf: %d\n", sf);
    printf("Rt: %d\n", rt);
    printf("Xn: %d\n", xn);
    printf("Xm: %d\n", xm);
    printf("Offset: %ld\n", offset);
    printf("Literal: %s\n", literalStr);
    printf("Post-index: %d\n", isPostIndex);
    printf("Pre-index: %d\n", isPreIndex);
    printf("Unsigned Offset: %d\n", isPreIndex);
    printf("Load Literal: %d\n", isPreIndex);

    // Start constructing the binary instructions
    binInstruction |= (isLit << 31) | (sf << 30) | rt;

    if (!isLit) {
        binInstruction |= (0xE0 << 22); 
        binInstruction |= (isLoad << 22);
        binInstruction |= (isU << 24);
        binInstruction |= (xn << 5);

        if (isU) {
            int size = sf ? 8 : 4;
            binInstruction |= ((offset/size) << 10);
        } else if (isReg) {
            binInstruction |= (1 << 21);
            binInstruction |= (xn << 16);
            binInstruction |= (0x1A <<10);
        } else {
            // pre/post index
            binInstruction |= (0 << 21);
            binInstruction |= (offset << 12);
            binInstruction |= (isPreIndex << 11);
            binInstruction |= (1 << 10);
        }

    } else {
        binInstruction |= (0x18 << 24);
        // need to sort out simm19

    }
    

    free(addressPt1);
    free(addressPt2);
    free(literalStr);

    //printf("This is our binary instruction %d\n", binInstruction);
    printBinary(binInstruction, 32);

    return binInstruction;
}

int main() {
    // Example instructions
    char* instructions[] = {
        "ldr w28, [x12], #226",
        "ldr w11, [x20, #60]!",
        "ldr x3, [x1, #0x0]",
        "ldr w17, [x15, x4]",
        "ldr x0, l1"
    };

    for (int i = 0; i < 5; i++) {
        singleDataTransfer(1, instructions[i]);
        printf("\n");
    }

    return 0;
}