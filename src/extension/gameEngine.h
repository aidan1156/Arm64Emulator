#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_KEYPRESSES 512

typedef struct Window *Window;

typedef char* (*LookupFunc)(char);

extern volatile bool quitGame; // Declare quitGame as extern

void engineInit(void);

void engineQuit(Window window);

Window createWindow(int width, int height, LookupFunc LookupFunc);

void fillWindow(Window window, char fill);

void drawWindow(Window window);

void setPixel(Window window, int x, int y, char value);

char getPixel(Window window, int x, int y);

char* getKeyPresses(void);

int getWindowWidth(Window window);

int getWindowHeight(Window window);

bool getQuit(void);

void tick(int delay);

#endif // GAME_ENGINE_H