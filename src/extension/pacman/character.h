#include <stdlib.h>
#include "./walls.h"

typedef struct charac *charac;

charac createCharac(int height, int width);

void drawCharac(charac c, Window window);

void moveCharac(Window win, charac c, int dir);

int getPosX(charac c);

int getPosY(charac c);

void resetCharac(charac c);

void freeCharac(charac c);