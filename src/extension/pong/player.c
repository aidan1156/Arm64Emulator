
#include "../gameEngine.h"
#include "./player.h"


#include <stdlib.h>

#define PLAYER_SIZE 5

typedef struct Player {
    int x;
    int y;
    int windowWidth;
    int windowHeight;
    int score;
} *Player;

Player createPlayer(int x, int windowWidth, int windowHeight) {
    Player result = malloc(sizeof(struct Player));
    result -> x = x;
    result -> windowHeight = windowHeight;
    result -> windowWidth = windowWidth;
    result -> score = 0;
    resetPlayer(result);

    return result;
}

void movePlayer(Player player, int diff) {
    player -> y += diff;
    if (player -> y < 0) {
        player -> y = 0;
    }
    if (player -> y + PLAYER_SIZE > player -> windowHeight) {
        player -> y = player -> windowHeight - PLAYER_SIZE;
    }
}

void drawPlayer(Player player, Window window) {
    for (int i=player -> y;i<player -> y + PLAYER_SIZE; i++) {
        setPixel(window, player -> x, i, '.');
    }
    int x = player -> windowWidth / 2 - 1;
    if (player -> x > player -> windowWidth / 2) {
        x = player -> windowWidth / 2 + 1;
    }
    setPixel(window, x, 0, '0' + player -> score);
}

int detectCollision(Player player, int x, int y) {
    int direction = 1;
    if (player -> x > player -> windowWidth / 2) {
        direction = -1;
    }

    if (x == player -> x + direction) {
        if (y >= player -> y && y <= player -> y + PLAYER_SIZE) {
            if (y < player -> y + 2) {
                return 0;
            } else if (y > player -> y + PLAYER_SIZE - 2) {
                return 2;
            } else {
                return 1;
            }
        }
    }
    return -1;
}

void resetPlayer(Player player) {
    player -> y = player -> windowHeight / 2 - PLAYER_SIZE / 2;
}

void incrementScore(Player player) {
    player -> score++;
}

int getScore(Player player) {
    return player -> score;
} 