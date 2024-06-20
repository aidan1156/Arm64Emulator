

typedef struct Player *Player;

Player createPlayer(int x, int windowWidth, int windowHeight);

void movePlayer(Player player, int diff);

void drawPlayer(Player player, Window window);

int detectCollision(Player player, int x, int y);

void resetPlayer(Player player);

void incrementScore(Player player);

int getScore(Player player);
