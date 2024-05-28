#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

int shifting( int shift, int sf, int rm, int operand, int opr );

void logic(struct Machine *machine, int opc, int rd, int rn, int op2, int sf, int rmAddress);

void multiply(struct Machine *machine, int ra, int rn, int rm, int rdAddress, int sf, int x);

void dataProcessingRegister(struct Machine *machine, uint32_t instruction);
