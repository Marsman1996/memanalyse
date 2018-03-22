#include <stdio.h>
#include <stdint.h>
#include "utils.h"

#define APP_BLOCK_BITS 16
//64KB
#define APP_BLOCK_SIZE (1 << APP_BLOCK_BITS)
//1 byte
#define REDZONE_SIZE 1

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
    
    uint32_t redzone_size;
    uint32_t redzone_value;

    uint32_t default_value;
    sp_blk_t default_block;

    uint32_t special_block_num;
    sp_blk_t special_blocks[MAX_NUM_SPECIAL_BLOCKS];
} map_t;