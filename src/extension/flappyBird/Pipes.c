
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include "./Pipes.h"


// number of pixels between pipes
#define PIPE_SPACING 12
#define PIPE_GAP 6
#define PIPE_WIDTH 4

struct Pipe {
    int x;
    int gap;
};

typedef struct Pipe Pipe;

// define the array of pipes as Pipes
// typedef Pipe** Pipes;

typedef struct Pipes {
    Pipe** pipes;
    int width;
    int height;
} *Pipes;


static int getMaxPipes(int width) {
    return (width / PIPE_SPACING) + 2;
}


Pipes createPipes(int width, int height) {
    // initialise the random number generation
    srand(time(NULL));

    // calculate the max number of pipes that can be on screen then + 2 for safety
    int maxPipes = getMaxPipes(width);
    Pipes result = malloc(sizeof(struct Pipes));
    assert(result != NULL);

    result -> width = width;
    result -> height = height;
    result -> pipes = malloc(sizeof(Pipe*) * maxPipes);
    assert(result -> pipes != NULL);

    resetPipes(result);

    return result;
}

static Pipe* createPipe(int x, int height) {
    Pipe* result = malloc(sizeof(struct Pipe));
    assert(result != NULL);

    result -> x = x;

    int padding = (PIPE_GAP / 2) + 1; // the padding from each edge of the screen where the centre of the pipe can be
    result -> gap = (rand() % (height - padding * 2)) + padding; 

    return result;
}

void resetPipes(Pipes pipes) {
    for (int i=0; i<getMaxPipes(pipes -> width); i++) {
        pipes -> pipes[i] = NULL;
    }
    pipes -> pipes[0] = createPipe(pipes -> width - PIPE_WIDTH - 3, pipes -> height);
}



void updatePipes(Pipes pipes, int* score) {
    // find the right most pipe
    int rightMostX = 0;
    for (int i=0; i<getMaxPipes(pipes -> width); i++) {
        if (pipes -> pipes[i] != NULL && pipes -> pipes[i] -> x > rightMostX) {
            rightMostX = pipes -> pipes[i] -> x;
        }
    }

    // we need to create another pipe
    if (pipes -> width - PIPE_SPACING > rightMostX) {
        for (int i=0; i<getMaxPipes(pipes -> width); i++) {
            // find a free index
            if (pipes -> pipes[i] == NULL) {
                pipes -> pipes[i] = createPipe(pipes -> width, pipes -> height);

                // stop
                break;
            }
        }
    }


    // loop over each pipe and move it
    for (int i=0; i<getMaxPipes(pipes -> width); i++) {
        if (pipes -> pipes[i] != NULL) {
            pipes -> pipes[i] -> x -= 1;

            // if the pipe is now offscreen delete it
            if (pipes -> pipes[i] -> x < -PIPE_WIDTH) {
                free(pipes -> pipes[i]);
                pipes -> pipes[i] = NULL;
                *score += 1;
            }
        }
    }
}

void drawPipes(Pipes pipes, Window window) {
    for (int i=0; i<getMaxPipes(pipes -> width); i++) {
        Pipe* pipe = (pipes -> pipes[i]);
        if (pipe != NULL) {
            for (int j=0; j<PIPE_WIDTH; j++) {
                for (int k=0; k<pipes -> height; k++) {
                    // if this isnt in the gap
                    if (k < pipe -> gap - PIPE_GAP / 2 || k > pipe -> gap + PIPE_GAP / 2) {
                        // if this isnt offscreen
                        if (pipe -> x + j >= 0 && pipe -> x + j < pipes -> width) {
                            setPixel(window, pipe -> x + j, k, 'H');
                        }
                    }
                }
            }
        }
    }
}


bool testIntersect(Pipes pipes, int x, int y) {
    for (int i=0; i<getMaxPipes(pipes -> width); i++) {
        Pipe* pipe = pipes -> pipes[i];
        if (pipe != NULL) {
            if (y < pipe -> gap - PIPE_GAP / 2 || y > pipe -> gap + PIPE_GAP / 2) {
                if (x >= pipe -> x && x < pipe -> x + PIPE_WIDTH) {
                    return true;
                }
            }
        }
    }
    return false;
}

