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

void initializeGame(void);
void drawMap(Window window, int playerX, int playerY);
void movePlayer(char direction, int *playerX, int *playerY);
void addItem(Inventory *inventory, const char *item);
void showInventory(Inventory *inventory);
void combat(Entity *player, Entity *enemy);
int main(void);