#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

int64_t load(int8_t *memory, uint64_t address, int size);

void store(int8_t *memory, uint64_t address, int64_t data, int size);

void execute_sdt(uint32_t instruction, struct Machine *machine);