#include "alloc.h"
#include "shadow.h"

// alloc_ptr always points to last
extern alloc_link_t alloc_start;
alloc_link_t alloc_ptr;
entry_link_t entry_start;

void alloc_init(){
    alloc_start = (alloc_link_t)malloc(sizeof(alloc_node_t));
    alloc_start->next = NULL;
    alloc_ptr = alloc_start;

    entry_start = (entry_link_t)malloc(sizeof(entry_node_t));
    entry_start->key = NULL;
    entry_start->next = NULL;
}

void alloc_routine_in(char *s_alloc_name, char *s_pc_count, char *s_pc_count2, char *s_addr, char *s_size, char *s_old_addr){
    alloc_link_t alloc_temp;
    uint32_t pc_count, pc_count2, size; 
    app_pc addr, old_addr;

    alloc_temp = (alloc_link_t)malloc(sizeof(alloc_node_t));
    alloc_temp->next = NULL;
    strcpy(alloc_temp->routine.name, s_alloc_name);

    alloc_temp->routine.is_load = false;

    pc_count = strtoll(s_pc_count, NULL, 10);
    alloc_temp->routine.pc_count = pc_count;
    
    pc_count2 = strtoll(s_pc_count2, NULL, 10);
    alloc_temp->routine.pc_count2 = pc_count2;    

    addr = (app_pc)strtoll(s_addr, NULL, 16);
    alloc_temp->routine.addr = addr;
    
    if(s_size != NULL){
        size = strtol(s_size, NULL, 10);
        alloc_temp->routine.size = size;
    }
    if(s_old_addr != NULL){
        old_addr = (app_pc)strtoll(s_old_addr, NULL, 16);
        alloc_temp->routine.old_addr = old_addr;
    }

    alloc_ptr->next = alloc_temp;
    alloc_ptr = alloc_temp;
}

void malloc_entry_add(app_pc start, app_pc end){
    //alloc_type用于区分 malloc, new, new[]
    entry_link_t e = (entry_link_t)malloc(sizeof(entry_node_t));

    e->key = start;
    e->entry.start = start;
    e->entry.end = end;
    e->next = NULL;
    //e->alloc_type = alloc_type;
    //e->data = NULL;
    entry_link_t entry_temp;
    for(entry_temp = entry_start; entry_temp->next != NULL; entry_temp = entry_temp->next)
        ;//找到末尾
    entry_temp->next = e;
}

bool entry_remove(app_pc key){
    entry_link_t entry_temp, entry_last;
    for(entry_temp = entry_start->next, entry_last = entry_start; 
    entry_temp != NULL; 
    entry_temp = entry_temp->next, entry_last = entry_last->next){
        if(entry_temp->key == key){
            entry_last->next = entry_temp->next;
            free(entry_temp);
            return true;
        }
    }
    return false;
}

entry_link_t entry_lookup(app_pc key){
    entry_link_t entry_temp;
    for(entry_temp = entry_start->next; entry_temp != NULL; entry_temp = entry_temp->next){
        if(entry_temp->key == key)
            return entry_temp;
    }
    return NULL;
}

void handle_free_post(alloc_routine_t *routine){
    entry_link_t e;
    e = entry_lookup(routine->addr);
    if(e == NULL){
        assert(false);
    }
    shadow_write_range(e->entry.start - REDZONE_SIZE, e->entry.end + REDZONE_SIZE, SHADOW_UNADDRESSABLE);
    entry_remove(e->entry.start);
}

void handle_malloc_post(alloc_routine_t *routine){
    malloc_entry_add(routine->addr, routine->addr + routine->size);
    shadow_write_range(routine->addr, routine->addr + routine->size, SHADOW_UNDEFINED);
    shadow_block_add_redzone(routine->addr, routine->size);
}

void handle_realloc_post(alloc_routine_t *routine){
    //add routine->old_addr = NULL condition, 相当于malloc
    if(routine->old_addr == NULL){
        handle_malloc_post(routine);
        return ;
    }
    entry_link_t e;
    e = entry_lookup(routine->old_addr);
    if(e == NULL){
        assert(false);
    }
    uint32_t old_size = e->entry.end - e->entry.start;
    //写new区域
    if(routine->size > old_size){//new区域大
        if(routine->addr != routine->old_addr)//新旧分配内存区域起始点不同
            shadow_copy_range(routine->old_addr, routine->addr, old_size);
        shadow_write_range(routine->addr + old_size, routine->addr + routine->size, SHADOW_UNDEFINED);
    }
    else {//old区域大
        if(routine->addr != routine->old_addr)
            shadow_copy_range(routine->old_addr, routine->addr, routine->size);
    }
    //消除old区域
    if(routine->addr > routine->old_addr){//比较start, 若new在old后,
        shadow_write_range(routine->old_addr, (app_pc)((routine->addr < routine->old_addr + old_size) ? routine->addr : e->entry.end), SHADOW_UNADDRESSABLE);
    }
    if(routine->addr + routine->size < e->entry.end){//比较end 若new在old前
        shadow_write_range((app_pc)((routine->addr + routine->size > routine->old_addr) ? routine->addr + routine->size : routine->old_addr), e->entry.end, SHADOW_UNADDRESSABLE);
    }
    //加redzone
    shadow_block_add_redzone(routine->addr, routine->size);
    //添加新hash
    malloc_entry_add(routine->addr, routine->addr + routine->size);
    //销毁旧hash
    entry_remove(e->entry.start);
}

void handle_calloc_post(alloc_routine_t *routine){
    malloc_entry_add(routine->addr, routine->addr + routine->size);
    shadow_write_range(routine->addr, routine->addr + routine->size, SHADOW_DEFINED);
    shadow_block_add_redzone(routine->addr, routine->size);
}

void handle_free_pre(alloc_routine_t *routine){

}

void handle_malloc_pre(alloc_routine_t *routine){

}

void handle_realloc_pre(alloc_routine_t *routine){

}

void handle_calloc_pre(alloc_routine_t *routine){

}

void alloc_check(uint32_t pc_count){
    alloc_link_t alloc_temp;
    alloc_temp = alloc_start->next;
    if(alloc_temp == NULL 
    || alloc_temp->routine.pc_count != pc_count 
    || alloc_temp->routine.pc_count2 != pc_count)
        return ;
    if(alloc_temp->routine.is_load == false){
        if (strcmp(alloc_temp->routine.name, "free") == 0){
            handle_free_pre(&(alloc_temp->routine));
        }
        else if (strcmp(alloc_temp->routine.name, "malloc") == 0){
            handle_malloc_pre(&(alloc_temp->routine));
        }
        else if (strcmp(alloc_temp->routine.name, "realloc") == 0){
            handle_realloc_pre(&(alloc_temp->routine));
        }
        else if (strcmp(alloc_temp->routine.name, "calloc") == 0){
            handle_calloc_pre(&(alloc_temp->routine));
        }
        else{
            assert(false);
        }
        alloc_temp->routine.is_load = true;
        return ;
    }
    if(alloc_temp->routine.is_load == true){
        if (strcmp(alloc_temp->routine.name, "free") == 0){
            handle_free_post(&(alloc_temp->routine));
        }
        else if (strcmp(alloc_temp->routine.name, "malloc") == 0){
            handle_malloc_post(&(alloc_temp->routine));
        }
        else if (strcmp(alloc_temp->routine.name, "realloc") == 0){
            handle_realloc_post(&(alloc_temp->routine));
        }
        else if (strcmp(alloc_temp->routine.name, "calloc") == 0){
            handle_calloc_post(&(alloc_temp->routine));
        }
        else{
            assert(false);
        }
        print_for_test(alloc_temp->routine.addr - REDZONE_SIZE);
        //destroy alloc_node
        alloc_start->next = alloc_temp->next;
        free(alloc_temp);
        return ;
    }

}