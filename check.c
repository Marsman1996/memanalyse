#include "check.h"
#include "shadow.h"

extern app_pc mmap1, mmap2, mmap3, mmap4;
extern app_pc exe_start, exe_end;
extern entry_link_t entry_start;
//TODO:完善check read和check write
static app_pc eip_store;
static app_pc eip_stack[MAX_STACK] = {0};
static app_pc last_esp;

void add_ptr(app_pc pc, app_pc app_addr, uint32_t content){
    if(exe_start <= pc && pc < exe_end){
        entry_link_t entry_temp;
        for(entry_temp = entry_start->next; entry_temp != NULL; entry_temp = entry_temp->next){
            
        }
    }
}

bool is_in_eip(app_pc app_addr){
    int i;
    for (i = 0; i < (int)eip_stack[0]; i++) {
        int idx = (int)eip_stack[0] - i;
        if (app_addr >= eip_stack[idx] && app_addr < eip_stack[idx] + 4)
            return true;
    }
    return false;
}

void check_read(app_pc pc, app_pc app_addr, uint32_t size, app_pc real_esp, uint32_t content, uint32_t pc_count){
    uint32_t i;
    uint32_t shadow_value;
    
    for(i = 0; i < size; i++){
        if (app_addr >= real_esp) {
            continue;
        }
        shadow_value = shadow_get_byte(app_addr + i);
        if(shadow_value == SHADOW_REDZONE){//读越界
            error_store(app_addr, size, real_esp, content, pc_count, 0, "read overflow");
            return ;
        }
        if(shadow_value == SHADOW_UNADDRESSABLE){
            error_store(app_addr, size, real_esp, content, pc_count, 0, "read unaddr");
            return ;
        }
        if(shadow_value == SHADOW_UNDEFINED){
            error_store(app_addr, size, real_esp, content, pc_count, 0, "read undefined");
            return ;
        }
    }
}

void check_write(app_pc pc, app_pc app_addr, uint32_t size, app_pc real_esp, uint32_t content, uint32_t pc_count){
    uint32_t i;
    uint32_t shadow_value;
    
    add_ptr(pc, app_addr, content);

    for(i = 0; i < size; i++){
        if (app_addr >= real_esp - 4) {
            if (is_in_eip(app_addr)){
                printf("eip corrupt %u\n", pc_count);
                error_store(app_addr, size, real_esp, content, pc_count, 1, "eip corrupt");
                return ;
            }
            else
                continue;
        }

        if (shadow_is_in_special_block(app_addr + i)){
            error_store(app_addr, size, real_esp, content, pc_count, 1, "write so");
            return ;
        }
        shadow_value = shadow_get_byte(app_addr + i);
        if (shadow_value == SHADOW_UNADDRESSABLE){
            error_store(app_addr, size, real_esp, content, pc_count, 1, "write unaddr");
            return ;
        }
        else if (shadow_value == SHADOW_REDZONE){//写越界
            error_store(app_addr, size, real_esp, content, pc_count, 1, "write overflow");
            return ;
        }
        else if (shadow_value == SHADOW_UNDEFINED){
            shadow_block_write_byte(app_addr + i, SHADOW_DEFINED);
        }
    }
}

void check(uint32_t write, app_pc pc, const char *instr, app_pc app_addr, uint32_t size, app_pc esp, uint32_t content, uint32_t pc_count, alloc_link_t alloc_link){
    //TODO:暂时排除mmap(maybe)区域读写

    //排除alloc操作head读写
    bool j_mark = false;
    if((app_addr < mmap2 && app_addr >= mmap1) 
    || (app_addr < mmap4 && app_addr >= mmap3))
        j_mark = true;
    if(alloc_link != NULL 
    && alloc_link->routine.is_load == true 
    && alloc_link->routine.entry_link != NULL){
        if(strcmp(alloc_link->routine.name, "realloc") == 0){
            uint32_t aligned_size;
            aligned_size = ((alloc_link->routine.size + 2 * REDZONE_SIZE + 4) < 16) ? 16 : ALIGN_FORWARD((uint32_t)(alloc_link->routine.size + 2 * REDZONE_SIZE + 4), 8);

            if((app_addr < 
            alloc_link->routine.entry_link->entry.aligned_end
            &&
            app_addr >=
            alloc_link->routine.entry_link->entry.aligned_start)
            ||
            (app_addr <
            alloc_link->routine.addr - REDZONE_SIZE + aligned_size
            &&
            app_addr >=
            alloc_link->routine.addr - REDZONE_SIZE - 4 )
            ){
                j_mark = true;
            }
        }
        else{
            if(app_addr < 
            alloc_link->routine.entry_link->entry.aligned_end
            &&
            app_addr >=
            alloc_link->routine.entry_link->entry.aligned_start){
                j_mark = true;
            }
        }
    }
    if(alloc_link != NULL && alloc_link->routine.entry_link == NULL){
        if(app_addr < 
            alloc_link->routine.addr - REDZONE_SIZE + 16
            &&
            app_addr >=
            alloc_link->routine.addr - REDZONE_SIZE - 4){
                j_mark = true;
            }
    }
    //check
    if(j_mark != true){
        if (write == 1)
            check_write(pc, app_addr, size, esp, content, pc_count);
        else if (write == 0)
            check_read(pc, app_addr, size, esp, content, pc_count);
        else
            assert(false);
    }
    //handle eip
    if (strcmp(instr, "call") == 0 && write == 1) {
        eip_stack[0]++;
        eip_stack[(int)eip_stack[0]] = app_addr;
    }
    if (strcmp(instr, "ret") == 0 && app_addr == eip_stack[(int)eip_stack[0]]) {
        eip_stack[0]--;
    }
    //记录esp
    last_esp = esp;
}