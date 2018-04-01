#include "error.h"

error_store_t error_list[MAX_NUM_ERROR];
int error_num = 0;

void error_store(app_pc app_addr, uint32_t size, app_pc real_esp, app_pc real_ebp, uint32_t pc_count){
    error_list[error_num].app_addr = app_addr;
    error_list[error_num].size = size;
    error_list[error_num].real_esp = real_esp;
    error_list[error_num].real_ebp = real_ebp;
    error_list[error_num].pc_count = pc_count;    
    error_num++;
    if(error_num >= MAX_NUM_ERROR)
        assert(false);
}