#include "check.h"
#include "shadow.h"
#include "error.h"

extern app_pc mmap_start, mmap_end;
extern app_pc exe_start, exe_bss, exe_end;
extern app_pc heapmem_start, heapmem_end;
extern entry_link_t entry_start;
extern entry_link_t junk_start;
//TODO:完善check read和check write
static app_pc eip_store;
static app_pc eip_stack[MAX_STACK] = {0};
static app_pc last_esp;

void remove_ptr(app_pc app_addr){
    entry_link_t entry_temp;
    uint32_t i, j;

    for(entry_temp = entry_start->next; entry_temp != NULL; entry_temp = entry_temp->next){
        for(i = 1; i <= (uint32_t)(entry_temp->entry.ptr[0]); i++){
            if(app_addr == entry_temp->entry.ptr[i]){
                for(j = i; j < (uint32_t)(entry_temp->entry.ptr[0]); j++){
                    entry_temp->entry.ptr[j] = entry_temp->entry.ptr[j + 1];
                }
                entry_temp->entry.ptr[0]--;
            }
        }
    }

    for(entry_temp = junk_start->next; entry_temp != NULL; entry_temp = entry_temp->next){
        for(i = 1; i <= (uint32_t)(entry_temp->entry.ptr[0]); i++){
            if(app_addr == entry_temp->entry.ptr[i]){
                for(j = i; j < (uint32_t)(entry_temp->entry.ptr[0]); j++){
                    entry_temp->entry.ptr[j] = entry_temp->entry.ptr[j + 1];
                }
                entry_temp->entry.ptr[0]--;
            }
        }
    }
}

void add_ptr(app_pc pc, app_pc app_addr, uint32_t content){
    if(exe_start <= pc && pc < exe_end){
        entry_link_t entry_temp;
        for(entry_temp = entry_start->next; entry_temp != NULL; entry_temp = entry_temp->next){
            if(content == (uint32_t)entry_temp->entry.start){
                entry_temp->entry.ptr[++(uint32_t)(entry_temp->entry.ptr[0])] = app_addr;
            }
        }
    }
}

void check_ptr(app_pc app_addr, uint32_t size, app_pc esp, uint32_t content, uint32_t pc_count){
    entry_link_t entry_temp;
    uint32_t i;
    
    for(entry_temp = junk_start->next; entry_temp != NULL; entry_temp = entry_temp->next){
        for(i = 1; i <= (uint32_t)(entry_temp->entry.ptr[0]); i++){
            if(app_addr == entry_temp->entry.ptr[i]){
                error_store(app_addr, size, esp, content, pc_count, 0, "use after free");
            }
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
    uint32_t e_mark = 0;

    check_ptr(app_addr, size, real_esp, content, pc_count);
    
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
            if(exe_bss <= app_addr && app_addr < exe_end){
                error_store(app_addr, size, real_esp, content, pc_count, 0, "undef global val");
                return ;
            }
            //error_store(app_addr, size, real_esp, content, pc_count, 0, "read undefined");
            //return ;
            continue;
        }
    }
}

void check_write(app_pc pc, app_pc app_addr, uint32_t size, app_pc real_esp, uint32_t content, uint32_t pc_count){
    uint32_t i;
    uint32_t shadow_value;
    uint32_t e_mark = 0x0;
    
    remove_ptr(app_addr);
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
    bool j_mark = false;
    //TODO:暂时排除mmap(maybe)区域读写
    //TODO: 只能排除库函数部分的检查
    if(mmap_start <= app_addr && app_addr < mmap_end)
        j_mark = true;

    //TODO: 在内存分配函数时跳过所有检查
    //排除alloc操作head读写
    if(alloc_link != NULL 
    && alloc_link->routine.is_load == true){
        if(heapmem_start <= app_addr && app_addr < heapmem_end)
            j_mark = true;
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
    if ((strcmp(instr, "call") == 0 || (strcmp(instr, "push") == 0 && content == (uint32_t)app_addr + 5))  && write == 1) {
        eip_stack[0]++;
        eip_stack[(int)eip_stack[0]] = app_addr;
    }
    if ((strcmp(instr, "ret") == 0 || strcmp(instr, "pop") == 0) && app_addr == eip_stack[(int)eip_stack[0]]) {
        eip_stack[0]--;
    }
    //记录esp
    last_esp = esp;
}