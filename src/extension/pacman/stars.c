#include <stdlib.h>

#include "../gameEngine.h"
#include "./stars.h"

typedef struct star {
    int posX;
    int posY;
    bool hit;
} *star;

star createStar(int x, int y) { 
    star s = malloc(sizeof(struct star));
    s -> posX = x;
    s -> posY = y;
    s -> hit = false;
    return s;
}

void drawStar(star s, Window window) {
    if (s -> hit){
        setPixel(window, s -> posX, s -> posY, ' ');
    } else {
        setPixel(window, s -> posX, s -> posY, '!');
    }
}

int detectCollision(charac c, star s) {
    if (s -> hit == true) {
        return 0;
    }
    if (getPosX(c) == s -> posX && getPosY(c) == s -> posY) {
        s -> hit = true;
        return 5;
    }
    return 0;
}


void freeStar(star s) {
    free(s);
}
