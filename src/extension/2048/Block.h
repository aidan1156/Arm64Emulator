
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../gameEngine.h"

typedef struct Block {
    int posX;
    int posY;
    int value;
} Block;


typedef struct BlockArray {
    Block* blocks;
    int size;
} BlockArray;

Block createBlock(BlockArray existingBlocks);

void drawBlock(Block block, Window window);

void drawBlockArray(Window window, BlockArray array);