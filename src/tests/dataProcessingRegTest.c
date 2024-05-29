
#include "../emulator/machine.h"
#include "../emulator/instructions/dataProcessingReg.h"

bool noShiftAndNoFlag() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[1] = 0x00000001;
    machine.registers[2] = 0x00000011;

    dataProcessingRegister(&machine, 0x8A010043);
    // 64 bits
    // no shift : operand = 0
    // rm = 1
    // rn = 2
    // rd = 3
    // store rd := rm & rn

    if (machine.registers[3] != 0x00000001) {
        fprintf(stderr, "Failed noShiftAndNoFlag\n");
        return false;
    }
    return true;
}

bool noShiftInclusiveOr() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x00000001;
    machine.registers[1] = 0x00000010;

    dataProcessingRegister(&machine, 0xAA000022);
    // 64 bits
    // no shift : operand = 0
    // rm = 0
    // rn = 1
    // rd = 2
    // store rd := rm | rn

    if (machine.registers[2] != 0x000000011) {
        fprintf(stderr, "Failed noShiftInclusiveOr\n");
        return false;
    }
    return true;
}

bool noShiftExclusiveOr() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x00000001;
    machine.registers[1] = 0x00000010;

    dataProcessingRegister(&machine, 0xCA000022);
    // 64 bits
    // no shift : operand = 0
    // rm = 0
    // rn = 1
    // rd = 2
    // store rd := rm ^ rn

    if (machine.registers[2] != 0x000000011) {
        fprintf(stderr, "Failed noShiftExclusiveOr\n");
        return false;
    }
    return true;
}

bool noShiftAndFlag1() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x00000001;
    machine.registers[1] = 0x00000011;

    dataProcessingRegister(&machine, 0xEA000022);
    // 64 bits
    // no shift : operand = 0
    // rm = 0
    // rn = 1
    // rd = 2
    // store rd := rm & rn
    // N = 0
    // Z = 1..? 0..? not sure
    // C = 0
    // V = 0

    bool c = machine.PSTATE.C != 0;
    bool n = machine.PSTATE.N != 0;
    bool v = machine.PSTATE.V != 0;
    bool z = machine.PSTATE.Z != 0;

    if ((machine.registers[2] != 0x000000001) | c | v | n | z) {
        fprintf(stderr, "Failed noShiftAndFlag1\n");
        return false;
    }
    return true;
}

bool noShiftAndFlag2() {   // failed
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x80000001;
    machine.registers[1] = 0x80000011;

    dataProcessingRegister(&machine, 0xEA000022);
    // 64 bits
    // no shift : operand = 0
    // rm = 0
    // rn = 1
    // rd = 2
    // store rd := rm & rn
    // N = 1
    // Z = 0
    // C = 0
    // V = 0

    bool c = machine.PSTATE.C != 0;
    bool n = machine.PSTATE.N != 1;
    bool v = machine.PSTATE.V != 0;
    bool z = machine.PSTATE.Z != 0;
    printf("========== %ld\n", machine.registers[2]);
    printf("========== %d\n", 0x80000001);


    if ((machine.registers[2] != 0x80000001) | c | v | n | z) {
        fprintf(stderr, "Failed noShiftAndFlag2\n");
        return false;
    }
    return true;
}

bool noShiftAndFlag3() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x00000001;
    machine.registers[1] = 0x00000010;

    dataProcessingRegister(&machine, 0xEA000022);
    // 64 bits
    // no shift : operand = 0
    // rm = 0
    // rn = 1
    // rd = 2
    // store rd := rm & rn
    // N = 0
    // Z = 1
    // C = 0
    // V = 0

    bool c = machine.PSTATE.C != 0;
    bool n = machine.PSTATE.N != 0;
    bool v = machine.PSTATE.V != 0;
    bool z = machine.PSTATE.Z != 1;

    if ((machine.registers[2] != 0x000000000) | c | v | n | z) {
        fprintf(stderr, "Failed noShiftAndFlag3\n");
        return false;
    }
    return true;
}

// logical shift left with OR
bool lsl() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x00000001;

    dataProcessingRegister(&machine, 0xAA001822);
    // 64 bits
    // logical shift left : operand = 5
    // rm = 0
    // rn = 1
    // rd = 2
    // op2 = 0b1000
    // store rd := rm | rn

    if ((machine.registers[2] != 0x000000040)) {
        fprintf(stderr, "Failed lsl\n");
        return false;
    }
    return true;
}

// logical shift right with OR
bool lsr() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x00000008;

    dataProcessingRegister(&machine, 0xAA400C22);
    // 64 bits
    // logical shift left : operand = 3
    // rm = 0
    // rn = 1
    // rd = 2
    // op2 = 0b0001
    // store rd := rm | rn

    if ((machine.registers[2] != 0x000000001)) {
        fprintf(stderr, "Failed lsr\n");
        return false;
    }
    return true;
}

// arithmetic shift right with OR
bool asr() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 80000000;

    dataProcessingRegister(&machine, 0xAA800C22);
    // 64 bits
    // logical shift left : operand = 3
    // rm = 0
    // rn = 1
    // rd = 2
    // op2 = 0b0001
    // store rd := rm | rn

    printf(" ========== %ld \n", machine.registers[2]);
    // registers is uint64_t
    // needs to be int64_t
    
    if ((machine.registers[2] != 0xC0000000)) {
        fprintf(stderr, "Failed asr\n");
        return false;
    }
    return true;
}

// rotation right with OR
bool ror() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x00000005;

    dataProcessingRegister(&machine, 0x2AC00822);
    // 32 bits
    // logical shift left : operand = 3
    // rm = 0
    // rn = 1
    // rd = 2
    // op2 = rotate right 3 times for 0x0000 0005
    // store rd := rm | rn
    
    if ((machine.registers[2] != 0x40000001)) {
        fprintf(stderr, "Failed ror\n");
        return false;
    }
    return true;
}

bool add() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[0] = 0x8;
    machine.registers[1] = 0x0A;

    dataProcessingRegister(&machine, 0x8B000022);
    
    if ((machine.registers[2] != 0x12)) {
        fprintf(stderr, "Failed add\n");
        return false;
    }
    return true;
}


int main(void) {
    int failed = 0;

    failed += !noShiftAndNoFlag();
    failed += !noShiftInclusiveOr();
    failed += !noShiftExclusiveOr();
    failed += !noShiftAndFlag1();
    failed += !noShiftAndFlag2();
    failed += !noShiftAndFlag3();
    failed += !lsl();
    failed += !lsr();
    failed += !asr();
    failed += !ror();
    failed += !add();

    printf("%d failures\n", failed);
}