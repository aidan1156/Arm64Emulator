

#include "./Block.h"

// deleting block from BlockArray at a given index
void deleteBlock(BlockArray blockArray, int index) {
    assert (index < 0 || index >= blockArray.size); // index in the right range

    // Shift elements to the left to fill the gap
    for (int i = index; i < blockArray.size - 1; i++) {
        blockArray.blocks[i] = blockArray.blocks[i + 1];
    }

    // Decrement the size
    blockArray.size--;

}


// Function to add a new block
void addBlock(BlockArray blockArray, Block block) {
    // Increment the size
    blockArray.size++;

    assert(blockArray.size <= 16); // should not exceed max

    blockArray.blocks[blockArray.size - 1].posX = block.posX;
    blockArray.blocks[blockArray.size - 1].posY = block.posY;
    blockArray.blocks[blockArray.size - 1].value = block.value;
}


// find index of given block position
// returns index if found
// returns -1 if not found
int findIndex(int x, int y, BlockArray blockArray) {
    for (int i = 0; i < blockArray.size; i++) {
        if ((blockArray.blocks[i].posX == x) & (blockArray.blocks[i].posY == y)) {
            return i;
        }
    }
    return -1;
}


Block createBlock(BlockArray existingBlocks) {
    assert(existingBlocks.size < 16); 
    // createBlock should not be called when there are 16 existing blocks already

    Block block;
    block.value = 2;
    block.posX = rand() % 4;
    block.posY = rand() % 4;

    if (findIndex(block.posX, block.posY, existingBlocks) == -1) {
        block.posX = rand() % 4;
        block.posY = rand() % 4;
    }

    addBlock(existingBlocks, block); // add to blocks list

    return block;
}

void drawBlock(Block block, Window window) {
    int numDigits = 0;
    if ((block.value/1000) >= 1) { 
        // block can never have value 1000 so using 1000 instead of 999 for readability
        numDigits = 4;
    } else if ((block.value/100) >= 1) { // same goes for 100
        numDigits = 3;
    } else if ((block.value/10) >= 1) { // and so on
        numDigits = 2;
    } else {
        numDigits = 1;
    }

    char valueChar1;
    char valueChar2;
    char valueChar3;
    char valueChar4;
    int newY = block.posY * 5 + 1;
    switch (numDigits) {
        case 1:
            valueChar1 = block.value + '0';
            setPixel(window, block.posX * 5 + 1, newY, valueChar1);
            break;
            
        case 2:
            valueChar1 = (block.value) % 10 + '0';
            setPixel(window, block.posX * 5 + 2, newY, valueChar1);

            valueChar2 = (block.value / 10) % 10 + '0';
            setPixel(window, block.posX * 5 + 1, newY, valueChar1);
            break;

        case 3:
            valueChar1 = (block.value) % 10 + '0';
            setPixel(window, block.posX * 5 + 2, newY, valueChar1);

            valueChar2 = (block.value / 10) % 10 + '0';
            setPixel(window, block.posX * 5 + 1, newY, valueChar2);

            valueChar3 = (block.value / 100) % 10 + '0';
            setPixel(window, block.posX * 5, newY, valueChar3);
            break;

        case 4:
            valueChar1 = (block.value) % 10 + '0';
            setPixel(window, block.posX * 5 + 3, newY, valueChar1);

            valueChar2 = (block.value / 10) % 10 + '0';
            setPixel(window, block.posX * 5 + 2, newY, valueChar2);

            valueChar3 = (block.value / 100) % 10 + '0';
            setPixel(window, block.posX * 5 + 1, newY, valueChar3);

            valueChar4 = (block.value / 1000) % 10 + '0';
            setPixel(window, block.posX * 5, newY, valueChar4);
            break;
    }
}

void drawBlockArray(Window window, BlockArray array) {
    for (int i = 0; i < array.size; i++) {
        drawBlock(array.blocks[i], window);
    }
}

void mergeBlocks(Block block1, Block block2, BlockArray array, int newX, int newY) {
    // deleting the old blocks from array
    deleteBlock(array, findIndex(block1.posX, block1.posY, array)); 
    deleteBlock(array, findIndex(block2.posX, block2.posY, array));

    // find new Value
    int newVal = block1.value + block2.value;

    // making new block
    Block newBlock;
    newBlock.posX = newX;
    newBlock.posY = newY;
    newBlock.value = newVal;

    // adding new block
    addBlock(array, newBlock);
}


void updateBlocks(BlockArray array, char keyPress) {
    if (keyPress == 'w') { // up
        // any block with posY != 0 moves to 0
        // either merge or position moves up max

        for (int i = 0; i < 4; i++) {
            // check each column at a time
            
        }

    } else if (keyPress == 's') { // down

    } else if (keyPress == 'a') { // left
         
    } else if (keyPress == 'd') { // right

    }
}



// void freeBlock(Block block) {
//     free(block);
// }

// void freeBlockArray(BlockArray array) {
//     free(array.blocks);
// }