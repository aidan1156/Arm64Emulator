#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

int64_t shifting( int shift, int sf, int64_t rm, int operand, int opr );

void logic(struct Machine *machine, int opc, int64_t rn, int64_t op2, int sf, int rdAddress);

void multiply(struct Machine *machine, int64_t ra, int64_t rn, int64_t rm, int rdAddress, int sf, int x);

void dataProcessingRegister(struct Machine *machine, uint32_t instruction);
