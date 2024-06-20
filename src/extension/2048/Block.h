
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../gameEngine.h"

typedef struct Block *Block;

typedef struct BlockArray {
    Block* blocks;
    int size;
} BlockArray;

Block createBlock(BlockArray existingBlocks);

void drawBlock(Block block, Window window);