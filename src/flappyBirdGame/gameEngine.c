
#include <termios.h> // used to hide terminal input
#include <unistd.h> // also used to hide terminal input
#include <pthread.h> // used to create a thread specifically to get input from terminal
#include <stdio.h> 
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


static bool enterPressed = false;
static pthread_t inputThreadId;

typedef struct window {
    char* pixels;
    int width;
    int height;
} *Window;

static void* inputThread(void *vargp) { 
    while (true) {
        getc(stdin);
        fflush(stdin);
        enterPressed = true;
    }
    return NULL; 
} 

static char* charLookup(char c) {
    switch (c) {
        case '.':
            return "🐦";
        case ' ':
            return "🟦";
        case 'H':
            return "🟩";
    }
    return "🟦";
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
    // disable echo
    termios.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);

    // start the thread to get user input  
    pthread_create(&inputThreadId, NULL, inputThread, NULL); 
}

void engineQuit(Window window) {
    // reenable the terminal echoing
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    term.c_lflag |= ECHO;
    tcsetattr(fileno(stdin), 0, &term);

    // shut down the input thread
    pthread_cancel(inputThreadId);

    // free the window
    free(window -> pixels);
    free(window);
}

Window createWindow(int width, int height) {
    Window result = malloc(sizeof(struct window));
    assert(result != NULL);
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
            printf("%s", charLookup(getPixel(window, j, i)));
        }
        putc('\n', stdout);
    }
    fflush(stdout);
}

bool getEnterPressed(void) {
    bool result = enterPressed;
    enterPressed = false;
    return result;
}

int getWindowWidth(Window window) {
    return window -> width;
}

int getWindowHeight(Window window) {
    return window -> height;
}