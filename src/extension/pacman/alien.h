#include <stdlib.h>
#include "../gameEngine.h"

typedef struct alien *alien;

alien createAlien(int startx, int starty, int setDir);

void drawAlien(alien a, Window window);

void updateAlien(alien a, bool pos);

int getAlienPosX(alien a);

int getAlienPosY(alien a);

void freeAlien(alien a);
