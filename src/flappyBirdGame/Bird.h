

typedef struct Bird *Bird;

Bird createBird(int height);

void drawBird(Bird bird, Window window);

void updateBird(Bird bird, Window window);

void flapBird(Bird bird);

bool detectDeath(Bird bird);

void resetBird(Bird bird);