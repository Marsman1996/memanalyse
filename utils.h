#include <stdint.h>

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
