
#include "./maps.h"

#ifndef UTILITIES_H
#define UTILITIES_H

bool isLabel(char* instruction);

bool isIntDirective(char* instruction);

int64_t parseToInt(char* number);

void parseRegister(char* string, int* sf, int* reg);

#endif