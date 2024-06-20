

#include "./Block.h"


// typedef struct Block {
//     int posX;
//     int posY;
//     int value;
// } Block;


// typedef struct BlockArray {
//     Block* blocks; // flattened out 1D list 
//     int size;
// } BlockArray;


// getting index of block in array depending on the position of block
static int getBlockIndex(int x, int y){
    return y * 4 + x;
}

void initBlockArray(BlockArray array) {
    for (int i = 0; i < 16; i++) {
        array.blocks[i].value = 0;
        array.blocks[i].posX = -1;
        array.blocks[i].posY = -1;
    }
}


// deleting block from BlockArray at a given index
void deleteBlock(BlockArray blockArray, int index) {
    assert (index < 0 || index >= blockArray.size); // index in the right range

    // // Shift elements to the left to fill the gap
    // for (int i = index; i < blockArray.size - 1; i++) {
    //     blockArray.blocks[i] = blockArray.blocks[i + 1];
    // }

    blockArray.blocks[index].value = 0; // reinitialising the value

    // Decrement the size
    blockArray.size--;

}


// Function to add a new block
void addBlock(BlockArray blockArray, Block block) {
    // Increment the size
    blockArray.size++;

    assert(blockArray.size <= 16); // should not exceed max

    int index = getBlockIndex(block.posX, block.posY);

    blockArray.blocks[index].posX = block.posX;
    blockArray.blocks[index].posY = block.posY;
    blockArray.blocks[index].value = block.value;
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
    for (int i = 0; i < 16; i++) {
        if (array.blocks[i].value != 0) { // if there is a value there
            drawBlock(array.blocks[i], window);
        }
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


// bool checkBlocks(Block block1, Block block2, BlockArray array) {
//     if 
// }

bool updateBlocks(BlockArray* array, char keyPress) {
    bool moved = false;

    if (keyPress == 'w') { // Up
        for (int x = 0; x < 4; x++) {
            for (int y = 1; y < 4; y++) {
                int index = getBlockIndex(x, y);

                // only working with blocks that actually exist
                if (array->blocks[index].value != 0) {
                    
                    // setting targetY: first y value above
                    int targetY = y;
                    while (targetY > 0 && array->blocks[getBlockIndex(x, targetY - 1)].value == 0) {
                        targetY--;
                    }
                    int targetIndex = getBlockIndex(x, targetY - 1);

                    if (targetY > 0 && array->blocks[targetIndex].value == array->blocks[index].value) {
                        // if targetY has same value then merge
                        array->blocks[targetIndex].value *= 2;
                        array->blocks[index].value = 0;
                    
                        array->blocks[targetIndex].posX = x;
                        array->blocks[targetIndex].posY = targetY - 1;

                        moved = true;
                    } else {
                        // different values:
                        targetIndex = getBlockIndex(x, targetY);
                        if (targetIndex != index) {
                            // if shifting up is needed
                            array->blocks[targetIndex] = array->blocks[index];
                            array->blocks[index].value = 0;
                            
                            array->blocks[targetIndex].posX = x;
                            array->blocks[targetIndex].posY = targetY;
                            moved = true;
                        }
                    }
                }
            }
        }

    } else if (keyPress == 's') { // Down
        for (int x = 0; x < 4; x++) {
            for (int y = 2; y >= 0; y--) {
                int index = getBlockIndex(x, y);

                if (array->blocks[index].value != 0) {
                    int targetY = y;
                    while (targetY < 3 && array->blocks[getBlockIndex(x, targetY + 1)].value == 0) {
                        targetY++;
                    }
                    
                    int targetIndex = getBlockIndex(x, targetY + 1);
                    if (targetY < 3 && array->blocks[targetIndex].value == array->blocks[index].value) {
                        array->blocks[targetIndex].value *= 2;
                        array->blocks[index].value = 0;

                        array->blocks[targetIndex].posX = x;
                        array->blocks[targetIndex].posY = targetY + 1;
                        moved = true;
                    } else {
                        targetIndex = getBlockIndex(x, targetY);
                        if (targetIndex != index) {
                            array->blocks[targetIndex] = array->blocks[index];
                            array->blocks[index].value = 0;

                            array->blocks[targetIndex].posX = x;
                            array->blocks[targetIndex].posY = targetY;
                            moved = true;
                        }
                    }
                }
            }
        }
    } else if (keyPress == 'a') { // Left
        for (int y = 0; y < 4; y++) {
            for (int x = 1; x < 4; x++) {
                int index = getBlockIndex(x, y);
                if (array->blocks[index].value != 0) {
                    int targetX = x;
                    while (targetX > 0 && array->blocks[getBlockIndex(targetX - 1, y)].value == 0) {
                        targetX--;
                    }
                    int targetIndex = getBlockIndex(targetX - 1, y);
                    if (targetX > 0 && array->blocks[targetIndex].value == array->blocks[index].value) {
                        array->blocks[targetIndex].value *= 2;
                        array->blocks[index].value = 0;

                        array->blocks[targetIndex].posX = targetX - 1;
                        array->blocks[targetIndex].posY = y;
                        moved = true;
                    } else {
                        targetIndex = getBlockIndex(targetX, y);
                        if (targetIndex != index) {
                            array->blocks[targetIndex] = array->blocks[index];
                            array->blocks[index].value = 0;

                            array->blocks[targetIndex].posX = targetX;
                            array->blocks[targetIndex].posY = y;
                            moved = true;
                        }
                    }
                }
            }
        }
    } else if (keyPress == 'd') { // Right
        for (int y = 0; y < 4; y++) {
            for (int x = 2; x >= 0; x--) {
                int index = getBlockIndex(x, y);
                if (array->blocks[index].value != 0) {
                    int targetX = x;
                    while (targetX < 3 && array->blocks[getBlockIndex(targetX + 1, y)].value == 0) {
                        targetX++;
                    }
                    int targetIndex = getBlockIndex(targetX + 1, y);
                    if (targetX < 3 && array->blocks[targetIndex].value == array->blocks[index].value) {
                        array->blocks[targetIndex].value *= 2;
                        array->blocks[index].value = 0;

                        array->blocks[targetIndex].posX = targetX + 1;
                        array->blocks[targetIndex].posY = y;
                        moved = true;
                    } else {
                        targetIndex = getBlockIndex(targetX, y);
                        if (targetIndex != index) {
                            array->blocks[targetIndex] = array->blocks[index];
                            array->blocks[index].value = 0;

                            array->blocks[targetIndex].posX = targetX;
                            array->blocks[targetIndex].posY = y;
                            moved = true;
                        }
                    }
                }
            }
        }
    }

    return moved;
}




// void freeBlock(Block block) {
//     free(block);
// }

// void freeBlockArray(BlockArray array) {
//     free(array.blocks);
// }