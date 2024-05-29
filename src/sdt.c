#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "machine.h"
#include "sdt.h"

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

    // size of data we are loading / storing
    int size = is_64 ? 8 : 4;

    //printf("xn: %d, rt: %d \n", xn, rt);

    uint64_t address;

    if (is_sdt) {
        // 3 cases: register offset, pre/post-index, unsigned offset
        bool is_pre_indexed = (instruction >> 11) & ONE_BIT_MASK; // I
        bool is_register_offset = (instruction >> 21) & ONE_BIT_MASK;

        if (is_unsigned_offset) {
            // unsigned offset
            // printf("Unsigned offset...\n");

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
            // printf("Pre indexing...\n");
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
        is_load = true;
        
        if (simm19 & 0x40000) {
            simm19 |= ~0x7FFFF;
        }
        address = machine->PC + (simm19 << 2); // PC + simm19*4
        
    }

    printf("The address is %" PRIx64 "\n", address);

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
    machine.registers[4] = 0x1000;
    machine.PC = 0x2000;

    // Print initial state
    printf("Initial state:\n");
    printMachine(&machine, NULL);

    // check load and store for 64 and 32 bit
    // check write back

    // PRE-INDEXED
    // Store instruction: STR X0, [X1, simm9 = 0] 
    uint32_t instr = 0xF8000C20; // 1 1 1 110 0  0  0  0   0 000 000 000 11  00001 00000
    execute_sdt(instr, &machine);
    printf("After STR X0, [X1, simm9 = 0]:\n");
    printMachine(&machine, NULL);

    // Store instruction: STR X0, [X1, simm9 = 8],
    instr = 0xF8008C20; // 1 1 1 110 0  0  0  0   0 000 001 000 11  00001 00000
    execute_sdt(instr, &machine);
    printf("After STR X0, [X1, simm9 = 8]]:\n");
    printMachine(&machine, NULL);

    // Store instruction: STR W0, [X1, simm9 = 16]
    instr = 0xB8010C20; //1 0 1 110 0 0 0 0 0 000 010 000 11 00001 00000
    execute_sdt(instr, &machine);
    printf("After STR W0, [X1, simm9 = 0]!:\n");
    printMachine(&machine, NULL);

    // Load instruction: LDR W3, [X1, simm9 = 0] load value at address in X1
    instr = 0xB8400C23; // 1 0 1 110 0 0 0 1 0 000 000 000 11 00001 00011
    execute_sdt(instr, &machine);
    printf("After LDR W3, [X1, simm9 = 0]!:\n");
    printMachine(&machine, NULL);

    // POST INDEX
    // Load instruction: LDR X5, [X4, simm9 = 4] load value at address in X4 into X5
    instr = 0xF8404485; // 1 1 1 110 0 0 0 1 0 000 000 100 01 00100 00101
    execute_sdt(instr, &machine);
    printf("After LDR X5, [X4, simm9 = 4]!:\n");
    printMachine(&machine, NULL);

    // UNSIGNED OFFSET
    // Load instruction: LDR W6, [X4, imm12 = 2 (offset = 8)] load 32 bit value at address in X4 into X5
    instr = 0xB9400886; // 1 0 1 110 0 1 0 1 0000 0000 0010 00100 00110
    execute_sdt(instr, &machine);
    printf("After LDR X6, [X4, imm12 = 1]!:\n");
    printMachine(&machine, NULL);


    // REGISTER OFFSET
    // Load instruction: store value at X2 into address X1 + X4
    instr = 0xF8246822; // 1 1 1 110 0 0 0 0 1 00100 011010 00001 00010
    execute_sdt(instr, &machine);
    printf("store value at X2 into address X1 + X4:\n");
    printMachine(&machine, NULL);


    // LOAD FROM LITERAL
    // Load instruction: load value at 0x201c into X7
    // PC = 2000, simm19 = 7
    instr = 0x180000E7; // 0 0 011 000 0000 00000 00000 00111 00111
    execute_sdt(instr, &machine);
    printf("load value at 0x201c into X7:\n");
    printMachine(&machine, NULL);
    
    return EXIT_SUCCESS;
}