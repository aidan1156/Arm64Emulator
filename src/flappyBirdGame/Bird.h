
#include "./gameEngine.h"
#include "./Pipes.h"

typedef struct Bird *Bird;

Bird createBird(int height);

void drawBird(Bird bird, Window window);

void updateBird(Bird bird);

void flapBird(Bird bird);

bool detectDeath(Bird bird, Pipes pipes);

void resetBird(Bird bird);

void freeBird(Bird bird);