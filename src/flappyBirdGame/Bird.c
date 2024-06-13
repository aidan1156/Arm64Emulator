
#include <stdlib.h>


#include "./Bird.h"

#define BIRD_X 1

typedef struct Bird {
    int pos;
    int momentum;
    int windowHeight;
} *Bird;

Bird createBird(int height) {
    Bird bird = malloc(sizeof(struct Bird));
    bird -> windowHeight = height;
    resetBird(bird);

    return bird;
}

void drawBird(Bird bird, Window window) {
    setPixel(window, BIRD_X, bird -> pos, '.');
}

void updateBird(Bird bird) {
    if (bird -> momentum > -2) {
        bird -> momentum -= 1;
    }

    bird -> pos -= bird -> momentum;
    if (bird -> pos < 0) {
        bird -> pos = 0;
    }
}

void flapBird(Bird bird) {
    bird -> momentum = 3;
}

bool detectDeath(Bird bird, Pipes pipes) {
    if (bird -> pos >= bird -> windowHeight - 1) {
        bird -> pos = bird -> windowHeight - 1;
        return true;
    }
    return testIntersect(pipes, BIRD_X, bird -> pos);
}

void resetBird(Bird bird) {
    bird -> pos = bird -> windowHeight / 2;
    bird -> momentum = 0;
}

void freeBird(Bird bird) {
    free(bird);
}