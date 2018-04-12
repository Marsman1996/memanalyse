#include "shadow.h"
#include "alloc.h"
#include "error.h"

map_t *shadow_map;
static int static_shadow_table[SHADOW_TABLE_ENTRIES];

void shadow_block_remove_redzone(app_pc app_addr, uint32_t app_size){
    app_pc app_red_start, app_red_end;
    app_red_start = app_addr - REDZONE_SIZE;
    app_red_end = app_addr + app_size;

    shadow_block_write_byte(app_red_start, SHADOW_DEFINED);
    shadow_block_write_byte(app_red_end, SHADOW_DEFINED);
}

void shadow_block_add_redzone(app_pc app_addr, uint32_t app_size){
    app_pc app_red_start, app_red_end;
    app_red_start = app_addr - REDZONE_SIZE;
    app_red_end = app_addr + app_size;
    
    shadow_block_write_byte(app_red_start, SHADOW_REDZONE);
    shadow_block_write_byte(app_red_end, SHADOW_REDZONE);
}

byte* shadow_special_block_create(app_pc app_start, app_pc app_end, uint32_t val){
    if(shadow_is_in_special_block(app_start))
        return NULL;
    int i = shadow_map->special_block_num;

    shadow_map->special_blocks[i].start = app_start;
    shadow_map->special_blocks[i].end = app_end;
    shadow_map->special_blocks[i].value = val;
    shadow_map->special_block_num++;
}

byte* shadow_block_create(){
    byte *block;
    block = (byte *)malloc(shadow_map->shadow_block_size);
    
    return block;
}

void shadow_block_replace(app_pc app_block_base){//输入app_block
    byte *block;
    uint32_t idx;

    if(shadow_is_in_default_block(shadow_table_app_to_shadow((uint32_t)app_block_base))){
        //printf("shadow_block_replace: I'm replace\n");
        block = shadow_block_create();
        memset(block, shadow_map->default_value, shadow_map->shadow_block_size);
        idx = SHADOW_TABLE_INDEX(app_block_base);
        shadow_map->shadow_table[idx] = (int)block;
    }
}

void shadow_block_write_range(app_pc app_addr, uint32_t app_size, uint32_t dword_val){
    app_pc app_blk_base, app_blk_end, app_src_end;
    app_pc start, end;
    size_t size, iter_size;
    byte  *shadow_start;
    size_t shdw_size;
    uint32_t blk_val;
    size_t     blk_val_sz;

    shdw_size = 0;

    //TODO: 替换这个宏定义，如果有时间
    APP_RANGE_LOOP(app_addr, app_size, app_blk_base, app_blk_end, app_src_end, start, end, iter_size, {
        shadow_start = shadow_table_app_to_shadow((uint32_t)start);
        size = iter_size >> shadow_map->shift;
        if(shadow_is_in_special_block(start))
            assert(false);//TODO:
        if(shadow_is_in_default_block(shadow_start)){
            shadow_block_replace(app_blk_base);
            shadow_start = shadow_table_app_to_shadow((uint32_t)start);
        }
        memset(shadow_start, dword_val, size);
        //shdw_size += size;
    })
    
}

void shadow_block_write_byte(app_pc app_addr, uint32_t val){//写2bit
    app_pc app_base = (app_pc)ALIGN_BACKWARD(app_addr, shadow_map->app_block_size);
    app_pc shadow_base = shadow_table_app_to_shadow((uint32_t)app_base);

    if(shadow_is_in_special_block(app_addr))
        assert(false);//TODO:
    if(shadow_is_in_default_block(shadow_base)){
        shadow_block_replace(app_base);
        shadow_base = shadow_table_app_to_shadow((uint32_t)app_base);
    }

    uint32_t app_offset = app_addr - app_base;
    uint32_t shift = ((app_offset) % 16) * 2;
    bitmap_t sbm = (bitmap_t)shadow_base;
    uint32_t idx = app_offset / 16;
    uint32_t orig = sbm[idx];
    orig &= (((0xfffffffc | val) << shift) | (~(0xffffffff << shift)));
    orig |= (val << shift);
    sbm[idx] = orig;
}

uint32_t shadow_get_byte(app_pc app_addr){//读2bit
    app_pc shadow_addr = shadow_table_app_to_shadow((uint32_t)app_addr);
    uint32_t i = shadow_is_in_special_block(app_addr);
    if(i != 0)
        return shadow_map->special_blocks[i + 1].value;
    if(shadow_is_in_default_block(shadow_addr)){
        return SHADOW_UNADDRESSABLE;
    }
    app_pc app_base = (app_pc)ALIGN_BACKWARD(app_addr, shadow_map->app_block_size);
    app_pc shadow_base = shadow_table_app_to_shadow((uint32_t)app_base);
    uint32_t app_offset = (uint32_t)(app_addr - app_base);
    bitmap_t sbm = (bitmap_t)shadow_base;
    uint32_t idx = app_offset / 16;
    uint32_t shift = ((app_offset) % 16) * 2;
    return ((sbm[idx] >> shift) & 3);
}

//都是app_addr
void shadow_copy_range(app_pc old_start, app_pc new_start, uint32_t size){
    uint32_t i;
    //暴力复制
    byte *temp = (byte *)malloc(size);
    for(i = 0; i < size; i++)
        temp[i] = (byte) shadow_get_byte(old_start + i);
    for(i = 0; i < size; i++)
        shadow_block_write_byte(new_start + i, temp[i]);
    free(temp);
    return ;
    //FIXME: 取巧的方法
}

void shadow_write_range(app_pc app_start, app_pc app_end, uint32_t val){
    app_pc aligned_start, aligned_end;
    app_pc pc;

    aligned_start = (app_pc)ALIGN_FORWARD(app_start, 4);
    aligned_end = (app_pc)ALIGN_BACKWARD(app_end, 4);
    
    //对未对齐start赋值
    for(pc = app_start; pc < aligned_start && pc < app_end; pc++){
        shadow_block_write_byte(pc, val);
    }

    //对对齐部分赋值
    if(aligned_end > aligned_start){
        shadow_block_write_range(aligned_start, aligned_end - aligned_start, shadow_value_byte_to_dword(val));
    }
    //对未对齐end部分赋值
    for(pc = aligned_end; aligned_end >= aligned_start && pc < app_end; pc++){
        //dr_fprintf(STDERR, "END: write 0x%x in the 0x%08x \n", val, pc);
        shadow_block_write_byte(pc, val);
    }
}

void shadow_init(){
    shadow_map = (map_t *)malloc(sizeof(map_t));
    if(shadow_map == NULL){
        assert(false);
    }

    memset(shadow_map, 0, sizeof(*shadow_map));
    shadow_map->shift = 2;
    shadow_map->shadow_table = static_shadow_table;
    //64KB
    shadow_map->app_block_size = APP_BLOCK_SIZE;
    //16KB
    shadow_map->shadow_block_size = APP_BLOCK_SIZE >> shadow_map->shift;

    shadow_map->default_value = SHADOW_DWORD_UNADDRESSABLE;
    shadow_map->special_block_num = 0;

    byte *block_start;
    int i;

    //建立default块
    block_start = (byte *)malloc(shadow_map->shadow_block_size);
    // printf("default block start is 0x%08x\n", block_start);
    memset(block_start, shadow_map->default_value, shadow_map->shadow_block_size);
    
    shadow_map->default_block.start = block_start;
    shadow_map->default_block.value = shadow_map->default_value;
    shadow_map->default_block.value_size = 1;
    //将未初始化的shadow块挂在default上
    for(i = 0; i < SHADOW_TABLE_ENTRIES; i++){
        shadow_map->shadow_table[i] = (uint32_t)shadow_map->default_block.start;
    }

    // hashtable_config_t hashconfig;
    // //初始化hashtable,2^12个hash条目
    // hashtable_init(&malloc_table, 12, HASH_INTPTR, false);
    // hashconfig.size = sizeof(hashconfig);
    // hashconfig.resizable = true;
    // hashconfig.resize_threshold = 50;
}

uint32_t shadow_is_in_special_block(byte *app_addr){
    uint32_t i;
    for(i = 0; i < shadow_map->special_block_num; i++){
        if(app_addr >= shadow_map->special_blocks[i].start && app_addr < shadow_map->special_blocks[i].end)
            return i + 1;
    }
    return 0;
}

bool shadow_is_in_default_block(byte *shadow_addr){
    if(shadow_addr >= shadow_map->default_block.start && shadow_addr < (shadow_map->default_block.start + shadow_map->shadow_block_size))
        return true;
    else
        return false;
}

uint32_t shadow_value_byte_to_dword(uint32_t val){
    if(val == SHADOW_UNADDRESSABLE)
        return SHADOW_DWORD_UNADDRESSABLE;
    else if(val == SHADOW_UNDEFINED)
        return SHADOW_DWORD_UNDEFINED;
    else if(val == SHADOW_DEFINED)
        return SHADOW_DWORD_DEFINED;
    else if(val == SHADOW_REDZONE)
        return SHADOW_DWORD_REDZONE;
    else
        assert(false);
}

app_pc shadow_table_app_to_shadow(uint32_t app_addr){//是app_addr
    //右移16位，即除以64K内存块
    int idx = SHADOW_TABLE_INDEX(app_addr);
    uint32_t base = shadow_map->shadow_table[idx];
    uint32_t offset = (app_addr & (APP_BLOCK_SIZE - 1)) >> shadow_map->shift;
    return (app_pc)(base + offset);
}

void print_for_test(app_pc ptr){
    int i, j;
    //ptr--;
    printf("--------------- \n");
    printf("shadow is \n");
    ptr = ptr - 16;
    for(j = 0; j < 5; j ++){
        for(i = 0; i < 16; i++, ptr++)
            printf("%x", shadow_get_byte((app_pc)ptr));
        printf("\n");    
    }
    printf("--------------- \n");
}
