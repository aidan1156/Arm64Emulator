
#include <stdlib.h>

#include "./gameEngine.h"
#include "./Bird.h"

typedef struct Bird {
    int pos;
    int momentum;
    int windowHeight;
} *Bird;

Bird createBird(int height) {
    Bird bird = malloc(sizeof(Bird));
    bird -> windowHeight = height;
    resetBird(bird);

    return bird;
}

void drawBird(Bird bird, Window window) {
    setPixel(window, 1, bird -> pos, '.');
}

void updateBird(Bird bird, Window window) {
    if (bird -> momentum > -4) {
        bird -> momentum -= 1;
    }

    bird -> pos -= bird -> momentum;
    if (bird -> pos < 0) {
        bird -> pos = 0;
    }
}

void flapBird(Bird bird) {
    bird -> momentum = 4;
}

bool detectDeath(Bird bird) {
    if (bird -> pos >= bird -> windowHeight - 1) {
        bird -> pos = bird -> windowHeight - 1;
        return true;
    }
    return false;
}

void resetBird(Bird bird) {
    bird -> pos = bird -> windowHeight / 2;
    bird -> momentum = 0;
}

