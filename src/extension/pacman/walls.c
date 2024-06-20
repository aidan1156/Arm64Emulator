
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "./walls.h"


void drawWalls(Window win) {
    int w = getWindowWidth(win);
    int h = getWindowHeight(win);
    for (int i=0; i<w; i++) {
        setPixel(win, i, 0, 'w');
        setPixel(win, i, h-1, 'w');
    }
    for (int j=0; j<h; j++) {
        setPixel(win, 0, j, 'w');
        setPixel(win, w-1, j, 'w');
    }
    for (int i=0; i<w/2 - 1; i++) {
        setPixel(win, i, (h-1)*2/3, 'w');
        setPixel(win, i, (h-1)/3, 'w');
    }
    for (int j=0; j<h/3; j++) {
        setPixel(win, w*2/3, j, 'w');
    }
    for (int j=h*2/3; j<h; j++) {
        setPixel(win, w*2/3, j, 'w');
    }
}

bool testIntersectWalls(Window win, int x, int y) {
    int w = getWindowWidth(win);
    int h = getWindowHeight(win);
    if (x == w-1 || x == 0 || y == h-1 || y == 0){ //dont hit edges
        return true;
    } else if (x < w/2 - 1 && (y == (h-1)*2/3 || y == (h-1)/3)) {
        return true;
    } else if (x == w*2/3 && (y < h/3 || y >= h*2/3)) {
        return true;
    }
    return false;
}

