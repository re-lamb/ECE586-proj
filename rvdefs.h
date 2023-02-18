/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * Global definitions
 */

#ifndef _RVDEFS_
#define _RVDEFS_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MEMSZ       0x10000

int memload(uint8_t *mem, uint addr, uint8_t size, bool signxt);
void memstore(uint8_t *mem, uint addr, uint8_t size, int value);
void run(uint pc, uint sp, uint8_t *mem);

#endif
