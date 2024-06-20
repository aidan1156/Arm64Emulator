#include <stdlib.h>
#include "./alien.h"

typedef struct alien {
    int posX;
    int posY;
    int startX;
    int startY;
    int dir; //dir: 0 is vertical and 1 is hoz
} *alien;

alien createAlien(int startx, int starty, int setDir) { 
    alien a = malloc(sizeof(struct alien));
    a -> posX = startx;
    a -> posY = starty;
    a -> startX = startx;
    a -> startY = starty;
    a -> dir = setDir;
    return a;
}

void drawAlien(alien a, Window window) {
    setPixel(window, a -> posX, a -> posY, 'x');
}

void updateAlien(alien a, bool pos) {
    if  (a -> dir == 0) {
        if ((a -> startX) == (a -> posX)) {
            if (pos) {
                (a -> posX) ++;
            } else {
                (a -> posX) --;
            }
        } else if ((a -> startX - 1) == (a -> posX)) {
            if (pos) {
                (a -> posX) ++;
            } else {
                (a -> posX) --;
            }
        } else if ((a -> startX - 2) == (a -> posX)) {
            (a -> posX) ++;
        } else if ((a -> startX + 1) == (a -> posX)) {
            if (pos) {
                (a -> posX) ++;
            } else {
                (a -> posX) --;
            }
        } else {
            (a -> posX) --;
        }
    } else {
        if ((a -> startY) == (a -> posY)) {
            if (pos) {
                (a -> posY) ++;
            } else {
                (a -> posY) --;
            }
        } else if ((a -> startY - 1) == (a -> posY)) {
            if (pos) {
                (a -> posY) ++;
            } else {
                (a -> posY) --;
            }
        } else if ((a -> startY - 2) == (a -> posY)) {
            (a -> posY) ++;
        } else if ((a -> startY + 1) == (a -> posY)) {
            if (pos) {
                (a -> posY) ++;
            } else {
                (a -> posY) --;
            }
        } else {
            (a -> posY) --;
        }
    }
}

bool detectDeath(charac c, alien a) {
    if (getPosX(c) == a -> posX && getPosY(c) == a -> posY) {
        return true;
    }
    return false;
}

void freeAlien(alien a) {
    free(a);
}
