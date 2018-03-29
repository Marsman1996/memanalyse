#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "utils.h"

typedef struct _alloc_routine_t{
    char name[20];
    uint32_t pc_count;
    app_pc addr;
    uint32_t size;
    app_pc old_addr;
}alloc_routine_t;

typedef struct _malloc_entry_t {
    app_pc start;
    //app_pc real_start;
    app_pc end;
    //alloc_type_t alloc_type;
} malloc_entry_t;

typedef struct _alloc_node_t{
    alloc_routine_t routine;
    struct _alloc_node_t *next;
} alloc_node_t, *alloc_link_t;

typedef struct _entry_node_t{
    malloc_entry_t entry;
    app_pc key;
    struct _entry_node_t *next;
} entry_node_t, *entry_link_t;

void alloc_init();

void alloc_routine_in(char *s_alloc_name, char *s_pc_count, char *s_addr, char *s_size, char *s_old_addr);

void alloc_check(uint32_t pc_count);