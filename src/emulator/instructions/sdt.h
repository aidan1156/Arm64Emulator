#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

uint64_t load(uint8_t *memory, uint64_t address, int size);

void store(uint8_t *memory, uint64_t address, uint64_t data, int size);

void execute_sdt(uint32_t instruction, struct Machine *machine);