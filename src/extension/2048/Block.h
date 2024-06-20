
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../gameEngine.h"

typedef struct Block {
    int posX; // pos is used for drawBlock()
    int posY;
    int value;
} Block;


typedef struct BlockArray {
    Block* blocks; // flattened out 1D list 
    int size;
} BlockArray;

bool checkIfWon(BlockArray array);

void initBlockArray(BlockArray array);

Block createBlock(BlockArray* existingBlocks);

void drawBlock(Block block, Window window);

void drawBlockArray(Window window, BlockArray array);

bool updateBlocks(BlockArray* array, char keyPress);