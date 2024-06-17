
#include "./ball.h"

typedef struct Player *Player;

Player createPlayer(int x, int windowWidth, int windowHeight);

void movePlayer(Player player, int diff);

void drawPlayer(Player player, Window window);

int detectCollision(Player player, int x, int y);
