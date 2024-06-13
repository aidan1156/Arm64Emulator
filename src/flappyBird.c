#include <time.h>
#include <signal.h>

#include "./flappyBirdGame/gameEngine.h"
#include "./flappyBirdGame/Bird.h"
#include "./flappyBirdGame/Pipes.h"
#include "./flappyBirdGame/text.h"

#define WINDOW_WIDTH 25
#define WINDOW_HEIGHT 30

static volatile bool quit = false;

void quitHandler(int a) {
    quit = true;
}

int main(void) {
    signal(SIGINT, quitHandler);
    engineInit();
    Window window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    Bird bird = createBird(WINDOW_HEIGHT);
    Pipes pipes = createPipes(WINDOW_WIDTH, WINDOW_HEIGHT);
    int score = 0;

    bool playing = false;
    bool firstTime = true;

    while (!quit) {
        clock_t start_time = clock();
        fillWindow(window, ' ');
        if (getEnterPressed()) {
            if (!playing) {
                playing = true;
                // if this isnt the first game reset components
                if (!firstTime) {
                    resetBird(bird);
                    resetPipes(pipes);
                    score = 0;
                }
                firstTime = false;
            }
            flapBird(bird);
        }
        if (playing) {
            // move all the components
            updatePipes(pipes, &score);
            updateBird(bird);

            //finally detect if the bird has died
            if (detectDeath(bird, pipes)) {
                playing = false;
            }
        }

        drawPipes(pipes, window);
        drawBird(bird, window);
        drawScore(window, score);
        drawWindow(window);
        while (clock() < start_time + 100000);
    }

    engineQuit(window);
    exit(0);
}


