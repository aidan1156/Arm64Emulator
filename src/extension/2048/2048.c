#include <string.h>
#include <stdlib.h>

#include "../gameEngine.h"
#include "./Block.h"

#define WINDOW_WIDTH 19
#define WINDOW_HEIGHT 19
#define MAX_BLOCK_NUM 16

static char* charLookup(char c) {
    switch (c) {
        case ' ':
            return "🟦";
        case '.':
            return "⬜";
        case '0':
            return "0️⃣ ";
        case '1':
            return "1️⃣ ";
        case '2':
            return "2️⃣ ";
        case '3':
            return "3️⃣ ";
        case '4':
            return "4️⃣ ";
        case '5':
            return "5️⃣ ";
        case '6':
            return "6️⃣ ";
        case '7':
            return "7️⃣ ";
        case '8':
            return "8️⃣ ";
        case '9':
            return "9️⃣ ";
    }
    return "🟦";
}

void drawGridLines(Window window) {
    for (int i = 0; i < WINDOW_HEIGHT; i++) { 
        setPixel(window, 4, i, '.');
        setPixel(window, 9, i, '.');
        setPixel(window, 14, i, '.');

        setPixel(window, i, 4, '.');
        setPixel(window, i, 9, '.');
        setPixel(window, i, 14, '.');
    }
}

int main(void) {
    engineInit();
    Window window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, &charLookup);
    fillWindow(window, ' '); // fill in window with blue

    drawGridLines(window); // drawing grid lines

    BlockArray blockArray; // initialising array of blocks
    blockArray.size = 0;
    blockArray.blocks = malloc((MAX_BLOCK_NUM) * sizeof(struct Block)); // dont have to realloc this way
    assert (blockArray.blocks != NULL);
    initBlockArray(blockArray);

    
    bool firstTime = true;
    bool quit = getQuit();

    while (!quit) {
        if (firstTime) {
            createBlock(blockArray); // generate first block if start of game
            firstTime = false;
        }
        
        char* keyPresses = getKeyPresses();
        if (strlen(keyPresses) > 0) {
            // take first keyPress
            // and make any relevant merges of blocks
            // update all blocks
            if (updateBlocks(&blockArray, keyPresses[0])) { // if updated block
                // one of w, s, a, d keys pressed
                createBlock(blockArray);
            }
            
        }
        free(keyPresses);

        fillWindow(window, ' ');
        drawGridLines(window);
        drawBlockArray(window, blockArray);
        drawWindow(window);
        tick(100);
        quit = getQuit();
    }

    
    // Block block = createBlock(blockArray);

    // drawBlock(block, window);
    // drawWindow(window);

    // updateBlocks(&blockArray, 'd'); 


    // fillWindow(window, ' ');
    // drawGridLines(window);
    // drawBlockArray(window, blockArray);
    // drawWindow(window);


    free(blockArray.blocks);

    engineQuit(window);
    exit(0);
}