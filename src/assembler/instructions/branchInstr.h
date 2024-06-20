

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../maps.h"

uint32_t branchInstruction(int opcode, char* instruction, int cur_address, char cond[2], Map* labelmap);