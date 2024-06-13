#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "sdt.h"
#include "../utilities.h"
#include "../maps.h"

void parseAddress(char* addressPt1, char* addressPt2, int* sf, int* rt, int* xn, int* xm,
 int64_t* offset, bool* isU, bool* isPostIndex, bool* isPreIndex,
  bool* isLit, bool* isReg) {
    char* immStr = malloc((strlen(addressPt2) + 1) * sizeof(char));

    // identify if post-index, pre-index, unsigned offset, register offset, or literal
    // returns NULL - if ch is not present in s - or
    //  returns a pointer to the left-most occurrence of ch in s
    char* closeBracketPt1 = strchr(addressPt1, ']');
    char* closeBracketPt2 = strchr(addressPt2, ']');
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

    free(immStr);
}

uint32_t singleDataTransfer(int isLoad, char* instruction, int PC, Map* labelMap) {
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

        parseAddress(addressPt1, addressPt2, &sf, &rt, &xn, &xm, &offset,
         &isU, &isPostIndex, &isPreIndex, &isLit, &isReg);
  
    } else if(sscanf(instruction, "%s %s %s", opcode, rtStr, addressPt1) == 3 &&
        instruction[strlen(instruction) - 1] == ']') {
        // zero unsigned case
        isU = 1;
        // extract the base register, xn
        char* closeBracketPt1 = strchr(addressPt1, ']');
        *closeBracketPt1 = '\0';
        parseRegister(addressPt1 + 1, &sf, &xn);

    } else if (sscanf(instruction, "%s %s %s", opcode, rtStr, literalStr) == 3) {
        isLit = 1;
    }


    parseRegister(rtStr, &sf, &rt);

    // sign extend only correct number of bits
    if (isPostIndex || isPreIndex) {
        offset = offset & 0x1FF;
    }
    
    // Start constructing the binary instructions
    binInstruction |= (!isLit << 31) | (sf << 30) | rt;

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
            binInstruction |= (xm << 16);
            binInstruction |= (0x1A <<10);
        } else {
            // pre/post index
            binInstruction |= (0 << 21);
            binInstruction |= (offset << 12);
            binInstruction |= (isPreIndex << 11);
            binInstruction |= (1 << 10);
        }

    } else {
        // load literal
        printMap(labelMap);
        offset = getMap(labelMap, literalStr);
        
        binInstruction |= (0x18 << 24);
        // simm19
        offset = ((offset - PC) / 4) & 0x7FFFF;// make sure to sign extend only up to 19
        binInstruction |= offset << 5;


    }

    free(addressPt1);
    free(addressPt2);
    free(literalStr);

    return binInstruction;
}
