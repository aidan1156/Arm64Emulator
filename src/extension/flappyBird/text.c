
#include "./text.h"

void drawScore(Window window, int score) {
    int width = getWindowWidth(window);

    // find the size of the score in num digits
    int order = 10;
    int size = 1;
    while (order <= score) {
        order *= 10;
        size++;
    }

    // if the width of the screen is even make the score size even
    if (width % 2 == 0 && size % 2 != 0) {
        size++;
    } else if (width % 2 == 1 && size % 2 == 0) { // and vice versa
        size++;
    }

    // set min size
    if (size < 2 && width % 2 == 0) {
        size = 2;
    } else if (size < 3 && width % 2 == 1) {
        size = 3;
    }

    int xpos = width / 2 + size / 2;
    for (int i=0; i<size; i++) {
        setPixel(window, xpos, 0, (score % 10) + '0');
        score = score / 10;
        xpos--;
    }
}

