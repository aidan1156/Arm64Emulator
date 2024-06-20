
#include "./Block.h"


typedef struct Block {
    int posX;
    int posY;
    int value;
} *Block;

// deleting block from BlockArray at a given index
void deleteBlock(BlockArray* blockArray, int index) {
    if (index < 0 || index >= blockArray->size) {
        printf("Index out of bounds\n");
        return;
    }

    // Shift elements to the left to fill the gap
    for (int i = index; i < blockArray->size - 1; i++) {
        blockArray->blocks[i] = blockArray->blocks[i + 1];
    }

    // Decrement the size
    blockArray->size--;

    // Reallocate memory to shrink the array
    blockArray->blocks = realloc(blockArray->blocks, blockArray->size * sizeof(Block));
    if (blockArray->blocks == NULL && blockArray->size > 0) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}


// find index of given block position
// returns index if found
// returns -1 if not found
int findIndex(int x, int y, BlockArray blockArray) {
    for (int i = 0; i < blockArray.size; i++) {
        if ((blockArray.blocks[i]->posX == x) & (blockArray.blocks[i]->posY == y)) {
            return i;
        }
    }
    return -1;
}


Block createBlock(BlockArray existingBlocks) {
    assert(existingBlocks.size < 16); 
    // createBlock should not be called when there are 16 existing blocks already

    Block block = malloc(sizeof(struct Block));
    block->value = 2;
    block->posX = rand() % 4;
    block->posY = rand() % 4;

    if (findIndex(block->posX, block->posY, existingBlocks) == -1) {
        block->posX = rand() % 4;
        block->posY = rand() % 4;
    }

    return block;
}

void drawBlock(Block block, Window window) {
    int numDigits = 0;
    if ((block->value/1000) >= 1) { 
        // block can never have value 1000 so using 1000 instead of 999 for readability
        numDigits = 4;
    } else if ((block->value/100) >= 1) { // same goes for 100
        numDigits = 3;
    } else if ((block->value/10) >= 1) { // and so on
        numDigits = 2;
    } else {
        numDigits = 1;
    }

    char valueChar1;
    char valueChar2;
    char valueChar3;
    char valueChar4;
    int newY = block->posY * 5 + 1;
    switch (numDigits) {
        case 1:
            valueChar1 = block->value + '0';
            setPixel(window, block->posX * 5 + 1, newY, valueChar1);
            break;
            
        case 2:
            valueChar1 = (block->value) % 10 + '0';
            setPixel(window, block->posX * 5 + 2, newY, valueChar1);

            valueChar2 = (block->value / 10) % 10 + '0';
            setPixel(window, block->posX * 5 + 1, newY, valueChar1);
            break;

        case 3:
            valueChar1 = (block->value) % 10 + '0';
            setPixel(window, block->posX * 5 + 2, newY, valueChar1);

            valueChar2 = (block->value / 10) % 10 + '0';
            setPixel(window, block->posX * 5 + 1, newY, valueChar2);

            valueChar3 = (block->value / 100) % 10 + '0';
            setPixel(window, block->posX * 5, newY, valueChar3);
            break;

        case 4:
            valueChar1 = (block->value) % 10 + '0';
            setPixel(window, block->posX * 5 + 3, newY, valueChar1);

            valueChar2 = (block->value / 10) % 10 + '0';
            setPixel(window, block->posX * 5 + 2, newY, valueChar2);

            valueChar3 = (block->value / 100) % 10 + '0';
            setPixel(window, block->posX * 5 + 1, newY, valueChar3);

            valueChar4 = (block->value / 1000) % 10 + '0';
            setPixel(window, block->posX * 5, newY, valueChar4);
            break;
    }

}

void freeBlock(Block block) {
    free(block);
}

void freeBlockArray(BlockArray array) {
    free(array.blocks);
}