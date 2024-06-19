#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "../gameEngine.h"

#define MAP_WIDTH 40
#define MAP_HEIGHT 20

char grid[MAP_HEIGHT][MAP_WIDTH];
char newGrid[MAP_HEIGHT][MAP_WIDTH];

char* lookupFunc(char pixel) {
    static char alive[] = "⬜";
    static char dead[] = "⬛";
    switch (pixel) {
        case '1': return alive;
        case '0': return dead;
        default: return dead;
    }
}

// init with a random grid
void initializeGame() {
    engineInit();
    srand(time(NULL));
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = rand() % 2 ? '1' : '0';
        }
    }
}

// count alive neighbors
int countAliveNeighbors(int y, int x) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int ny = y + i;
            int nx = x + j;
            if (ny >= 0 && ny < MAP_HEIGHT && nx >= 0 && nx < MAP_WIDTH) {
                if (grid[ny][nx] == '1') {
                    count++;
                }
            }
        }
    }
    return count;
}

// update the grid and check neighbours for next tick
void updateGrid() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int aliveNeighbors = countAliveNeighbors(y, x);
            if (grid[y][x] == '1') {
                newGrid[y][x] = (aliveNeighbors == 2 || aliveNeighbors == 3) ? '1' : '0';
            } else {
                newGrid[y][x] = (aliveNeighbors == 3) ? '1' : '0';
            }
        }
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = newGrid[y][x];
        }
    }
}

// Draw the grid
void drawGrid(Window window) {
    fillWindow(window, '0');
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            setPixel(window, x, y, grid[y][x]);
        }
    }
    drawWindow(window);
}

// Main game loop
int main() {
    initializeGame();
    Window window = createWindow(MAP_WIDTH, MAP_HEIGHT, lookupFunc);
    bool running = true;

    while (!getQuit()) {
        if (running) {
            updateGrid();
            drawGrid(window);
        }

        char *keyPresses = getKeyPresses();
        for (int i = 0; keyPresses[i] != '\0'; i++) {
            char key = keyPresses[i];
            if (key == 'q') {
                engineQuit(window);
                return 0; // exit 
            } else if (key == 'p') {
                running = !running; // pause or resume
            } else if (key == 'r') {
                initializeGame(); // reset
            }
        }
        free(keyPresses);

        tick(100);
    }

    engineQuit(window);
    return 0;
}



