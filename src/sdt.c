#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./machine.c"

#define ONE_BIT_MASK 0x1
#define FIVE_BIT_MASK 0x1F
#define NINE_BIT_MASK 0x1FF
#define TWELVE_BIT_MASK 0xFFF
#define NINETEEN_BIT_MASK 0x7FFFF


void execute_sdt(uint32_t instruction, struct Machine *machine) {
    bool is_sdt = (instruction >> 31) & ONE_BIT_MASK;
    int target_register = instruction & FIVE_BIT_MASK; // rt
    bool is_64 = (instruction >> 30) & ONE_BIT_MASK; // sf

    bool is_load = (instruction >> 22) & ONE_BIT_MASK; // L
    bool is_unsigned_offset = (instruction >> 24) & ONE_BIT_MASK; // U
    int offset = (instruction >> 10) & TWELVE_BIT_MASK;
    int xn = (instruction >> 5) & FIVE_BIT_MASK; // base register

    int simm19 = (instruction >> 5) & NINETEEN_BIT_MASK;

    uint64_t address;

    if (is_sdt) {
        // 3 cases: register offset, pre/post-index, unsigned offset
        bool is_pre_indexed = (instruction >> 11) & ONE_BIT_MASK; // I
        bool is_register_offset = (instruction >> 21) & ONE_BIT_MASK;

        if (is_unsigned_offset) {
            // unsigned offset
            if (is_64) {
                offset *= 8;
            } else {
                offset *= 4;
            }
            address = machine->registers[xn] + offset;

        } else if (is_register_offset) {
            // register offset
            int xm = (instruction >> 16) & FIVE_BIT_MASK;
            address = machine->registers[xn] + machine->registers[xm];

        } else if (is_pre_indexed) {
            // pre indexed
            int32_t simm9 = (int32_t)((instruction >> 12) & NINE_BIT_MASK);
            
            // Sign extend
            if (simm9 & 0x100) {
                // set all bits above the lowest 9 to 1
                simm9 |= ~0x1FF;
            }
            address = machine->registers[xn] + simm9;
            machine->registers[xn] = address;

        } else {
            // post indexed
            int32_t simm9 = (int32_t)((instruction >> 12) & NINE_BIT_MASK);
            
            // Sign extend
            if (simm9 & 0x100) {
                // set all bits above the lowest 9 to 1
                simm9 |= ~0x1FF;
            }
            address = machine->registers[xn];
            machine->registers[xn] += simm9;
        }
        
    } else {
        // load literal
        if (simm19 & 0x40000) {
            simm19 |= ~0x7FFFF;
        }
        address = machine->PC + (simm19 << 2); //PC+simm19*4
        
    }
    
}