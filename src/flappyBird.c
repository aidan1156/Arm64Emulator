#include <time.h>
#include <signal.h>
#include <string.h>

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

// used to look up character and replace them with colourful emoji!
static char* charLookup(char c) {
    switch (c) {
        case '.':
            return "🐦";
        case ' ':
            return "🟦";
        case 'H':
            return "🟩";
        case '0':
            return "0️⃣ ";
        case '1':
            return "1️⃣ ";
        case '2':
            return "2️⃣ ";
        case '3':
            return "3️⃣ ";
        case '4':
            return "4️⃣ ";
        case '5':
            return "5️⃣ ";
        case '6':
            return "6️⃣ ";
        case '7':
            return "7️⃣ ";
        case '8':
            return "8️⃣ ";
        case '9':
            return "9️⃣ ";
    }
    return "🟦";
}

int main(void) {
    signal(SIGINT, quitHandler);
    engineInit();
    Window window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, &charLookup);
    Bird bird = createBird(WINDOW_HEIGHT);
    Pipes pipes = createPipes(WINDOW_WIDTH, WINDOW_HEIGHT);
    int score = 0;

    bool playing = false;
    bool firstTime = true;

    while (!quit) {
        clock_t start_time = clock();
        fillWindow(window, ' ');

        char* keyPresses = getkeyPresses();
        if (strlen(keyPresses) > 0) {
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
        free(keyPresses);

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


