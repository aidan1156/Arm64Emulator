
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t dataProcessingRegAddSub(int opcode, char* rdStr, char* rnStr, char* op2Str);

uint32_t dataProcessingCmpCmn(int opcode, char* instruction);

uint32_t dataProcessingNeg(int opcode, char* instruction);

uint32_t dataProcessingRegLogic(int opcode, char* instruction);

uint32_t dataProcessingRegMultiply(int opcode, char* instruction);