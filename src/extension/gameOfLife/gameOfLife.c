#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../gameEngine.h"

#define MAP_WIDTH 40
#define MAP_HEIGHT 20

char grid[MAP_HEIGHT][MAP_WIDTH];
char newGrid[MAP_HEIGHT][MAP_WIDTH];
int unchangedCount[MAP_HEIGHT][MAP_WIDTH];
bool colorfulMode = true;

char* lookupFunc(char pixel) {
    static char red[] = "🟥";
    static char orange[] = "🟧";
    static char yellow[] = "🟨";
    static char green[] = "🟩";
    static char blue[] = "🟦";
    static char purple[] = "🟪";
    static char white[] = "⬜";
    static char black[] = "⬛";
    if (colorfulMode) {
        switch (pixel) {
            case '0': return black;
            case '1': return red;
            case '2': return orange;
            case '3': return yellow;
            case '4': return green;
            case '5': return blue;
            case '6': return purple;
            default: return black;
        }
    } else {
        switch (pixel) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6': return white;
            case '0': default: return black;
        }
    }
}

void initializeGame() {
    engineInit();
    srand(time(NULL));
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            // randomly assign a cell to be alive / dead 
            grid[y][x] = rand() % 2 ? '1' : '0';
            unchangedCount[y][x] = 0;
        }
    }
}

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

// update grid for next tick
void updateGrid() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int aliveNeighbors = countAliveNeighbors(y, x);
            char newState = '0';
            if (grid[y][x] == '1') {
                newState = (aliveNeighbors == 2 || aliveNeighbors == 3) ? '1' : '0';
            } else {
                newState = (aliveNeighbors == 3) ? '1' : '0';
            }

            if (newState == grid[y][x]) {
                if (unchangedCount[y][x] < 6) unchangedCount[y][x]++;
            } else {
                unchangedCount[y][x] = 1; // immediately turn red if changed
            }

            newGrid[y][x] = newState;
        }
    }

    // Copy new grid to grid
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = newGrid[y][x];
        }
    }
}

void drawGrid(Window window) {
    fillWindow(window, '0');
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            setPixel(window, x, y, grid[y][x] == '0' ? '0' : (char)(unchangedCount[y][x] + '0'));
        }
    }
    drawWindow(window);
}

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
                // exit
                return 0; 
            } else if (key == 'p') { // pause or resume the game
                running = !running; 
            } else if (key == 'r') { // reset game
                initializeGame(); 
            } else if (key == 'c') { // toggle colour mode
                colorfulMode = !colorfulMode; 
            }
        }
        free(keyPresses);

        tick(100);
    }

    engineQuit(window);
    return 0;
}
