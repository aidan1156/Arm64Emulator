#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "machine.h"

#define ONE_BIT_MASK 0x1
#define FIVE_BIT_MASK 0x1F
#define NINE_BIT_MASK 0x1FF
#define TWELVE_BIT_MASK 0xFFF
#define NINETEEN_BIT_MASK 0x7FFFF

uint64_t load(uint8_t *memory, uint64_t address, int size) {
    uint64_t data = 0;
    for (int i = 0; i < size; i++) {
        data |= ((uint64_t)memory[address + i]) << (i * 8);
    }
    return data;
}

void store(uint8_t *memory, uint64_t address, uint64_t data, int size) {
    for (int i = 0; i < size; i++) {
        memory[address + i] = (data >> (i * 8)) & 0xFF; // extract 1 byte
    }
}

void execute_sdt(uint32_t instruction, struct Machine *machine) {
    bool is_sdt = (instruction >> 31) & ONE_BIT_MASK;
    int rt = instruction & FIVE_BIT_MASK; // target register
    bool is_64 = (instruction >> 30) & ONE_BIT_MASK; // sf

    bool is_load = (instruction >> 22) & ONE_BIT_MASK; // L
    bool is_unsigned_offset = (instruction >> 24) & ONE_BIT_MASK; // U
    int offset = (instruction >> 10) & TWELVE_BIT_MASK;
    int xn = (instruction >> 5) & FIVE_BIT_MASK; // base register

    int simm19 = (instruction >> 5) & NINETEEN_BIT_MASK;

    printf("xn: %d, rt: %d \n", xn, rt);

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
            
            printf("simm9 is %d", simm9);

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

    printf("The address is %" PRIu64 "\n", address);

    // size of data we are loading / storing
    int size = is_64 ? 8 : 4;
    if (is_load) {
        // load
        uint64_t data = load(machine->memory, address, size);
        if (is_64) {
            machine->registers[rt] = data;
        } else {
            machine->registers[rt] = (uint32_t)data; // Store only lower 32 bits
        }
    } else {
        // store value in register into memory
        uint64_t data = machine->registers[rt]; 
        if (!is_64) {
            // mask to 32 bits
            data &= 0xFFFFFFFF;
        }
        store(machine->memory, address, data, size);
    }
    
}

int main(void) {
    struct Machine machine;
    initialiseMachine(&machine);
    
    // Initialize some test values
    machine.registers[0] = 0x123456789ABCDEF0; // X0
    machine.registers[1] = 0x1000; // X1 (base address)
    machine.registers[2] = 0x4; // X2 (offset)
    machine.registers[3] = 0xFFFFFFFFFFFFFFFF; // X3 (to be loaded with 32-bit value)
    machine.PC = 0x2000;

    // Print initial state
    printf("Initial state:\n");
    printMachine(&machine, NULL);

    // Store instruction: STR X0, [X1, simm9 = 0] 
    uint32_t storeInstr = 0xF8000C20; // 1 1 1 110 0  0  0  0   0 000 000 000 11  00001 00000
    execute_sdt(storeInstr, &machine);
    printf("After STR X0, [X1, #8]:\n");
    printMachine(&machine, NULL);

// Store instruction: STR X0, [X1, simm9 = 8],
    storeInstr = 0xF8008C20; // 1 1 1 110 0  0  0  0   0 000 001 000 11  00001 00000
    execute_sdt(storeInstr, &machine);
    printf("After STR X0, [X1, #8]:\n");
    printMachine(&machine, NULL);

    // // Load instruction: LDR X3, [X1, #8]
    // uint32_t loadInstr = 0xF9400843; // 1111 1001 0100 0000 0000 1000 0100 0011
    // execute_sdt(loadInstr, &machine);
    // printf("After LDR X3, [X1, #8]:\n");
    // printMachine(&machine, NULL);

    // // Store instruction: STR W0, [X1, #4]
    // storeInstr = 0xB9000440; // 1011 1001 0000 0000 0000 0100 0100 0000
    // execute_sdt(storeInstr, &machine);
    // printf("After STR W0, [X1, #4]:\n");
    // printMachine(&machine, NULL);

    // // Load instruction: LDR W3, [X1, #4]
    // loadInstr = 0xB9400443; // 1011 1001 0100 0000 0000 0100 0100 0011
    // execute_sdt(loadInstr, &machine);
    // printf("After LDR W3, [X1, #4]:\n");
    // printMachine(&machine, NULL);

    return EXIT_SUCCESS;
}