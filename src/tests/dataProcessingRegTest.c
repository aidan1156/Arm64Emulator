
#include "../dataProcessingReg.c"

bool noShiftAnd() {
    struct Machine machine;
    initialiseMachine(&machine);

    machine.registers[1] = 0x0000000A;
    machine.registers[2] = 0x0000000B;

    dataProcessingRegister(&machine, 0x8A010043);
    // 64 bits
    // no shift : operand = 0
    // rm = 1
    // rn = 2
    // rd = 3
    // store rd := rm & rn

    if (machine.registers[3] != 0x00000015) {
        fprintf(stderr, "Failed noShiftAnd\n");
        return false;
    }
    return true;
}

int main(void) {
    int failed = 0;

    failed += !noShiftAnd();

    printf("%d failures\n", failed);
}