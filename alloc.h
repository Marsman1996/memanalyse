#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "utils.h"

void alloc_init();

void alloc_routine_in(char *s_alloc_name, char *s_pc_count, char *s_pc_count2, char *s_addr, char *s_size, char *s_old_addr);

alloc_link_t alloc_check(uint32_t pc_count);

void leak_detect();