
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

    result -> windowHeight = windowHeight;
    result -> windowWidth = windowWidth;
    
    resetBall(result);

    return result;
}

void updateBall(Ball ball, Player* players) {
    ball -> x += ball -> xChange;
    ball -> y += ball -> yChange;

    if (ball -> y <= 0 || ball -> y >= ball -> windowHeight - 1) {
        ball -> yChange *= -1;
    }

    for (int i=0; players[i] != NULL; i++) {
        int collision = detectCollision(players[i], ball -> x, ball -> y);
        if (collision != -1) {
            switch (collision) {
                case 0:
                    ball -> xChange *= -1;
                    ball -> yChange = -1;
                    break;
                case 1:
                    ball -> xChange *= -1;
                    ball -> yChange = 0;
                    break;
                case 2:
                    ball -> xChange *= -1;
                    ball -> yChange = 1;
                    break;
            }
        }
    }
}

void drawBall(Ball ball, Window window) {
    setPixel(window, ball -> x, ball -> y, '.');
}

void resetBall(Ball ball) {
    ball -> x = ball -> windowWidth / 2;
    ball -> y = ball -> windowHeight / 2;
    ball -> xChange = (rand() % 2 == 0) ? 1 : -1;
    ball -> yChange = 0;
}

int detectRoundEnd(Ball ball) {
    if (ball -> x == 0) {
        return -1;
    }
    if (ball -> x == ball -> windowWidth - 1) {
        return 1;
    }
    return 0;
}
