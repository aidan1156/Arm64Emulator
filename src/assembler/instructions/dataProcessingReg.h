
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utilities.h"


int shiftOpr(char* shiftStr);

uint32_t dataProcessingRegAddSub(int opcode, char* rdStr, char* rnStr, char* op2Str);

uint32_t dataProcessingCmpCmn(int opcode, char* instruction);

uint32_t dataProcessingNeg(int opcode, char* instruction);

uint32_t dataProcessingRegLogic(int opcode, char* instruction, int N);

uint32_t dataProcessingTstMvnMov(char* instruction);

uint32_t dataProcessingRegMultiply(int x, char* instruction);

uint32_t dataProcessingMulMneg(int x, char* instruction);