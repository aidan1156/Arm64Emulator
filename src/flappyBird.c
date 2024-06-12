#include <time.h>
#include <signal.h>

#include "./flappyBirdGame/gameEngine.h"
#include "./flappyBirdGame/Bird.h"

#define WINDOW_WIDTH 20
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

    bool playing = false;

    while (!quit) {
        clock_t start_time = clock();
        fillWindow(window, ' ');
        if (getEnterPressed()) {
            if (!playing) {
                playing = true;
                resetBird(bird);
            }
            flapBird(bird);
        }
        if (playing) {
            // move all the components
            updateBird(bird, window);

            //finally detect if the bird has died
            if (detectDeath(bird)) {
                playing = false;
            }
        }
        drawBird(bird, window);
        drawWindow(window);
        while (clock() < start_time + 100000);
    }

    engineQuit();
    exit(0);
}


