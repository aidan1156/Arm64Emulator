#include <string.h>
#include <stdlib.h>
#include "../gameEngine.h"
#include "./character.h"
#include "./walls.h"
#include "./alien.h"
#include "./stars.h"
#include "../flappyBird/text.h"

#define WINDOW_WIDTH 25
#define WINDOW_HEIGHT 25

// used to look up character and replace them with colourful emoji!
static char* charLookup(char c) {
    switch (c) {
        case '.':
            return "🙃";
        case 'w':
            return "🧱";
        case '!':
            return "⭐";
        case 'x':
            return "👾";
        case ' ':
            return "🔹";
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
        case ';':
            return "💀";
        case '*':
            return "🎈";
    }
    
    return "🔹";
}

int main(void) {
    engineInit();
    Window window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, &charLookup);
    int winW = getWindowWidth(window);
    int winH = getWindowHeight(window);
    charac pacCharac = createCharac(WINDOW_HEIGHT, WINDOW_WIDTH);

    //aliens
    alien a1 = createAlien(winW/4, winH/6, 0);
    alien a2 = createAlien(winW/4, winH/2, 0);
    alien a3 = createAlien(winW/4, winH*5/6, 0);
    alien a4 = createAlien(winW*4/5, winH/2, 1);

    //stars
    star s1 = createStar(winW/8, winH/6);
    star s2 = createStar(winW/2, winH/6);
    star s3 = createStar(winW*4/5, winH/6);
    star s4 = createStar(winW/8, winH/2);
    star s5 = createStar(winW/8, winH*5/6);
    star s6 = createStar(winW*4/5, winH*5/6);


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
                setPixel(window, winW/2, winH/2, ';');
                setPixel(window, winW/2 + 2, winH/2, ';');
                setPixel(window, winW/2 - 2, winH/2, ';');
                drawScore(window, score);
                drawWindow(window);
                isPlaying = false;
                engineQuit(window);
                exit(0);
            }
        }

        //build window
        fillWindow(window, ' ');
        drawWalls(window);
        drawAlien(a1, window); drawAlien(a2, window); drawAlien(a3, window); drawAlien(a4, window);
        drawStar(s1, window); drawStar(s2, window); drawStar(s3, window); 
        drawStar(s4, window); drawStar(s5, window);  drawStar(s6, window);
        drawCharac(pacCharac, window);
        score += detectCollision(pacCharac, s1) + detectCollision(pacCharac, s2) + detectCollision(pacCharac, s3);
        score += detectCollision(pacCharac, s4) + detectCollision(pacCharac, s5) + detectCollision(pacCharac, s6);
        drawScore(window, score);
        drawWindow(window);
        if (!isPlaying) {isPlaying = true;} 
        if (score == 30) {
            setPixel(window, winW/2, winH/2, '*');
            setPixel(window, winW/2 + 2, winH/2, '*');
            setPixel(window, winW/2 - 2, winH/2, '*');
            drawScore(window, score);
            drawWindow(window);
            isPlaying = false;
            engineQuit(window);
            exit(0);
        }
        tick(200);
        quit = getQuit();
    }
    engineQuit(window);
    exit(0);
}