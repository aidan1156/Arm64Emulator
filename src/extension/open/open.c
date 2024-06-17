#include <termios.h> // For terminal settings
#include <unistd.h> // For STDIN_FILENO and other POSIX functions
#include <pthread.h> // For threading
#include <signal.h> // For signal handling
#include <stdio.h> // For standard input/output
#include <stdbool.h> // For boolean type
#include <assert.h> // For assertions
#include <stdlib.h> // For memory allocation and other utilities
#include <string.h> // For string manipulation
#include <time.h> // For time-related functions

#include "../gameEngine.h"

#define MAP_WIDTH 20
#define MAP_HEIGHT 10

typedef struct {
    char items[10][20];
    int itemCount;
} Inventory;

typedef struct {
    int health;
    int attack;
} Entity;

// Example map
char gameMap[MAP_HEIGHT][MAP_WIDTH] = {
    "####################",
    "#..#...............#",
    "#..#..#####..#.....#",
    "#..#..#...#..#.....#",
    "#......#...#.......#",
    "#####..#####..#####.",
    "#...........#.......",
    "#..######...#.......",
    "#...........#.......",
    "####################"
};

// Function to lookup characters for rendering
char* lookupFunc(char pixel) {
    static char empty[] = " ";
    static char wall[] = "#";
    static char player[] = "@";
    static char *lookup[] = {empty, wall, player};

    switch (pixel) {
        case ' ': return empty;
        case '#': return wall;
        case '@': return player;
        default: return empty;
    }
}

// Initialize the game
void initializeGame() {
    engineInit();
}

// Draw the map
void drawMap(Window window, int playerX, int playerY) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (i == playerY && j == playerX) {
                setPixel(window, j, i, '@'); // Player character
            } else {
                setPixel(window, j, i, gameMap[i][j]);
            }
        }
    }
    drawWindow(window);
}

// Move the player
void movePlayer(char direction, int *playerX, int *playerY) {
    int newX = *playerX, newY = *playerY;
    if (direction == 'w') newY--;
    if (direction == 's') newY++;
    if (direction == 'a') newX--;
    if (direction == 'd') newX++;
    if (gameMap[newY][newX] != '#') { // Check for walls
        *playerX = newX;
        *playerY = newY;
    }
}

// Add an item to inventory
void addItem(Inventory *inventory, const char *item) {
    if (inventory->itemCount < 10) {
        strcpy(inventory->items[inventory->itemCount], item);
        inventory->itemCount++;
    }
}

// Show inventory
void showInventory(Inventory *inventory) {
    printf("Inventory:\n");
    for (int i = 0; i < inventory->itemCount; i++) {
        printf("%s\n", inventory->items[i]);
    }
}

// Combat system
void combat(Entity *player, Entity *enemy) {
    while (player->health > 0 && enemy->health > 0) {
        enemy->health -= player->attack;
        if (enemy->health > 0) {
            player->health -= enemy->attack;
        }
    }
    if (player->health <= 0) {
        printf("You have been defeated!\n");
        quitGame = true;
    } else {
        printf("You defeated the enemy!\n");
    }
}

// Main game loop
int main() {
    initializeGame();
    Window window = createWindow(MAP_WIDTH, MAP_HEIGHT, lookupFunc);

    int playerX = 1, playerY = 1;
    Inventory inventory = {.itemCount = 0};

    while (!getQuit()) {
        drawMap(window, playerX, playerY);
        char *keyPresses = getKeyPresses();
        for (int i = 0; keyPresses[i] != '\0'; i++) {
            char key = keyPresses[i];
            if (key == 'q') quitGame = true;
            else if (key == 'i') showInventory(&inventory);
            else movePlayer(key, &playerX, &playerY);
        }
        free(keyPresses);
        tick(200); // Delay for smoother gameplay
    }

    engineQuit(window);
    return 0;
}
