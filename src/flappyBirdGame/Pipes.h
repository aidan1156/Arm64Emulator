
#include "./gameEngine.h"


typedef struct Pipe Pipe;

typedef struct Pipes *Pipes;


Pipes createPipes(int width, int height);

void resetPipes(Pipes pipes);

void updatePipes(Pipes pipes, int* score);

void drawPipes(Pipes pipes, Window window);

bool testIntersect(Pipes pipes, int x, int y);