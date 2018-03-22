#include "analyse.h"

int main(void){
    FILE *f;
    f = fopen("../log/instrace.log", "r");
    if(f == NULL){
        printf("unable to open trace\n");
        return -1;
    }
    char line[100];
    char split[] = " ";
    char *s_write, *s_pc, *s_instr, *s_size, *s_addr, *s_esp, *s_ebp;
    int write, pc, size, addr, esp, ebp;

    while(fgets(line, 100, f) != NULL){
        fgets(line, 100, f);
        s_write = strtok(line, split);
        s_pc    = strtok(NULL, split);
        s_instr = strtok(NULL, split);
        s_size  = strtok(NULL, split);
        s_addr  = strtok(NULL, split);
        s_esp   = strtok(NULL, split);
        s_ebp   = strtok(NULL, split);
        write = strtol(s_write, NULL, 16);
        pc = strtol(s_pc, NULL, 16);
        size = strtol(s_size, NULL, 16);
        addr = strtol(s_addr, NULL, 16);
        esp = strtol(s_esp, NULL, 16);
        ebp = strtol(s_ebp, NULL, 16);
        printf("%d\n", write);
    }
    
    return 0;
}