
#include "../branchInstr.c"

//TESTING UNCONDITIONAL BRANCHES

bool uncondSmallOffset() {
    struct Machine machine;
    initialiseMachine(&machine);
    branchInstruction(&machine, 0x14000003); // simm26 = 3

    if (machine.PC != 12) {
         fprintf(stderr, "Failed unconditional small offset\n");
         return false;
    }
    printf("Passed unconditional small offset\n");
    return true;
}

bool uncondLargeOffset() {
    struct Machine machine;
    initialiseMachine(&machine);
    branchInstruction(&machine, 0x1400FFFF); // simm26 = 65535
    
    if (machine.PC != 262140) {
         fprintf(stderr, "Failed unconditional large offset\n");
         return false;
    }
    printf("Passed unconditional large offset\n");
    return true;
}

//TESTING REGISTER BRANCHES

bool registerSmallXN() {
    struct Machine machine;
    initialiseMachine(&machine);
    branchInstruction(&machine, 0XD61F0020); // xn = 1
    
    if (machine.PC != 1) {
         fprintf(stderr, "Failed register small xn\n");
         return false;
    }
    printf("Passed register small xn\n");
    return true;
}

bool registerLargeXN() {
    struct Machine machine;
    initialiseMachine(&machine);
    branchInstruction(&machine, 0xD61F03E0); // xn = 15
    
    if (machine.PC != 31) {
         fprintf(stderr, "Failed register large xn\n");
         return false;
    }
    printf("Passed register large xn\n");
    return true;
}

//TESTING CONDITIONAL BRANCHES

bool condAlwaysSmallOffset() {
    struct Machine machine;
    initialiseMachine(&machine);
    branchInstruction(&machine, 0X5400002E); // simm19 = 1, cond = AL
    
    if (machine.PC != 4) {
         fprintf(stderr, "Failed cond small offset\n");
         return false;
    }
    printf("Passed cond small offset\n");
    return true;
}

bool condAlwaysLargeOffset() {
    struct Machine machine;
    initialiseMachine(&machine);
    branchInstruction(&machine, 0x547FE00E); // simm19 = 261888, cond = AL
    if (machine.PC != 1047552) { 
         fprintf(stderr, "Failed cond large offset\n");
         return false;
    }
    printf("Passed cond large offset\n");
    return true;
}

int main(void) {
    int failed = 0;

    failed += !uncondSmallOffset();
    failed += !uncondLargeOffset();
    failed += !registerSmallXN();
    failed += !registerLargeXN();
    failed += !condAlwaysSmallOffset();
    failed += !condAlwaysLargeOffset();

    printf("Executed tests with %d failures\n", failed);
}