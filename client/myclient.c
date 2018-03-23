#include "myclient.h"

static client_id_t client_id;
//static void *lock; /* protects alloc_routine_table */
static int tcls_idx;
static int tls_idx;
static int static_shadow_table[SHADOW_TABLE_ENTRIES];
map_t *shadow_map;
static hashtable_t malloc_table;

static void event_exit(void){
    print_special_block(shadow_map);
    dr_fprintf(STDERR, "Client said bye-bye\n");
    drmgr_unregister_cls_field(event_thread_context_init,
                               event_thread_context_exit,
                               tcls_idx);
    drmgr_unregister_tls_field(tls_idx);
    drmgr_unregister_thread_init_event(event_thread_init);
    drmgr_unregister_thread_exit_event(event_thread_exit);
    drmgr_unregister_bb_insertion_event(event_app_instruction);

    drwrap_exit();
    drutil_exit();
    drreg_exit();
    drmgr_exit();
    drx_exit();
}
// add redzone
void handle_free_post(arg_t *data){
    malloc_entry_t *e;
    e = hashtable_lookup(&malloc_table, (void *)(data->ptr));
    if(e == NULL){
        DR_ASSERT(false);
    }
    uint32_t size;
    shadow_write_range(e->start - REDZONE_SIZE, e->end + REDZONE_SIZE, SHADOW_UNADDRESSABLE);
    hashtable_remove(&malloc_table, e->start);
}

void handle_malloc_post(arg_t *data, app_pc ret){
    malloc_entry_add(ret, ret + data->size, ALLOC_TYPE_MALLOC);
    shadow_write_range(ret, (ret + data->size), SHADOW_UNDEFINED);
    shadow_block_add_redzone(ret, data->size);
}

void handle_realloc_post(arg_t *data, app_pc ret){
    //add data->ptr = NULL condition, 相当于malloc
    if(data->ptr == NULL){
        handle_malloc_post(data, ret);
        return ;
    }
    malloc_entry_t *e;
    e = hashtable_lookup(&malloc_table, (void *)(data->ptr));
    if(e == NULL){
        DR_ASSERT(false);
    }
    uint32_t old_size = e->end - e->start;
    //写new区域
    if(data->size > old_size){//new区域大
        if(ret != data->ptr)
            shadow_copy_range(data->ptr, ret, old_size);
        shadow_write_range(ret + old_size, ret + data->size, SHADOW_UNDEFINED);
    }
    else {//old区域大
        if(ret != data->ptr)
            shadow_copy_range(data->ptr, ret, data->size);
    }
    //消除old区域
    if(ret > data->ptr){//比较start, 若new在old后,
        shadow_write_range((app_pc)data->ptr, (app_pc)((ret < data->ptr + old_size) ? ret : e->end), SHADOW_UNADDRESSABLE);
    }
    if(ret + data->size < e->end){//比较end 若new在old前
        shadow_write_range((app_pc)((ret + data->size > data->ptr) ? ret + data->size : data->ptr), e->end, SHADOW_UNADDRESSABLE);
    }
    //加redzone
    shadow_block_add_redzone(ret, data->size);
    //添加新hash
    malloc_entry_add(ret, ret + data->size, ALLOC_TYPE_MALLOC);
    //销毁旧hash
    hashtable_remove(&malloc_table, e->start);
}

void handle_calloc_post(arg_t *data, app_pc ret){
    malloc_entry_add(ret, ret + data->size, ALLOC_TYPE_MALLOC);
    shadow_write_range(ret, ret + data->size, SHADOW_DEFINED);
    shadow_block_add_redzone(ret, data->size);
}

static void hook_alloc_post(void *wrapcxt, void *user_data){
    int i;
    possible_alloc_routine_t *possible;
    arg_t *data;
    void *drcontext;

    drcontext = (void *)drwrap_get_drcontext(wrapcxt);
    data = (arg_t *)drmgr_get_cls_field(drcontext, tcls_idx);
    possible = user_data;
    //防止两次malloc, realloc之类的
    if(data->skip == true){
        data->skip = false;
        return ;
    }
    data->lock = false;
    dr_fprintf(STDERR, "release %s\n", possible->name);

    if(possible->type == HEAP_ROUTINE_FREE){
        dr_fprintf(STDERR, "ptr is 0x%08x \n", data->ptr);

        handle_free_post(data);

        opnd_t pc_count;
        pc_count = dr_raw_tls_opnd(drcontext, reg_pc_count, reg_pc_offs + INSTRACE_TLS_OFFS_BUF_PTR);
        //pc_count = dr_get_dr_segment_base(reg_pc_count);
        dr_fprintf(STDERR, "pc_count is %u \n", opnd_get_immed_int(pc_count));

        print_for_test(data->ptr);
    }
    else if(possible->type == HEAP_ROUTINE_MALLOC){
        dr_fprintf(STDERR, "size is %u \n", data->size);
        app_pc ret = (app_pc)drwrap_get_retval(wrapcxt);
        dr_fprintf(STDERR, "ret is 0x%08x \n", ret);

        handle_malloc_post(data, (app_pc)(ret + REDZONE_SIZE));

        print_for_test(ret);
        drwrap_set_retval(wrapcxt, (void *)(ret + REDZONE_SIZE));
    }
    else if(possible->type == HEAP_ROUTINE_REALLOC){
        dr_fprintf(STDERR, "ptr is 0x%08x \n", data->ptr);
        dr_fprintf(STDERR, "size is %u \n", data->size);
        app_pc real_ret = (app_pc)drwrap_get_retval(wrapcxt);
        dr_fprintf(STDERR, "real_ret is 0x%08x \n", real_ret);
        
        handle_realloc_post(data, real_ret + REDZONE_SIZE);

        print_for_test(real_ret);
        drwrap_set_retval(wrapcxt, (void *)(real_ret + REDZONE_SIZE));
    }
    else if(possible->type == HEAP_ROUTINE_CALLOC){
        dr_fprintf(STDERR, "size is %u \n", data->size);
        app_pc ret = (app_pc)drwrap_get_retval(wrapcxt);
        dr_fprintf(STDERR, "ret is 0x%08x \n", ret);

        handle_calloc_post(data, ret + REDZONE_SIZE);

        print_for_test(ret);
        drwrap_set_retval(wrapcxt, ret + REDZONE_SIZE);
    }
}

static void hook_alloc_pre(void *wrapcxt, INOUT void **user_data){
    //int i;
    possible_alloc_routine_t *possible;
    void *drcontext;
    arg_t *data;
    static int count = 1;

    drcontext = (void *)drwrap_get_drcontext(wrapcxt);
    data = (arg_t *)drmgr_get_cls_field(drcontext, tcls_idx);
    possible = *user_data;
    //防止两次malloc, realloc之类的
    data->skip = false;
    if(data->lock == true){
        data->skip = true;
        return ;
    }
    data->lock = true;

    dr_fprintf(STDERR, "%d. catch %s\n", count, possible->name);

    if(possible->type == HEAP_ROUTINE_FREE){
        data->ptr = (app_pc)drwrap_get_arg(wrapcxt, 0);
        dr_fprintf(STDERR, "ptr is 0x%08x \n", data->ptr);
        
        drwrap_set_arg(wrapcxt, 0, (void *)(data->ptr - REDZONE_SIZE));
    }
    else if(possible->type == HEAP_ROUTINE_MALLOC){
        data->size = (uint32_t)drwrap_get_arg(wrapcxt, 0);
        dr_fprintf(STDERR, "size is %u \n", data->size);

        drwrap_set_arg(wrapcxt, 0, (void *)(data->size + 2*REDZONE_SIZE));
    }
    else if(possible->type == HEAP_ROUTINE_REALLOC){
        data->ptr = (app_pc)drwrap_get_arg(wrapcxt, 0);
        data->size = (uint32_t)drwrap_get_arg(wrapcxt, 1);
        dr_fprintf(STDERR, "ptr is 0x%08x \n", data->ptr);
        dr_fprintf(STDERR, "size is %u \n", data->size);

        if(data->ptr != NULL)
            drwrap_set_arg(wrapcxt, 0, (void *)(data->ptr - REDZONE_SIZE));
        drwrap_set_arg(wrapcxt, 1, (void *)(data->size + 2*REDZONE_SIZE));
    }
    else if(possible->type == HEAP_ROUTINE_CALLOC){//设置成1*(size+2)??
        data->size = (uint32_t)drwrap_get_arg(wrapcxt, 0) * (uint32_t)drwrap_get_arg(wrapcxt, 1);
        dr_fprintf(STDERR, "size is %u \n", data->size);

        drwrap_set_arg(wrapcxt, 0, (void *)1);
        drwrap_set_arg(wrapcxt, 1, (void *)(data->size + 2*REDZONE_SIZE));
    }
    count++;
}

void
find_alloc_routines(const module_data_t *mod, //就是info
                    const possible_alloc_routine_t *possible, 
                    uint num_possible){
    int i, j;
    int opcode;
    for(i = 0; i < num_possible; i++){
        app_pc pc;
        const char *name = possible[i].name;
        pc = (app_pc) dr_get_proc_address(mod->handle, name);
        if(pc != NULL){
            dr_fprintf(STDERR, "%s \t", name);
            dr_fprintf(STDERR, "0x%08x \n", pc);
            if(!drwrap_wrap_ex(pc, hook_alloc_pre, hook_alloc_post, (void *)&possible[i], DRWRAP_CALLCONV_DEFAULT))
                DR_ASSERT(false);
        }
    }
}


void alloc_module_load(void *drcontext, const module_data_t *info, bool loaded){
    bool search_syms = true;
    bool is_libc;
    const char *modname = dr_module_preferred_name(info);
    //is_libc = text_match(module_name, "libc*");
    //is_libc = strcmp(module_name, "libc.so.6");
    if (modname != NULL && (strcmp(modname, "libdynamorio.so") == 0 ||strcmp(modname, "libmyclient.so") == 0))
        search_syms = false;
    is_libc = text_matches_pattern(modname, "libc*", false);
    if(search_syms){
        if(is_libc){
            find_alloc_routines(info, 
                                possible_libc_routines, 
                                POSSIBLE_LIBC_ROUTINE_NUM);
        }
    }
}

static void event_module_load(void *drcontext, const module_data_t *info, bool loaded){
    const char *module_name = dr_module_preferred_name(info);
    shadow_special_block_create(info->start, info->end, SHADOW_DEFINED);
    dr_fprintf(STDERR, "[MODUL] mn:%s \nms: 0x%08x me: 0x%08x\n", module_name, (ptr_uint_t)info->start, (ptr_uint_t)info->end);
    //dr_fprintf(STDERR, "%d\n", info->contiguous);证明连续
    alloc_module_load(drcontext, info, loaded);
}

static void
event_thread_context_init(void *drcontext, bool new_depth){
    //dr_fprintf(STDERR, "new thread context id="TIDFMT"%s\n", dr_get_thread_id(drcontext), new_depth ? " new depth" : "");
    arg_t *data;
    if(new_depth){
        data = (arg_t *)dr_thread_alloc(drcontext, sizeof(arg_t));
        drmgr_set_cls_field(drcontext, tcls_idx, data);
    }
    else
        data = (arg_t *)drmgr_get_cls_field(drcontext, tcls_idx);
    memset(data, 0, sizeof(*data));
}

static void
event_thread_context_exit(void *drcontext, bool thread_exit){
    //dr_fprintf(STDERR, "resuming prior thread context id="TIDFMT"\n",dr_get_thread_id(drcontext));
    if (thread_exit) {
        arg_t *data = (arg_t *) drmgr_get_cls_field(drcontext, tcls_idx);
        dr_thread_free(drcontext, data, sizeof(arg_t));
    }
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
    block = dr_global_alloc(shadow_map->shadow_block_size);
    
    return block;
}

void shadow_block_replace(app_pc app_block_base){//输入app_block
    byte *block;
    uint32_t idx;

    if(shadow_is_in_default_block(shadow_table_app_to_shadow((uint32_t)app_block_base))){
        dr_fprintf(STDERR, "shadow_block_replace: I'm replace\n");
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
    ptr_uint_t blk_val;
    size_t     blk_val_sz;

    shdw_size = 0;

    //TODO: 替换这个宏定义，如果有时间
    APP_RANGE_LOOP(app_addr, app_size, app_blk_base, app_blk_end, app_src_end, start, end, iter_size, {
        shadow_start = shadow_table_app_to_shadow((uint32_t)start);
        size = iter_size >> shadow_map->shift;
        if(shadow_is_in_special_block(start))
            DR_ASSERT(false);//TODO: 
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
        DR_ASSERT(false);//TODO:
    if(shadow_is_in_default_block(shadow_base)){
        shadow_block_replace(app_base);
        shadow_base = shadow_table_app_to_shadow((uint32_t)app_base);
    }

    uint app_offset = app_addr - app_base;
    uint shift = ((app_offset) % 16) * 2;
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
    byte *temp = dr_global_alloc(size);
    for(i = 0; i < size; i++)
        temp[i] = (byte) shadow_get_byte(old_start + i);
    for(i = 0; i < size; i++)
        shadow_block_write_byte(new_start + i, temp[i]);
    dr_global_free(temp, size);
    return ;
    //FIXME: 取巧的方法
}

void shadow_write_range(app_pc start, app_pc end, uint val){
    app_pc aligned_start, aligned_end;
    app_pc pc;

    aligned_start = (app_pc)ALIGN_FORWARD(start, 4);
    aligned_end = (app_pc)ALIGN_BACKWARD(end, 4);
    
    //对未对齐start赋值
    for(pc = start; pc < aligned_start && pc < end; pc++){
        shadow_block_write_byte(pc, val);
    }

    //对对齐部分赋值
    if(aligned_end > aligned_start){
        shadow_block_write_range(aligned_start, aligned_end - aligned_start, shadow_value_byte_to_dword(val));
    }
    //对未对齐end部分赋值
    for(pc = aligned_end; aligned_end >= aligned_start && pc < end; pc++){
        //dr_fprintf(STDERR, "END: write 0x%x in the 0x%08x \n", val, pc);
        shadow_block_write_byte(pc, val);
    }
}

void malloc_entry_add(app_pc start, app_pc end, alloc_type_t alloc_type){
    //alloc_type用于区分 malloc, new, new[]
    malloc_entry_t *e = (malloc_entry_t *)dr_global_alloc(sizeof(*e));
    malloc_entry_t *old_e;

    e->start = start;
    e->end = end;
    e->alloc_type = alloc_type;
    e->data = NULL;
    old_e = hashtable_add_replace(&malloc_table, (void *)start, (void *)e);
    if(old_e != NULL){
        dr_global_free(old_e, sizeof(old_e));
    }
}

void shadow_init(){
    shadow_map = (map_t *)dr_global_alloc(sizeof(map_t));
    if(shadow_map == NULL)
        DR_ASSERT(false);
    memset(shadow_map, 0, sizeof(*shadow_map));
    shadow_map->shift = 2;
    shadow_map->shadow_table = static_shadow_table;
    //64KB
    shadow_map->app_block_size = APP_BLOCK_SIZE;
    //16KB
    shadow_map->shadow_block_size = APP_BLOCK_SIZE >> shadow_map->shift;
    //0.5KB
    shadow_map->redzone_size = 512;
    shadow_map->redzone_value = SHADOW_DWORD_REDZONE;
    shadow_map->default_value = SHADOW_DWORD_UNADDRESSABLE;
    shadow_map->special_block_num = 0;

    sp_blk_t sp;
    byte *block_start;
    int i;
    uint32_t tempbase;

    //建立default块
    block_start = (byte *)dr_nonheap_alloc(shadow_map->shadow_block_size, DR_MEMPROT_READ|DR_MEMPROT_WRITE);
    dr_fprintf(STDERR, "default block start is 0x%08x\n", block_start);
    //FIXME: add redzone
    //& size加上redzone size*2
    memset(block_start, shadow_map->default_value, shadow_map->shadow_block_size);
    dr_memory_protect(block_start, shadow_map->shadow_block_size, DR_MEMPROT_READ);
    shadow_map->default_block.start = block_start;
    shadow_map->default_block.value = shadow_map->default_value;
    shadow_map->default_block.value_size = 1;
    //将未初始化的shadow块挂在default上
    for(i = 0; i < SHADOW_TABLE_ENTRIES; i++){
        shadow_map->shadow_table[i] = (int)shadow_map->default_block.start;
    }

    hashtable_config_t hashconfig;
    //初始化hashtable,2^12个hash条目
    hashtable_init(&malloc_table, 12, HASH_INTPTR, false);
    hashconfig.size = sizeof(hashconfig);
    hashconfig.resizable = true;
    hashconfig.resize_threshold = 50;
}

uint32_t shadow_is_in_special_block(byte *app_addr){
    int i;
    for(i = 0; i < shadow_map->special_block_num; i++){
        if(app_addr >= shadow_map->special_blocks[i].start && app_addr < shadow_map->special_blocks[i].end)
            return i + 1;
    }
    return 0;
}

bool shadow_is_in_default_block(byte *shadow_addr){
    //FIXME: about redzone？？
    if(shadow_addr >= shadow_map->default_block.start && shadow_addr < (shadow_map->default_block.start + shadow_map->shadow_block_size))
        return true;
    else
        return false;
}

uint shadow_value_byte_to_dword(uint32_t val){
    if(val == SHADOW_UNADDRESSABLE)
        return SHADOW_DWORD_UNADDRESSABLE;
    else if(val == SHADOW_UNDEFINED)
        return SHADOW_DWORD_UNDEFINED;
    else if(val == SHADOW_DEFINED)
        return SHADOW_DWORD_DEFINED;
    else if(val == SHADOW_REDZONE)
        return SHADOW_DWORD_REDZONE;
    else
        DR_ASSERT(false);
}

app_pc shadow_table_app_to_shadow(uint32_t addr){//是app_addr
    //右移16位，即除以64K内存块
    int idx = SHADOW_TABLE_INDEX(addr);
    uint32_t base = shadow_map->shadow_table[idx];
    uint32_t offset = (addr & (APP_BLOCK_SIZE - 1)) >> shadow_map->shift;
    return (app_pc)(base + offset);
}

void shadow_check_read(app_pc app_addr, uint32_t size, app_pc esp, app_pc ebp){
    int i;
    uint32_t shadow_value;
    //uint32_t shadow_addr;
    
    for(i = 0; i < size; i++){
        if(app_addr >= esp && app_addr <= ebp)
            continue;
        //shadow_addr = shadow_table_app_to_shadow(app_addr + i);
        shadow_value = shadow_get_byte(app_addr + i);
        if(shadow_value == SHADOW_REDZONE){//读越界
            DR_ASSERT(false);
        }
        if(shadow_value == SHADOW_UNADDRESSABLE || shadow_value == SHADOW_UNDEFINED){
            DR_ASSERT(false);
        }
    }
}

void shadow_check_write(app_pc app_addr, uint32_t size, app_pc esp){
    int i;
    uint32_t shadow_value;
    //app_pc shadow_addr;
    
    for(i = 0; i < size; i++){
        if(shadow_is_in_special_block(app_addr + i))
            DR_ASSERT(false);
        //shadow_addr = shadow_table_app_to_shadow(app_addr + i);
        shadow_value = shadow_get_byte(app_addr + i);
        if(shadow_value == SHADOW_UNADDRESSABLE){
            DR_ASSERT(false);
        }
        else if(shadow_value == SHADOW_REDZONE){//写越界
            DR_ASSERT(false);
        }
        else if(shadow_value == SHADOW_UNDEFINED){
            shadow_block_write_byte(app_addr + i, SHADOW_DEFINED);
        }
    }
}

static void
instrace(void *drcontext){
    per_thread_t *data;
    ins_ref_t *ins_ref, *buf_ptr;

    //dr_fprintf(STDERR, "In trace\n");
    data    = drmgr_get_tls_field(drcontext, tls_idx);
    buf_ptr = BUF_PTR(data->seg_base);
    for (ins_ref = (ins_ref_t *)data->buf_base; ins_ref < buf_ptr; ins_ref++) {
        fprintf(data->logf, "%d 0x%08x %8s %d 0x%08x 0x%08x 0x%08x %d\n",
                ins_ref->iswrite,
                (ptr_uint_t)ins_ref->pc,
                decode_opcode_name(ins_ref->opcode),
                ins_ref->size,
                (ptr_uint_t)ins_ref->addr,
                (ptr_uint_t)ins_ref->esp,
                (ptr_uint_t)ins_ref->ebp,
                ins_ref->pc_count);
        data->num_refs++;
    }
    BUF_PTR(data->seg_base) = data->buf_base;
}

static void
clean_call(void){
    void *drcontext = dr_get_current_drcontext();
    instrace(drcontext);
}

static void
insert_load_buf_ptr(void *drcontext, instrlist_t *ilist, instr_t *where,
                    reg_id_t reg_ptr){
    dr_insert_read_raw_tls(drcontext, ilist, where, tls_seg,
                           tls_offs + INSTRACE_TLS_OFFS_BUF_PTR, reg_ptr);
}

static void
insert_update_buf_ptr(void *drcontext, instrlist_t *ilist, instr_t *where,
                      reg_id_t reg_ptr, int adjust){
    MINSERT(ilist, where,
            XINST_CREATE_add(drcontext,
                             opnd_create_reg(reg_ptr),
                             OPND_CREATE_INT16(adjust)));
    dr_insert_write_raw_tls(drcontext, ilist, where, tls_seg,
                            tls_offs + INSTRACE_TLS_OFFS_BUF_PTR, reg_ptr);
}

static void
insert_save_opcode(void *drcontext, instrlist_t *ilist, instr_t *where,
                   reg_id_t base, reg_id_t scratch, int opcode){
    scratch = reg_resize_to_opsz(scratch, OPSZ_2);
    MINSERT(ilist, where,
            XINST_CREATE_load_int(drcontext,
                                  opnd_create_reg(scratch),
                                  OPND_CREATE_INT16(opcode)));
    MINSERT(ilist, where,
            XINST_CREATE_store_2bytes(drcontext,
                                      OPND_CREATE_MEM16(base,
                                                        offsetof(ins_ref_t, opcode)),
                                      opnd_create_reg(scratch)));
}

static void
insert_save_pc_count(void *drcontext, instrlist_t *ilist, instr_t *where,
                   reg_id_t base, reg_id_t scratch){
    scratch = reg_resize_to_opsz(scratch, OPSZ_4);
    dr_insert_read_raw_tls(drcontext, ilist, where, reg_pc_count,
                           reg_pc_offs + INSTRACE_TLS_OFFS_BUF_PTR, scratch);
    MINSERT(ilist, where,
            XINST_CREATE_add(drcontext,
                             opnd_create_reg(scratch),
                             OPND_CREATE_INT32(1)));
    dr_insert_write_raw_tls(drcontext, ilist, where, reg_pc_count,
                            reg_pc_offs + INSTRACE_TLS_OFFS_BUF_PTR, scratch);
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext,
                                      OPND_CREATE_MEM32(base,
                                                        offsetof(ins_ref_t, pc_count)),
                                      opnd_create_reg(scratch)));
}

static void
insert_save_pc(void *drcontext, instrlist_t *ilist, instr_t *where,
               reg_id_t base, reg_id_t scratch, app_pc pc){
    instrlist_insert_mov_immed_ptrsz(drcontext, (ptr_int_t)pc,
                                     opnd_create_reg(scratch),
                                     ilist, where, NULL, NULL);
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext,
                               OPND_CREATE_MEMPTR(base,
                                                  offsetof(ins_ref_t, pc)),
                               opnd_create_reg(scratch)));
}

static void
insert_save_esp(void *drcontext, instrlist_t *ilist, instr_t *where,
               reg_id_t base, reg_id_t scratch){
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext,
                               OPND_CREATE_MEMPTR(base,
                                                  offsetof(ins_ref_t, esp)),
                               opnd_create_reg(DR_REG_ESP)));
}

static void
insert_save_ebp(void *drcontext, instrlist_t *ilist, instr_t *where,
               reg_id_t base, reg_id_t scratch){
    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext,
                               OPND_CREATE_MEMPTR(base,
                                                  offsetof(ins_ref_t, ebp)),
                               opnd_create_reg(DR_REG_EBP)));
}

static void
insert_save_addr(void *drcontext, instrlist_t *ilist, instr_t *where,
               reg_id_t base, reg_id_t scratch, opnd_t tar){    
    drutil_insert_get_mem_addr(drcontext, ilist, where, tar, scratch, DR_REG_NULL);

    MINSERT(ilist, where,
            XINST_CREATE_store(drcontext,
                               OPND_CREATE_MEMPTR(base,
                                                  offsetof(ins_ref_t, addr)),
                               opnd_create_reg(scratch)));
}

static void
insert_save_iswrite(void *drcontext, instrlist_t *ilist, instr_t *where,
                   reg_id_t base, reg_id_t scratch, int iswrite){
    scratch = reg_resize_to_opsz(scratch, OPSZ_2);
    MINSERT(ilist, where,
            XINST_CREATE_load_int(drcontext,
                                  opnd_create_reg(scratch),
                                  OPND_CREATE_INT16(iswrite)));
    MINSERT(ilist, where,
            XINST_CREATE_store_2bytes(drcontext,
                                      OPND_CREATE_MEM16(base,
                                                        offsetof(ins_ref_t, iswrite)),
                                      opnd_create_reg(scratch)));
}

static void
insert_save_size(void *drcontext, instrlist_t *ilist, instr_t *where,
                   reg_id_t base, reg_id_t scratch, int size){
    scratch = reg_resize_to_opsz(scratch, OPSZ_2);
    MINSERT(ilist, where,
            XINST_CREATE_load_int(drcontext,
                                  opnd_create_reg(scratch),
                                  OPND_CREATE_INT16(size)));
    MINSERT(ilist, where,
            XINST_CREATE_store_2bytes(drcontext,
                                      OPND_CREATE_MEM16(base,
                                                        offsetof(ins_ref_t, size)),
                                      opnd_create_reg(scratch)));
}

static void
instrument_instr(void *drcontext, instrlist_t *ilist, instr_t *where, opnd_t tar, int iswrite){
    /* We need two scratch registers */
    reg_id_t reg_ptr, reg_tmp;

    drvector_t allowed;
    drreg_init_and_fill_vector(&allowed, true);
    drreg_set_vector_entry(&allowed, DR_REG_XBP, false);
    drreg_set_vector_entry(&allowed, DR_REG_XSP, false);

    if (drreg_reserve_register(drcontext, ilist, where, &allowed, &reg_ptr) !=
        DRREG_SUCCESS ||
        drreg_reserve_register(drcontext, ilist, where, &allowed, &reg_tmp) !=
        DRREG_SUCCESS) {
        DR_ASSERT(false); /* cannot recover */
        return;
    }

    insert_load_buf_ptr(drcontext, ilist, where, reg_ptr);
    insert_save_pc(drcontext, ilist, where, reg_ptr, reg_tmp, 
                    instr_get_app_pc(where));
    insert_save_esp(drcontext, ilist, where, reg_ptr, reg_tmp);
    insert_save_ebp(drcontext, ilist, where, reg_ptr, reg_tmp);
    insert_save_opcode(drcontext, ilist, where, reg_ptr, reg_tmp, 
                    instr_get_opcode(where));
    insert_save_pc_count(drcontext, ilist, where, reg_ptr, reg_tmp);
    insert_save_size(drcontext, ilist, where, reg_ptr, reg_tmp, 
                    drutil_opnd_mem_size_in_bytes(tar, where));
    if(iswrite == 1){
        insert_save_addr(drcontext, ilist, where, reg_ptr, reg_tmp, tar);
        insert_save_iswrite(drcontext, ilist, where, reg_ptr, reg_tmp, 1);
    }
    else{
        insert_save_addr(drcontext, ilist, where, reg_ptr, reg_tmp, tar);
        insert_save_iswrite(drcontext, ilist, where, reg_ptr, reg_tmp, 0);
    }
    insert_update_buf_ptr(drcontext, ilist, where, reg_ptr, sizeof(ins_ref_t));

    /* Restore scratch registers */
    if (drreg_unreserve_register(drcontext, ilist, where, reg_ptr) != DRREG_SUCCESS ||
        drreg_unreserve_register(drcontext, ilist, where, reg_tmp) != DRREG_SUCCESS)
        DR_ASSERT(false);
}

static dr_emit_flags_t
event_app_instruction(  void *drcontext, void *tag, instrlist_t *bb, 
                        instr_t *instr, bool for_trace, 
                        bool translating, void *user_data){
    int i;
    /* we don't want to auto-predicate any instrumentation */
    drmgr_disable_auto_predication(drcontext, bb);

    if (!instr_is_app(instr))
        return DR_EMIT_DEFAULT;

    /* insert code to add an entry to the buffer */
    if(instr_writes_memory(instr)){
        for(i = 0; i < instr_num_dsts(instr); i++){
            if (opnd_is_memory_reference(instr_get_dst(instr, i))) {
                instrument_instr(drcontext, bb, instr, instr_get_dst(instr, i), 1);
            }
        }
    }

    if(instr_reads_memory(instr)){
        for(i = 0; i < instr_num_srcs(instr); i++){
            if (opnd_is_memory_reference(instr_get_src(instr, i))) {
                instrument_instr(drcontext, bb, instr, instr_get_src(instr, i), 0);
            }
        }
    }
    
    /* insert code once per bb to call clean_call for processing the buffer */
    if (drmgr_is_first_instr(drcontext, instr) IF_AARCHXX(&& !instr_is_exclusive_store(instr))){
        dr_insert_clean_call(drcontext, bb, instr, (void *)clean_call, false, 0);

    }
    return DR_EMIT_DEFAULT;
}

static void 
event_thread_init(void *drcontext){
    per_thread_t *data = dr_thread_alloc(drcontext, sizeof(per_thread_t));
    DR_ASSERT(data != NULL);
    drmgr_set_tls_field(drcontext, tls_idx, data);

    data->seg_base = dr_get_dr_segment_base(tls_seg);
    data->buf_base = dr_raw_mem_alloc(MEM_BUF_SIZE,
                                      DR_MEMPROT_READ | DR_MEMPROT_WRITE,
                                      NULL);
    DR_ASSERT(data->seg_base != NULL && data->buf_base != NULL);
    /* put buf_base to TLS as starting buf_ptr */
    BUF_PTR(data->seg_base) = data->buf_base;

    data->num_refs = 0;
    data->log = log_file_open(client_id, drcontext, NULL /* using client lib path */,
                              "instrace",
                              DR_FILE_ALLOW_LARGE);
    data->logf = log_stream_from_file(data->log);
    fprintf(data->logf, "Format: <iswrite>,<instr address>,<opcode>,<address>,<esp>,<ebp>\n");
}

static void 
event_thread_exit(void *drcontext){
    per_thread_t *data;
    instrace(drcontext); /* dump any remaining buffer entries */
    data = drmgr_get_tls_field(drcontext, tls_idx);
    dr_mutex_lock(mutex);
    num_refs += data->num_refs;
    dr_mutex_unlock(mutex);
    log_stream_close(data->logf); /* closes fd too */
    dr_raw_mem_free(data->buf_base, MEM_BUF_SIZE);
    dr_thread_free(drcontext, data, sizeof(per_thread_t));
}

DR_EXPORT void dr_init(client_id_t id){
    client_id = id;
    drreg_options_t ops = {sizeof(ops), 3 /*max slots needed*/, false};

    if(!drmgr_init() || !drwrap_init() || !drutil_init() || !drx_init() || drreg_init(&ops) != DRREG_SUCCESS)
        DR_ASSERT(false);
    dr_register_exit_event(event_exit);
    //drmgr_register_pre_syscall_event(event_pre_syscall);
    tcls_idx = drmgr_register_cls_field(event_thread_context_init, event_thread_context_exit);
    tls_idx = drmgr_register_tls_field();

    if(!drmgr_register_module_load_event(event_module_load)
    || !drmgr_register_thread_init_event(event_thread_init)
    || !drmgr_register_thread_exit_event(event_thread_exit)
    || !drmgr_register_bb_instrumentation_event(NULL, event_app_instruction, NULL)
    || tcls_idx == -1
    || tls_idx == -1
    )
        DR_ASSERT(false);
    mutex = dr_mutex_create();
    if (!dr_raw_tls_calloc(&tls_seg, &tls_offs, INSTRACE_TLS_COUNT, 0))
        DR_ASSERT(false);
    if (!dr_raw_tls_calloc(&reg_pc_count, &reg_pc_offs, INSTRACE_TLS_COUNT, 0))
        DR_ASSERT(false);
    // int *count ;
    // count = dr_get_dr_segment_base(reg_pc_count);
    // *(count + reg_pc_offs) = 2; 
    // dr_fprintf(STDERR, "%d\n", *(count + reg_pc_offs));
    dr_fprintf(STDERR, "%d\n", reg_pc_offs);

    shadow_init();
}