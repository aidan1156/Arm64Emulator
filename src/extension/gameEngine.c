
#include <termios.h> // used to hide terminal input
#include <unistd.h> // also used to hide terminal input
#include <pthread.h> // used to create a thread specifically to get input from terminal
#include <stdio.h> 
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYPRESSES 512


typedef char* (*LookupFunc)(char);

typedef struct Window {
    LookupFunc lookupFunc;
    char* pixels;
    int width;
    int height;
} *Window;



static pthread_t inputThreadId; // thread handling inputs
static struct termios oldTerminal; // old terminal settings
static char keyPresses[MAX_KEYPRESSES];

static void* inputThread(void *vargp) { 
    char c = getc(stdin);
    while (c != EOF) {
        if (strlen(keyPresses) + 1 < MAX_KEYPRESSES) {
            keyPresses[strlen(keyPresses) + 1] = '\0';
            keyPresses[strlen(keyPresses)] = c;
        }
        c = getc(stdin);
    }
    return NULL; 
} 


static int getPixelIndex(Window window, int x, int y){
    return x + y * (window -> width);
}

void setPixel(Window window, int x, int y, char value) {
    window -> pixels[getPixelIndex(window, x, y)] = value;
}

char getPixel(Window window, int x, int y) {
    return window -> pixels[getPixelIndex(window, x, y)];
}

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

    // start the thread to get user input  
    pthread_create(&inputThreadId, NULL, inputThread, NULL); 
}

void engineQuit(Window window) {
    // restore terminal settings
    tcsetattr( STDIN_FILENO, TCSANOW, &oldTerminal);

    // shut down the input thread
    pthread_cancel(inputThreadId);

    // free the window
    free(window -> pixels);
    free(window);
}

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

void fillWindow(Window window, char fill) {
    for (int i=0; i<window -> height * window -> width; i++) {
        window -> pixels[i] = fill;
    }
}

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

char* getKeyPresses(void) {
    char* result = malloc((strlen(keyPresses) + 1) * sizeof(char));
    strcpy(result, keyPresses);
    keyPresses[0] = '\0';

    return result;
}

int getWindowWidth(Window window) {
    return window -> width;
}

int getWindowHeight(Window window) {
    return window -> height;
}