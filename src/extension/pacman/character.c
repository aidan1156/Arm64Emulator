#include <stdlib.h>
#include "./character.h"
#include "./walls.h"

typedef struct charac {
    int posX;
    int posY;
    int windowHeight;
    int windowWidth;
} *charac;

charac createCharac(int height, int width) {
    charac charac = malloc(sizeof(struct charac));
    charac -> windowHeight = height;
    charac -> windowWidth = width;
    resetCharac(charac);
    return charac;
}

void drawCharac(charac c, Window window) {
    setPixel(window, c -> posX, c -> posY, '.');
}

void moveCharac(Window win, charac c, int dir) {
    switch (dir) {
        case 1: //up
            if  (!testIntersectWalls(win, (c -> posX), (c -> posY - 1))) {
                (c -> posY) --;
            }
            break;
        case 2: //down
            if  (!testIntersectWalls(win, (c -> posX), (c -> posY + 1))) {
                (c -> posY) ++;
            }
            break;
        case 3: //right
            if  (!testIntersectWalls(win, (c -> posX + 1), (c -> posY))) {
                (c -> posX) ++;
            }
            break;
        case 4: //left
            if  (!testIntersectWalls(win, (c -> posX - 1), (c -> posY))) {
                (c -> posX) --;
            }
            break;
    }

}

bool detectDeath(charac c, alien a) {
    if (c -> posX == getAlienPosX(a) && c -> posY == getAlienPosY(a)) {
        return true;
    }
    return false;
}

void resetCharac(charac c) {
    c -> posX = c -> windowWidth / 2;
    c -> posY = c -> windowHeight / 2;
}

void freeCharac(charac c) {
    free(c);
}
