#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "utils.h"

#define APP_BLOCK_BITS 16
//64KB
#define APP_BLOCK_SIZE (1 << APP_BLOCK_BITS)

//65536
#define SHADOW_TABLE_ENTRIES (1 << (32 - APP_BLOCK_BITS))
#define MAX_NUM_SPECIAL_BLOCKS 8
#define SHADOW_GRANULARITY 4

#define SHADOW_UNADDRESSABLE    0x1
#define SHADOW_UNDEFINED        0x3
#define SHADOW_DEFINED          0x0
#define SHADOW_REDZONE          0x2

#define SHADOW_DWORD_UNADDRESSABLE 0x55
#define SHADOW_DWORD_UNDEFINED     0xff
#define SHADOW_DWORD_DEFINED       0x00
#define SHADOW_DWORD_REDZONE       0xaa

#define SHADOW_TABLE_INDEX(addr) ((uint32_t)(addr) >> APP_BLOCK_BITS)

typedef struct {
    byte *start;
    byte *end;
    uint32_t value;
    size_t value_size;
} sp_blk_t;

typedef struct {
    uint32_t shift;
    uint32_t app_block_size;
    uint32_t shadow_block_size;
    int *shadow_table;
    uint32_t shadow_block_alloc_size;

    uint32_t default_value;
    sp_blk_t default_block;

    uint32_t special_block_num;
    sp_blk_t special_blocks[MAX_NUM_SPECIAL_BLOCKS];
} map_t;

void shadow_block_add_redzone(app_pc app_addr, uint32_t app_size);

byte* shadow_special_block_create(app_pc app_start, app_pc app_end, uint32_t val);

byte* shadow_block_create();

void shadow_block_replace(app_pc app_block_base);

uint32_t shadow_get_byte(app_pc addr);

void shadow_copy_range(app_pc old_start, app_pc new_start, uint32_t size);

void shadow_write_range(app_pc start, app_pc end, uint32_t val);

void shadow_block_write_range(app_pc app_addr, uint32_t app_size, uint32_t val);

void shadow_block_write_byte(app_pc app_addr, uint32_t val);

void shadow_init();

uint32_t shadow_is_in_special_block(byte *app_addr);

bool shadow_is_in_default_block(byte *shadow_addr);

app_pc shadow_table_app_to_shadow(uint32_t addr);

uint32_t shadow_value_byte_to_dword(uint32_t val);

void print_for_test(app_pc ptr);