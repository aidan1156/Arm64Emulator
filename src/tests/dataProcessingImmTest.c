
#include "../emulator/instructions/dataProcessingImm.c"

#include "../emulator/machine.h"

bool add1ToReg() {
    struct Machine machine;
    initialiseMachine(&machine);
    dataProcessingImmediate(&machine, 0x91000822);

    if (machine.registers[2] != 2) {
        fprintf(stderr, "Failed add1ToReg\n");
        return false;
    }
    return true;
}

bool sub1ToReg() {
    struct Machine machine;
    initialiseMachine(&machine);
    dataProcessingImmediate(&machine, 0xF1000822);

    if (machine.registers[2] != -2) {
        fprintf(stderr, "Failed sub1ToReg with value %lx (%ld)\n", machine.registers[2], machine.registers[2]);
        return false;
    }
    return true;
}

bool overflowFlagSet32BitAdd() {
    struct Machine machine;
    initialiseMachine(&machine);
    dataProcessingImmediate(&machine, 0x313FFC42); // 0 01 100 010  0 1111 1111 1111 00010  00010

    for (int i=0; i<16*8+1;i++) {
        dataProcessingImmediate(&machine, 0x317FFC42); // 0 01 100 010  1 1111 1111 1111 00010  00010
    }
    
    if (machine.PSTATE.V == false) {
        fprintf(stderr, "Failed overflowFlagSet32BitAdd\n");
        return false;
    }
    return true;
}

bool overflowFlagNotSet64BitAdd() {
    struct Machine machine;
    initialiseMachine(&machine);
    dataProcessingImmediate(&machine, 0xB13FFC42);  // 1 01 100 010  0 1111 1111 1111 00010  00010

    for (int i=0; i<16*8+1;i++) {
        dataProcessingImmediate(&machine, 0xB17FFC42); // 1 01 100 010  1 1111 1111 1111 00010  00010
    }
    
    if (machine.PSTATE.V == true) {
        fprintf(stderr, "Failed overflowFlagSet32BitAdd\n");
        return false;
    }
    return true;
}

bool wideMovz() {
    struct Machine machine;
    initialiseMachine(&machine);

    dataProcessingImmediate(&machine, 0xD2800021);

    if (machine.registers[1] != 1) {
        fprintf(stderr, "Failed wideMovz with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }
    return true;
}

bool wideMovn() {
    struct Machine machine;
    initialiseMachine(&machine);

    // not 0x1 64 bit
    dataProcessingImmediate(&machine, 0x92800021); // 1 00 100 101  000000000000000001   00001
    if (machine.registers[1] != 0xfffffffffffffffe) {
        fprintf(stderr, "Failed wideMovn with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }

    // not 0x1 << (16 * 3)
    dataProcessingImmediate(&machine, 0x92E00021); // 1 00 100 101  110000000000000001   00001
    if (machine.registers[1] != 0xfffeffffffffffff) {
        fprintf(stderr, "Failed wideMovn with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }

    // not 0x1 32 bit
    dataProcessingImmediate(&machine, 0x12800021); // 0 00 100 101  000000000000000001   00001
    if (machine.registers[1] != 0x00000000fffffffe) {
        fprintf(stderr, "Failed wideMovn 32 bit with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }


    return true;
}

bool wideMovk() {
    struct Machine machine;
    initialiseMachine(&machine);
    

    // 32 bit movk
    dataProcessingImmediate(&machine, 0x92800001); // set R1 to be 0xfff...
    dataProcessingImmediate(&machine, 0x728175C1); // 0 11 100 101  00 000010111010 1110   00001
    if (machine.registers[1] != 0x00000000ffff0bae) {
        fprintf(stderr, "Failed wideMovk 32 bit with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }


    dataProcessingImmediate(&machine, 0x92800001);
    dataProcessingImmediate(&machine, 0xF28175C1); // 1 11 100 101  00 0000101110101110   00001
    if (machine.registers[1] != 0xffffffffffff0bae) {
        fprintf(stderr, "Failed wideMovk 64 bit with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }


    dataProcessingImmediate(&machine, 0x92800001);
    dataProcessingImmediate(&machine, 0xF2C175C1); // 1 11 100 101  10 0000 1011 1010 1110   00001
    if (machine.registers[1] != 0xffff0baeffffffff) {
        fprintf(stderr, "Failed wideMovk 64 bit with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }


    dataProcessingImmediate(&machine, 0x92800001);
    dataProcessingImmediate(&machine, 0x72A175C1); // 0 11 100 101  01 0000 1011 1010 1110   00001
    if (machine.registers[1] != 0x000000000baeffff) {
        fprintf(stderr, "Failed wideMovk 32 bit with value %lx (%ld)\n", machine.registers[1], machine.registers[1]);
        return false;
    }

    return true;
}


int main(void) {
    int failed = 0;

    failed += !overflowFlagSet32BitAdd();
    failed += !add1ToReg();
    failed += !overflowFlagNotSet64BitAdd();
    failed += !sub1ToReg();
    failed += !wideMovz();
    failed += !wideMovn();
    failed += !wideMovk();

    printf("Executed tests with %d failures\n", failed);
}