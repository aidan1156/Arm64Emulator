

#include <termios.h> // used to hide terminal input
#include <unistd.h> // also used to hide terminal input
#include <pthread.h> // used to create a thread specifically to get input from terminal
#include <stdio.h> 
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


typedef struct window *Window;

void engineInit(void);

void engineQuit(void);

Window createWindow(int width, int height);

void fillWindow(Window window, char fill);

void drawWindow(Window window);

void setPixel(Window window, int x, int y, char value);

char getPixel(Window window, int x, int y);

bool getEnterPressed();