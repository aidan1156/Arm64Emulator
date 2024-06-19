
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../gameEngine.h"
#include "./player.h"
#include "./ball.h"

#define WINDOW_WIDTH 21
#define WINDOW_HEIGHT 21

static char* charLookup(char c) {
    switch (c) {
        case '.':
            return "⬜";
        case ' ':
            return "⬛";
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
    return "⬛";
}

int main(void) {
    engineInit();
    Window window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, &charLookup);

    Player player1 = createPlayer(0, WINDOW_WIDTH, WINDOW_HEIGHT);
    Player player2 = createPlayer(WINDOW_WIDTH - 1, WINDOW_WIDTH, WINDOW_HEIGHT);
    Player players[3] = {player1, player2, NULL};
    Ball ball = createBall(WINDOW_WIDTH, WINDOW_HEIGHT);

    bool quit = getQuit();
    while (!quit) {
        char* keyPresses = getKeyPresses();
        if (strchr(keyPresses, 'w')) {
            movePlayer(player1, -2);
        } else if (strchr(keyPresses, 's')) {
            movePlayer(player1, 2);
        } else if (strchr(keyPresses, 'o')) {
            movePlayer(player2, -2);
        } else if (strchr(keyPresses, 'l')) {
            movePlayer(player2, 2);
        }
        free(keyPresses);

        updateBall(ball, players);

        if (detectRoundEnd(ball)) {
            if (detectRoundEnd(ball) == -1) {
                incrementScore(player2);
            } 
            else {
                incrementScore(player1);
            }
            resetBall(ball);
            resetPlayer(player1);
            resetPlayer(player2);
        }

        fillWindow(window, ' ');
        drawPlayer(player1, window);
        drawPlayer(player2, window);
        drawBall(ball, window);
        drawWindow(window);
        tick(100);
        quit = getQuit();
        if (getScore(player1) == 9 || getScore(player2) == 9) {
            quit = true;
        }
    }

    char* winner = "1";
    if (getScore(player1) == getScore(player2)) {
        winner = "1 and 2";
    } else if (getScore(player1) < getScore(player2)) {
        winner = "2";
    }
    printf("Player %s won\n", winner);

    engineQuit(window);
    exit(0);
}