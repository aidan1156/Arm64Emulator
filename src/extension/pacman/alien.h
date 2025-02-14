#include <stdlib.h>

#include "../gameEngine.h"
#include "./character.h"

typedef struct alien *alien;

alien createAlien(int startx, int starty, int setDir);

void drawAlien(alien a, Window window);

void updateAlien(alien a, bool pos);

bool detectDeath(charac c, alien a);

void freeAlien(alien a);
