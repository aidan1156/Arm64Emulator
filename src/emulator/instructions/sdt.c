#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../machine.h"
#include "./sdt.h"
#include "./dataProcessingImm.h"
#include "../utilities.h"

#define ONE_BIT_MASK 0x1
#define FIVE_BIT_MASK 0x1F
#define EIGHT_BIT_MASK 0xFF
#define NINE_BIT_MASK 0x1FF
#define TWELVE_BIT_MASK 0xFFF
#define NINETEEN_BIT_MASK 0x7FFFF
#define SIGN_BIT_MASK_9 0x100
#define SIGN_BIT_MASK_19 0x40000
#define SIGN_BIT_MASK_32 0x80000000
#define SIGN_EXTENDER_32 0xFFFFFFFF00000000

int64_t load(int8_t *memory, uint64_t address, int size) {
    int64_t data = 0;

    for (int i = 0; i < size; i++) {
        // don't prematurely sign extend)
        // 8 bits mask out any potential sign extension
        data |= ((uint64_t)(memory[address + i] & EIGHT_BIT_MASK)) << (8 * i);
    }

    // Explicitly handle 32-bit sign extension
    if (size == 4) {
        if (data & SIGN_BIT_MASK_32) { // If the sign bit (bit 31) is set
            data |= SIGN_EXTENDER_32; // Sign extend to 64 bits
        }
    }

    return data;
}

void store(int8_t *memory, uint64_t address, int64_t data, int size) {
    for (int i = 0; i < size; i++) {
        memory[address + i] = (data >> (i * 8)) & EIGHT_BIT_MASK; // extract 1 byte
    }
}

void executeSdt(struct Machine *machine, uint32_t instruction) {
    bool isSdt = (instruction >> 31) & ONE_BIT_MASK;
    int rt = instruction & FIVE_BIT_MASK; // target register
    bool is64 = (instruction >> 30) & ONE_BIT_MASK; // sf
    bool isLoad = (instruction >> 22) & ONE_BIT_MASK; // L
    bool isUnsignedOffset = (instruction >> 24) & ONE_BIT_MASK; // U
    int imm12 = (instruction >> 10) & TWELVE_BIT_MASK;
    int xn = (instruction >> 5) & FIVE_BIT_MASK; // base register
    int simm19 = (instruction >> 5) & NINETEEN_BIT_MASK;
    int size = is64 ? 8 : 4; // size of data we are loading / storing
    // for pre and post indexing
    int32_t simm9 = (int32_t)((instruction >> 12) & NINE_BIT_MASK);
    // Sign extend
            if (simm9 & SIGN_BIT_MASK_9) {
                // set all bits above the lowest 9 to 1
                simm9 |= ~NINE_BIT_MASK;
            }

    uint64_t address;

    if (isSdt) {
        // 3 cases: register offset, pre/post-index, unsigned offset
        bool isPreIndexed = (instruction >> 11) & ONE_BIT_MASK; // I
        bool isRegisterOffset = (instruction >> 21) & ONE_BIT_MASK;

        if (isUnsignedOffset) {
            // unsigned offset
            if (is64) {
                imm12 <<= 3;
            } else {
                imm12 <<= 2;
            }

            address = machine->registers[xn] + imm12;

        } else if (isRegisterOffset) {
            // register offset
            int xm = (instruction >> 16) & FIVE_BIT_MASK;
            address = machine->registers[xn] + machine->registers[xm];

        } else if (isPreIndexed) {
            // pre indexed
            address = machine->registers[xn] + simm9;
            machine->registers[xn] = address;

        } else {
            // post indexed
            address = machine->registers[xn];
            machine->registers[xn] = machine->registers[xn] + simm9;
        }
        
    } else {
        // load literal
        isLoad = true;
        if (simm19 & SIGN_BIT_MASK_19) {
            simm19 |= ~NINETEEN_BIT_MASK;
        }
        address = machine->PC + (simm19 << 2); // PC + simm19*4
        
    }

    printf("The address is %" PRIx64 "\n", address);

    if (isLoad) {
        // load
        int64_t data = load(machine->memory, address, size);
            machine->registers[rt] = data;
    } else {
        // store value in register into memory
        int64_t data = machine->registers[rt];
        store(machine->memory, address, data, size);
    }
    
}