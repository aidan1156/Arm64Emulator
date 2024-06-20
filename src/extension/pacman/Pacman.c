#include <string.h>
#include <stdlib.h>
#include "../gameEngine.h"
#include "./character.h"
#include "./walls.h"
#include "./alien.h"
#include "../flappyBird/text.h"

#define WINDOW_WIDTH 25
#define WINDOW_HEIGHT 25

// used to look up character and replace them with colourful emoji!
static char* charLookup(char c) {
    switch (c) {
        case '.':
            return "🙃";
            break;
        case 'w':
            return "🧱";
            break;
        case '!':
            return "⭐";
            break;
        case 'x':
            return "👾";
            break;
        case ' ':
            return "🔹";
            break;
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
    
    return "🔹";
}

int main(void) {
    engineInit();
    Window window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, &charLookup);
    charac pacCharac = createCharac(WINDOW_HEIGHT, WINDOW_WIDTH);
    alien a1 = createAlien(getWindowWidth(window)/4, getWindowHeight(window)/6, 0);
    alien a2 = createAlien(getWindowWidth(window)/4, getWindowHeight(window)/2, 0);
    alien a3 = createAlien(getWindowWidth(window)/4, getWindowHeight(window)*5/6, 0);
    alien a4 = createAlien(getWindowWidth(window)*4/5, getWindowHeight(window)/2, 1);

    bool quit = getQuit();
    bool isPlaying = false;
    int count = 0;
    int score = 0;
    
    while (!quit) {
        char* keyPresses = getKeyPresses();
        //move character using wasd
        if (strlen(keyPresses) > 0) {
            if (!strcmp(keyPresses, "w")) {
                moveCharac(window, pacCharac, 1);
            } else if (!strcmp(keyPresses, "s")) {
                moveCharac(window, pacCharac, 2);
            } else if (!strcmp(keyPresses, "d")) {
                moveCharac(window, pacCharac, 3);
            } else if (!strcmp(keyPresses, "a")) {
                moveCharac(window, pacCharac, 4);
            } 
        }

        //update aliens
        if (isPlaying) {
            if (count % 2 == 0) {
                if (count < 6) {
                    updateAlien(a1, true); updateAlien(a2, true); updateAlien(a3, true); updateAlien(a4, true);
                } else {
                    updateAlien(a1, false); updateAlien(a2, false); updateAlien(a3, false); updateAlien(a4, false);
                }
            }
            
            if (count == 12) {
                count = 0;
            } else {
                count ++;
            }
            if (detectDeath(pacCharac, a1) || detectDeath(pacCharac, a2) || 
            detectDeath(pacCharac, a3) || detectDeath(pacCharac, a4)) {
                isPlaying = false;
                engineQuit(window);
                exit(0);
            }
        }

        //build window
        fillWindow(window, ' ');
        drawWalls(window);
        drawAlien(a1, window); drawAlien(a2, window); drawAlien(a3, window); drawAlien(a4, window);
        drawCharac(pacCharac, window);
        drawScore(window, score);
        drawWindow(window);
        if (!isPlaying) {isPlaying = true;} 
        tick(200);
        quit = getQuit();
    }
    engineQuit(window);
    exit(0);
}