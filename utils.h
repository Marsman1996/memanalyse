#include <stdint.h>

#ifndef _UTILS_H
#define _UTILS_H
//1 byte
#define REDZONE_SIZE 1

#define ALIGN_BACKWARD(x, alignment) \
        (((uint32_t)x) & (~((uint32_t)(alignment)-1)))
#define ALIGN_FORWARD(x, alignment) \
        ((((uint32_t)x) + (((uint32_t)alignment)-1)) & \
         (~(((uint32_t)alignment)-1)))

#define POINTER_OVERFLOW_ON_ADD(ptr, add) \
    (((uint32_t)(ptr)) + (add) < ((uint32_t)(ptr)))
#define APP_RANGE_LOOP(app_addr, app_size, app_blk_base, app_blk_end, app_src_end,\
                       start, end, iter_size, loop_body) do {                     \
    app_src_end = app_addr + (app_size - 1); /* closed, to avoid overflow */      \
    for (app_blk_base = (app_pc) ALIGN_BACKWARD(app_addr, shadow_map->app_block_size);   \
         /* While the end is closed, we need to detect the loop end, so we rely   \
          * on the closed end never being aligned and thus allowing a < here      \
          * and an overflow check on the increment.                               \
          */                                                                      \
         app_blk_base < app_src_end;                                              \
         app_blk_base = POINTER_OVERFLOW_ON_ADD(app_blk_end, 1) ?                 \
             app_src_end : app_blk_end + 1) {                                     \
        app_blk_end = app_blk_base + (shadow_map->app_block_size - 1);                   \
        start       = app_addr > app_blk_base ? app_addr : app_blk_base;          \
        end         = app_src_end  < app_blk_end  ? app_src_end  : app_blk_end;   \
        iter_size   = (end - start + 1);                                          \
        loop_body                                                                 \
    }                                                                             \
} while (0);

typedef unsigned char byte;
typedef byte * app_pc;
typedef uint32_t* bitmap_t;

typedef struct _malloc_entry_t {
    app_pc start;
    //app_pc real_start;
    app_pc end;
    //alloc_type_t alloc_type;
    app_pc aligned_start;
    app_pc aligned_end;
    app_pc ptr[256];
} malloc_entry_t;


typedef struct _entry_node_t{
    malloc_entry_t entry;
    app_pc key;
    struct _entry_node_t *next;
} entry_node_t, *entry_link_t;

typedef struct _alloc_routine_t{
    char name[20];
    bool is_load;
    uint32_t pc_count;
    uint32_t pc_count2;
    app_pc addr;
    uint32_t size;
    app_pc old_addr;
    entry_link_t entry_link;
}alloc_routine_t;

typedef struct _alloc_node_t{
    alloc_routine_t routine;
    struct _alloc_node_t *next;
} alloc_node_t, *alloc_link_t;

#endif