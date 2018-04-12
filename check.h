#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "utils.h"

#define MAX_STACK 512

void check(uint32_t write, app_pc pc, const char *instr, app_pc app_addr, uint32_t size, app_pc esp, uint32_t content, uint32_t pc_count, alloc_link_t alloc_link);