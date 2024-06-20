
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

void initBlockArray(BlockArray array);

void deleteBlock(BlockArray blockArray, int index);

void addBlock(BlockArray blockArray, Block block);

int findIndex(int x, int y, BlockArray blockArray);

Block createBlock(BlockArray existingBlocks);

void drawBlock(Block block, Window window);

void drawBlockArray(Window window, BlockArray array);

void mergeBlocks(Block block1, Block block2, BlockArray array, int newX, int newY);

bool updateBlocks(BlockArray* array, char keyPress);