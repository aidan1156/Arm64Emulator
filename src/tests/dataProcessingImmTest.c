
#include "../dataProcessingImm.c"

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
    // 0 01 100 010  0 1111 1111 1111 00010  00010
    dataProcessingImmediate(&machine, 0x313FFC42);

    // 0 01 100 010  1 1111 1111 1111 00010  00010
    for (int i=0; i<16*8+1;i++) {
        dataProcessingImmediate(&machine, 0x317FFC42);
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
    // 1 01 100 010  0 1111 1111 1111 00010  00010
    dataProcessingImmediate(&machine, 0xB13FFC42);

    // 1 01 100 010  1 1111 1111 1111 00010  00010
    for (int i=0; i<16*8+1;i++) {
        dataProcessingImmediate(&machine, 0xB17FFC42);
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


int main(void) {
    int failed = 0;

    failed += !overflowFlagSet32BitAdd();
    failed += !add1ToReg();
    failed += !overflowFlagNotSet64BitAdd();
    failed += !sub1ToReg();
    failed += !wideMovz();

    printf("Executed tests with %d failures\n", failed);
}