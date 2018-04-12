#include "analyse.h"
#include "shadow.h"
#include "alloc.h"
#include "error.h"
#include "check.h"
alloc_link_t alloc_start;

app_pc mmap1, mmap2, mmap3, mmap4;
app_pc exe_start, exe_end;

void input_module(){
    FILE *f_module;
    char line[100];
    char elf_line[100];
    char split[] = " ";

    f_module = fopen("../log/module.log", "r");
    if (f_module == NULL){
        assert(false);
    }
    char *s_module_name, *s_module_start, *s_module_end, *s_module_path;
    char *s_nr, *s_name, *s_addr, *s_flag, *s_off;
    app_pc module_start, module_w_start, module_bss_start, module_end;
    uint32_t has_writed, off, base_off;
    app_pc addr, real_addr, base_addr;
    bool is_first = true;
    while(fgets(line, 200, f_module) != NULL){
        s_module_name   = strtok(line, split);
        s_module_start  = strtok(NULL, split);
        s_module_end    = strtok(NULL, split);
        s_module_path   = strtok(NULL, split);
        module_start    = (app_pc)strtoul(s_module_start, NULL, 16);
        module_end      = (app_pc)strtoul(s_module_end, NULL, 16);

        if(is_first == true){
            exe_start = module_start;
            exe_end   = module_end;
            is_first  = false;
        }
        //为了排除libc附近的读写操作: 可能是mmap?
        if(strcmp(s_module_name, "libc.so.6") == 0){
            mmap1 = (app_pc)((uint32_t)module_start & 0xfff00000);
            mmap2 = module_start;
            mmap3 = module_end;
        }
        if(strcmp(s_module_name, "libmyclient.so") == 0){
            mmap4 = module_start;
        }
        //排除虚拟动态共享库 (virtual dynamic shared library)
        if(strcmp(s_module_path, "[vdso]\n") == 0){
            shadow_special_block_create((app_pc)module_start, (app_pc)module_end, SHADOW_DEFINED);
            continue;
        }
        char cmd[200];
        strcpy(cmd, "readelf -S ");
        strcat(cmd, s_module_path);
        //printf("%s", cmd);
        FILE *pipe;
        pipe = _popen(cmd, "r");
        fgets(elf_line, 100, pipe);//remove useless elf_line
        fgets(elf_line, 100, pipe);
        fgets(elf_line, 100, pipe);
        fgets(elf_line, 100, pipe);
        fgets(elf_line, 100, pipe);

        has_writed = 0;
        while(fgets(elf_line, 100, pipe) != NULL){
            //printf("%s\n", elf_line);
            s_nr = strtok(elf_line, split);//nr
            if(strcmp(s_nr, "[") == 0)
                s_nr = strtok(NULL, split);
            s_name = strtok(NULL, split);
            if(strcmp(s_name, "to") == 0){
                break;
            }
            strtok(NULL, split);//type
            s_addr = strtok(NULL, split);
            s_off = strtok(NULL, split);//off
            strtok(NULL, split);//size
            strtok(NULL, split);//es
            s_flag = strtok(NULL, split);
            addr = (app_pc)strtoul(s_addr, NULL, 16);
            if(strcmp(s_nr, "1]") == 0){
                base_addr = addr;
                base_off = strtoul(s_off, NULL, 16);
            }
            real_addr = addr - base_addr + base_off + module_start;
            if(strcmp(s_flag, "WA") == 0 || strcmp(s_flag, "WAT") == 0){
                if(has_writed == 0){
                    has_writed = 1;
                    module_w_start = real_addr;
                }
                if(strcmp(s_name, ".bss") == 0){
                    printf("bss here\n");
                }
            }
        }
        shadow_special_block_create((app_pc)module_start, (app_pc)module_w_start, SHADOW_DEFINED);
        shadow_write_range((app_pc)module_w_start, (app_pc)module_end, SHADOW_DEFINED);
    }

}

void input_alloc(){
    FILE *f_alloc;
    f_alloc = fopen("../log/alloc.log", "r");
    if(f_alloc == NULL){
        assert(false);
    }
    char line[100];
    char split[] = " ";
    char *s_alloc_name, *s_pc_count, *s_pc_count2, *s_addr, *s_size, *s_old_addr;
    while(fgets(line, 100, f_alloc)){
        s_alloc_name = strtok(line, split);
        s_pc_count   = strtok(NULL, split);
        s_pc_count2  = strtok(NULL, split);
        s_addr       = strtok(NULL, split);
        s_size       = strtok(NULL, split);
        s_old_addr   = strtok(NULL, split);
        
        if (strcmp(s_alloc_name, "free") == 0
        || strcmp(s_alloc_name, "malloc") == 0
        || strcmp(s_alloc_name, "realloc") == 0
        || strcmp(s_alloc_name, "calloc") == 0){
            alloc_routine_in(s_alloc_name, s_pc_count, s_pc_count2, s_addr, s_size, s_old_addr);
        }
        else{
            printf("no alloc routine matched");
            assert(false);
        }
    }
}

void input_trace(){
    FILE *f_trace;
    char line[100];
    char split[] = " ";

    f_trace = fopen("../log/instrace.log", "r");
    if(f_trace == NULL){
        assert(false);
    }
    char *s_write, *s_pc, *s_instr, *s_size, *s_addr, *s_esp, *s_content, *s_pc_count;
    int write;
    app_pc pc, addr, esp;
    uint32_t size, pc_count, content;
    alloc_link_t alloc_link;
    fgets(line, 100, f_trace);//remove top line
    while(fgets(line, 100, f_trace) != NULL){
        s_write    = strtok(line, split);
        s_pc       = strtok(NULL, split);
        s_instr    = strtok(NULL, split);
        if(strcmp(s_instr, "rep") == 0)
            s_instr = strtok(NULL, split);
        s_size     = strtok(NULL, split);
        s_addr     = strtok(NULL, split);
        s_esp      = strtok(NULL, split);
        s_content  = strtok(NULL, split);
        s_pc_count = strtok(NULL, split);
        write    = strtol(s_write, NULL, 10);
        pc       = (app_pc)strtoul(s_pc, NULL, 16);
        size     = (uint32_t)strtol(s_size, NULL, 10);
        addr     = (app_pc)strtoul(s_addr, NULL, 16);
        esp      = (app_pc)strtoul(s_esp, NULL, 16);
        content  = strtoul(s_content, NULL, 16);
        pc_count = (uint32_t)strtoul(s_pc_count, NULL, 10);
        alloc_link = alloc_check(pc_count);
        check(write, pc, s_instr, addr, size, esp, content, pc_count, alloc_link);
    }
}

int main(void){
    shadow_init();
    alloc_init();
    //------------FOR TEST-------------------------
    // app_pc start, end;
    // start = 0x20;
    // end = 0x30;
    // shadow_write_range(start, end, SHADOW_UNDEFINED);
    // print_for_test(start);
    //---------------------------------------------
    // input module
    input_module();
    // input alloc
    input_alloc();
    // input trace
    input_trace();
    // detect memory leak
    leak_detect();
    
    error_output();
    return 0;
}