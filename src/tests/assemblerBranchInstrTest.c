
#include "../assembler/instructions/branchInstr.h"
#include "../assembler/maps.h"

#include "../assembler/instructions/dataProcessingImm.h"

//TESTING UNCONDITIONAL BRANCHES

bool uncond() {
     Map* labelmap = createMap(4);
     char c = 'd';
     char* literal = &c;
     insertMap(labelmap, literal, 10);
     if (branchInstruction(0, literal, 5, NULL, labelmap) != 0x14000005) {
          fprintf(stderr, "Failed unconditional branch test\n");
          return false;
     }
     printf("Passed unconditional branch test\n");
     return true;
}

//TESTING CONDITIONAL BRANCHES

bool cond() {
     Map* labelmap = createMap(4);
     char c = 'd';
     char* literal = &c;
     insertMap(labelmap, literal, 15);
     if (branchInstruction(1, literal, 5, "al", labelmap) != 0x540000ae) {
          fprintf(stderr, "Failed conditional branch test\n");
          return false;
     }
     printf("Passed conditional branch test\n");
     return true;
}

//TESTING REGISTER BRANCHES

bool regist() {
     Map* labelmap = createMap(4);
     char c[] = "x1";
     if (branchInstruction(2, c, 5, NULL, labelmap) != 0xd61f0020) {
          fprintf(stderr, "Failed conditional branch test\n");
          return false;
     }
     printf("Passed conditional branch test\n");
     return true;
}

//TESTING WIDE MOVE

bool wideMove() {
     char instrStr[] = "x0 #0x1234 lsl #32";
     char* instr1 = &(instrStr[0]);
     if (wideMoveInstruction(0, instr1) != 0x92e24680) {
          fprintf(stderr, "Failed wide move test\n");
          return false;
     }
     printf("Passed wide move test\n");
     return true;
}

int main(void) {
    int failed = 0;

    failed += !uncond();
    failed += !cond();
    failed += !regist();
    
    failed += !wideMove();

    printf("Executed tests with %d failures\n", failed);
}