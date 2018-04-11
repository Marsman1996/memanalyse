#include "error.h"

error_store_t error_list[MAX_NUM_ERROR];
int error_num = 0;

void error_store(app_pc app_addr, uint32_t size, app_pc real_esp, uint32_t content, uint32_t pc_count, int write, char *error_name){
    error_list[error_num].app_addr = app_addr;
    error_list[error_num].size = size;
    error_list[error_num].real_esp = real_esp;
    error_list[error_num].content = content;
    error_list[error_num].pc_count = pc_count;
    error_list[error_num].write = write;
    strcpy(error_list[error_num].ename, error_name);
    error_num++;
    if(error_num >= MAX_NUM_ERROR)
        assert(false);
}

void error_output(){
    FILE *error_file;
    error_file = fopen("../log/error_out.log", "w");
    if (error_file == NULL) {
        assert(false);
    }
    int i;
    for(i = 0; i < error_num; i++){
        fprintf(error_file, "%8u ", error_list[i].pc_count); 
        fprintf(error_file, "0x%08x ", (uint32_t)error_list[i].app_addr); 
        fprintf(error_file, "0x%08x ", (uint32_t)error_list[i].real_esp); 
        fprintf(error_file, "%08x ", error_list[i].content);
        fprintf(error_file, "%u ", error_list[i].write);         
        fprintf(error_file, "%u ", error_list[i].size); 
        fprintf(error_file, "%s\n", error_list[i].ename);
    }
}