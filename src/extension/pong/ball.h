
#include "../gameEngine.h"
#include "./player.h"

typedef struct Ball *Ball;

Ball createBall(int windowWidth, int windowHeight);

void updateBall(Ball ball, Player* players);

void drawBall(Ball ball, Window window);

void resetBall(Ball ball);

int detectRoundEnd(Ball ball);
