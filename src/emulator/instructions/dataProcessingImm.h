#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// sign extend a 32 bit number to 64 bit
static int64_t extendTo64Bit(int64_t a);

static bool findAddCarry(int64_t a, int64_t b, int size);

static bool findSubCarry(int64_t a, int64_t b, int size);

void computeArithmeticOperation(struct Machine* machine, int64_t a, int64_t b, short opc, short sf, short rd);


static void arithmeticInstruction(struct Machine* machine, short rd, int operand, short opc, short sf);

static void wideMoveInstruction(struct Machine* machine, short rd, int operand, short opc, short sf);

void dataProcessingImmediate(struct Machine* machine, uint32_t instr);