
#include "../emulator/machine.h" 
#include "../emulator/instructions/sdt.h"

void assertMemoryValue(int8_t *memory, uint64_t address, int64_t expected_value, int size, int *fail_count) {
    int64_t actual_value = load(memory, address, size);
    if (actual_value != expected_value) {
        printf("Assertion failed: Memory at address 0x%lx expected 0x%lx, but got 0x%lx\n", address, expected_value, actual_value);
        (*fail_count)++;
    }
}

void assertRegisterValue(int64_t *registers, int reg_num, int64_t expected_value, int *fail_count) {
    int64_t actual_value = registers[reg_num];
    if (actual_value != expected_value) {
        printf("Assertion failed: Register X%d expected 0x%lx, but got 0x%lx\n", reg_num, expected_value, actual_value);
        (*fail_count)++;
    }
}

int main(void) {
    struct Machine machine;
    initialiseMachine(&machine);
    int failed = 0;
    int count = 0;
    
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

    // PRE-INDEXED
    // STR X0, [X1, simm9 = 0] 
    uint32_t instr = 0xF8000C20; // 1 1 1 110 0  0  0  0   0 000 000 000 11  00001 00000
    execute_sdt(instr, &machine);
    printf("After STR X0, [X1, simm9 = 0]:\n");
    printMachine(&machine, NULL);
    assertMemoryValue(machine.memory, 0x1000, 0x123456789ABCDEF0, 8, &failed);
    count++;
    

    // STR X0, [X1, simm9 = 8],
    instr = 0xF8008C20; // 1 1 1 110 0  0  0  0   0 000 001 000 11  00001 00000
    execute_sdt(instr, &machine);
    printf("After STR X0, [X1, simm9 = 8]]:\n");
    printMachine(&machine, NULL);
    assertMemoryValue(machine.memory, 0x1008, 0x123456789ABCDEF0, 8, &failed);
    count++;


    // STR W0, [X1, simm9 = 16]
    instr = 0xB8010C20; //1 0 1 110 0 0 0 0 0 000 010 000 11 00001 00000
    execute_sdt(instr, &machine);
    printf("After STR W0, [X1, simm9 = 0]!:\n");
    printMachine(&machine, NULL);
    assertMemoryValue(machine.memory, 0x1018, 0x9ABCDEF0, 4, &failed);
    count++;

    // LDR W3, [X1, simm9 = 0] load value at address in X1
    instr = 0xB8400C23; // 1 0 1 110 0 0 0 1 0 000 000 000 11 00001 00011
    execute_sdt(instr, &machine);
    printf("After LDR W3, [X1, simm9 = 0]!:\n");
    printMachine(&machine, NULL);
    assertRegisterValue(machine.registers, 3, 0x9ABCDEF0, &failed);
    count++;

    // POST INDEX
    // LDR X5, [X4, simm9 = 4] load value at address in X4 into X5
    instr = 0xF8404485; // 1 1 1 110 0 0 0 1 0 000 000 100 01 00100 00101
    execute_sdt(instr, &machine);
    printf("After LDR X5, [X4, simm9 = 4]!:\n");
    printMachine(&machine, NULL);
    assertRegisterValue(machine.registers, 5, 0x9ABCDEF012345678, &failed);
    count++;

    // UNSIGNED OFFSET
    // LDR W6, [X4, imm12 = 2 (offset = 8)] load 32 bit value at address in X4 into X5
    instr = 0xB9400886; // 1 0 1 110 0 1 0 1 0000 0000 0010 00100 00110
    execute_sdt(instr, &machine);
    printf("After LDR W6, [X4, imm12 = 1]!:\n");
    printMachine(&machine, NULL);
    assertRegisterValue(machine.registers, 6, 0x12345678, &failed);
    count++;


    // REGISTER OFFSET
    // store value at X2 into address X1 + X4
    instr = 0xF8246822; // 1 1 1 110 0 0 0 0 1 00100 011010 00001 00010
    execute_sdt(instr, &machine);
    printf("store value at X2 into address X1 + X4:\n");
    printMachine(&machine, NULL);
    assertMemoryValue(machine.memory, 0x201c, 0x4, 8, &failed);
    count++;


    // LOAD FROM LITERAL
    // load value at 0x201c into X7
    // PC = 2000, simm19 = 7
    instr = 0x180000E7; // 0 0 011 000 0000 00000 00000 00111 00111
    execute_sdt(instr, &machine);
    printf("load value at 0x201c into X7:\n");
    printMachine(&machine, NULL);
    assertRegisterValue(machine.registers, 7, 0x4, &failed);
    count++;

    // pre-index negative immediate 
    // simm9 = -12 load from address in X1 + simm9 and store in X8
    instr = 0xB85F4C28; // 1 0 1 110 0 0 0 1 0 111 110 100 11 00001 01000
    execute_sdt(instr, &machine);
    printf("load from (address in X1) - 12 and store in X8:\n");
    printMachine(&machine, NULL);
    assertRegisterValue(machine.registers, 8, 0x12345678, &failed);
    count++;


    printf("Executed tests with %d failures out of %d\n", failed, count);

    return EXIT_SUCCESS;
}