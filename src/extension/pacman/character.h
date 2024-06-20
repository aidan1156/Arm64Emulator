#include <stdlib.h>
#include "./alien.h"
#include "../gameEngine.h"

typedef struct charac *charac;

charac createCharac(int height, int width);

void drawCharac(charac c, Window window);

void moveCharac(Window win, charac c, int dir);

bool detectDeath(charac c, alien a);

void resetCharac(charac c);

void freeCharac(charac c);