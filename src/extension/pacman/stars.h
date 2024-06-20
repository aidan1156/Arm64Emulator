#include <stdlib.h>
#include "./character.h"

typedef struct star *star;

star createStar(int x, int y);

void drawStar(star s, Window window);

int detectCollision(charac c, star s);

void freeStar(star s);
