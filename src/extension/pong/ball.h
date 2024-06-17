
#include "../gameEngine.h"
#include "./player.h"

typedef struct Ball *Ball;

Ball createBall(int windowWidth, int windowHeight);

void updateBall(Ball ball);

void drawBall(Ball ball, Window window);
