#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../machine.h"
#include "./sdt.h"
#include "./dataProcessingImm.h"

#define ONE_BIT_MASK 0x1
#define FIVE_BIT_MASK 0x1F
#define NINE_BIT_MASK 0x1FF
#define TWELVE_BIT_MASK 0xFFF
#define NINETEEN_BIT_MASK 0x7FFFF

int64_t load(int8_t *memory, uint64_t address, int size) {
    int64_t data = 0;
    for (int i = 0; i < size; i++) {
        data += ((memory[address + i]) & 0xff) << (8 * i);
        printf("data: %" PRIx64 "\n", data);
    }
    return extendTo64Bit(data);
}

void store(int8_t *memory, uint64_t address, int64_t data, int size) {
    for (int i = 0; i < size; i++) {
        memory[address + i] = (data >> (i * 8)) & 0xFF; // extract 1 byte
    }
}

void executeSdt(struct Machine *machine, uint32_t instruction) {
    bool isSdt = (instruction >> 31) & ONE_BIT_MASK;
    int rt = instruction & FIVE_BIT_MASK; // target register
    bool is64 = (instruction >> 30) & ONE_BIT_MASK; // sf

    bool isLoad = (instruction >> 22) & ONE_BIT_MASK; // L
    bool isUnsignedOffset = (instruction >> 24) & ONE_BIT_MASK; // U
    int offset = (instruction >> 10) & TWELVE_BIT_MASK;
    int xn = (instruction >> 5) & FIVE_BIT_MASK; // base register

    int simm19 = (instruction >> 5) & NINETEEN_BIT_MASK;

    // size of data we are loading / storing
    int size = is64 ? 8 : 4;

    //printf("xn: %d, rt: %d \n", xn, rt);

    uint64_t address;

    if (isSdt) {
        // 3 cases: register offset, pre/post-index, unsigned offset
        bool isPreIndexed = (instruction >> 11) & ONE_BIT_MASK; // I
        bool isRegisterOffset = (instruction >> 21) & ONE_BIT_MASK;

        if (isUnsignedOffset) {
            // unsigned offset
            // printf("Unsigned offset...\n");

            if (is64) {
                offset *= 8;
            } else {
                offset *= 4;
            }
            address = machine->registers[xn] + offset;

        } else if (isRegisterOffset) {
            // register offset
            int xm = (instruction >> 16) & FIVE_BIT_MASK;
            address = machine->registers[xn] + machine->registers[xm];

        } else if (isPreIndexed) {
            // pre indexed
            // printf("Pre indexing...\n");
            int32_t simm9 = (int32_t)((instruction >> 12) & NINE_BIT_MASK);
            
            // printf("simm9 is %d", simm9);

            // Sign extend
            if (simm9 & 0x100) {
                // set all bits above the lowest 9 to 1
                simm9 |= ~0x1FF;
            }
            address = machine->registers[xn] + simm9;
            machine->registers[xn] = address;

        } else {
            // post indexed
            // printf("Post indexing...\n");
            int32_t simm9 = (int32_t)((instruction >> 12) & NINE_BIT_MASK);
            
            // Sign extend
            if (simm9 & 0x100) {
                // set all bits above the lowest 9 to 1
                simm9 |= ~0x1FF;
            }
            address = machine->registers[xn];
            machine->registers[xn] = machine->registers[xn] + simm9;
        }
        
    } else {
        // load literal
        isLoad = true;
        
        if (simm19 & 0x40000) {
            simm19 |= ~0x7FFFF;
        }
        address = machine->PC + (simm19 << 2); // PC + simm19*4
        
    }

    printf("The address is %" PRIx64 "\n", address);

    if (isLoad) {
        // load
        int64_t data = load(machine->memory, address, size);
        if (is64) {
            machine->registers[rt] = data;
        } else {;
            machine->registers[rt] = extendTo64Bit(data); // Store only lower 32 bits
            printf("data: %" PRIx64 "\n", data);
        }
    } else {
        // store value in register into memory
        int64_t data = machine->registers[rt]; 
        if (!is64) {
            extendTo64Bit(data);
        }
        store(machine->memory, address, data, size);
    }
    
}