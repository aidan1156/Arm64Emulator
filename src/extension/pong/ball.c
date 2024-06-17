
#include <stdlib.h>

#include "./ball.h"

typedef struct Ball {
    int x;
    int y;
    int xChange;
    int yChange;
    int windowWidth;
    int windowHeight;
} *Ball;

Ball createBall(int windowWidth, int windowHeight) {
    Ball result = malloc(sizeof(struct Ball));
    
    result -> x = windowWidth / 2;
    result -> y = windowHeight / 2;
    result -> xChange = (rand() % 2 == 0) ? 1 : -1;
    result -> yChange = 0;

    result -> windowHeight = windowHeight;
    result -> windowWidth = windowWidth;

    return result;
}

void updateBall(Ball ball, Player player) {
    ball -> x += ball -> xChange;
    ball -> y += ball -> yChange;

    if (ball -> y <= 0 || ball -> y >= ball -> windowHeight - 1) {
        ball -> yChange *= -1;
    }
}

void drawBall(Ball ball, Window window) {
    setPixel(window, ball -> x, ball -> y, '.');
}
