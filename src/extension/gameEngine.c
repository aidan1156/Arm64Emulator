
#include <termios.h> // used to hide terminal input
#include <unistd.h> // also used to hide terminal input
#include <pthread.h> // used to create a thread specifically to get input from terminal
#include <signal.h> // used to intercept ctrl+c
#include <stdio.h> 
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYPRESSES 512


typedef char* (*LookupFunc)(char);

// Window
// char* pixels : pointer to chars
// pixels is a 1D array's pointer (which is a flattened version of the 2D array)
// int width
// int height
//
typedef struct Window {
    LookupFunc lookupFunc;
    char* pixels;
    int width;
    int height;
} *Window;


static pthread_t inputThreadId; // thread handling inputs
static pthread_mutex_t inputLock;
static char keyPresses[MAX_KEYPRESSES]; // buffer containing key presses
static struct termios oldTerminal; // old terminal settings
static volatile bool quitGame = false; // if we should quit the game
static clock_t lastTick; // the last time we ticked, initialised as clock(); = current time


// gets a character from stdin
// if it is less than the MAX_KEYPRESSES,
// adds to the end of keyPresses[]
// 
static void* inputThread(void *vargp) { 
    while (true) {
        char c = getc(stdin);
        pthread_mutex_lock(&inputLock); 
        int len = strlen(keyPresses);
        if (len + 1 < MAX_KEYPRESSES) {
            keyPresses[len + 1] = '\0';
            keyPresses[len] = c;
        }
        pthread_mutex_unlock(&inputLock); 
    }
    return NULL; 
} 


// for a given x,y coordinate,
// calculates the index for 1D array (windows -> pixels)
// 
static int getPixelIndex(Window window, int x, int y){
    return x + y * (window -> width);
}


// if quiteHandler is called, quitGame = true
//
void quitHandler(int a) {
    quitGame = true;
}


// sets given x,y pixel to the value given
//
void setPixel(Window window, int x, int y, char value) {
    window -> pixels[getPixelIndex(window, x, y)] = value;
}


// getting pixel value from given x,y coordinate
//
char getPixel(Window window, int x, int y) {
    return window -> pixels[getPixelIndex(window, x, y)];
}


// initialising engine
// setting up STDIN to have no echo and no wait for enter
// starting thread for user input
// initialising the global variable lastTick to be the current time = clock()
//
void engineInit(void) {
    // disable terminal input showing up
    // get terminal attributes
    struct termios termios;
    tcgetattr(STDIN_FILENO, &termios);
    oldTerminal = termios;
    
    termios.c_lflag &= ~ECHO; // disable echo
    termios.c_lflag &= ~ICANON; // dont wait for enter to read input
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);

    // empty the key presses buffer
    keyPresses[0] = '\0';
    pthread_mutex_init(&inputLock, NULL);

    // start the thread to get user input  
    pthread_create(&inputThreadId, NULL, inputThread, NULL); 

    signal(SIGINT, quitHandler);

    lastTick = clock();
}


// restoring terminal settings
// shut down input thread
// free the malloced windows
//
void engineQuit(Window window) {
    // restore terminal settings
    tcsetattr( STDIN_FILENO, TCSANOW, &oldTerminal);

    // shut down the input thread
    pthread_cancel(inputThreadId);

    // free the window
    free(window -> pixels);
    free(window);
}


// given width, height
// loopup function: used to replace characters with emojis / strings.
// returns the initialised window
// 
Window createWindow(int width, int height, LookupFunc lookup) {
    Window result = malloc(sizeof(struct Window));
    assert(result != NULL);
    result -> lookupFunc = lookup;
    result -> width = width;
    result -> height = height;
    result -> pixels = malloc(sizeof(char) * width * height);
    assert(result -> pixels != NULL);

    return result;
}


// filling given window with given char
//
void fillWindow(Window window, char fill) {
    for (int i=0; i<window -> height * window -> width; i++) {
        window -> pixels[i] = fill;
    }
}


// printing Window to terminal
//
void drawWindow(Window window) {
    puts("\n\n\n\n\n\n\n\n\n\n\n");
    for (int i=0; i<window -> height; i++) {
        for (int j=0; j<window -> width; j++) {
            printf("%s", (window -> lookupFunc)(getPixel(window, j, i)));
        }
        putc('\n', stdout);
    }
    fflush(stdout);
}


// mallocing for key pressing
// flushing the old values and return the new keys pressed 
//
char* getKeyPresses(void) {
    pthread_mutex_lock(&inputLock); 
    char* result = malloc((strlen(keyPresses) + 1) * sizeof(char));
    strcpy(result, keyPresses);
    keyPresses[0] = '\0';
    pthread_mutex_unlock(&inputLock); 

    return result;
}

// width return
//
int getWindowWidth(Window window) {
    return window -> width;
}

// height return
//
int getWindowHeight(Window window) {
    return window -> height;
}


// get value of quitGame
//
bool getQuit(void) {
    return quitGame;
}


// waits for given delay time,
// then sets the lastTick to current time again
//
void tick(int delay) {
    while (clock() < lastTick + delay * 1000);
    lastTick = clock();
}

// draw a window onto a window (lol)
void drawWindowOntoWindow(Window src, Window dest, int x, int y) {
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    for (int i=x; i<src->width && i<dest->width; i++) {
        for (int j=y;j<src->height && j<dest->height; j++) {
            setPixel(dest, i, j, getPixel(src, i, j));
        }
    }
}