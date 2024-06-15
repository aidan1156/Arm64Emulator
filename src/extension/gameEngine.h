
#include <stdbool.h>


typedef struct Window *Window;

typedef char* (*LookupFunc)(char);

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