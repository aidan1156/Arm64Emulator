#include <time.h>

#include "./flappyBirdGame/gameEngine.h"



int main(void) {
    engineInit();
    Window window = createWindow(15,30);
    int pos = 0;
    while (true) {
        clock_t start_time = clock();
        fillWindow(window, ' ');
        if (getEnterPressed()) {
            pos -= 2;
        } else {
            pos += 1;
        }
        setPixel(window, 0, pos, '.');
        drawWindow(window);
        while (clock() < start_time + 100000);
    }
    return 0;
}