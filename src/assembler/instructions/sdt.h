#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utilities.h"
#include "../fileIO.h"

void parseAddress(char* addressPt1, char* addressPt2, int* sf, int* rt, int* xn, int* xm,
 int64_t* offset, bool* isU, bool* isPostIndex, bool* isPreIndex,
  bool* isLit, bool* isReg);

uint32_t singleDataTransfer(int isLoad, char* instruction, int PC, Map* labelmap);
