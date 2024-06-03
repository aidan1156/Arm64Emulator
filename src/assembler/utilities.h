
#include "./maps.h"


bool isLabel(char* instruction);

bool isIntDirective(char* instruction);

int64_t parseToInt(char* number);

void parseRegister(char* string, short* sf, short* reg);