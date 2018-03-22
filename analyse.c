#include <stdio.h>

int main(void){
    FILE *f;
    f = fopen("instrace.log", "r");
    if(f == NULL){
        printf("unable to open trace\n");
        return -1;
    }
    char line[100];
    char split[] = " ";
    char *s_write;
    char *s_pc; 
    char *s_instr; 
    char *s_size; 
    char *s_addr; 
    char *s_esp; 
    char *s_ebp;
    int i = 0;
    //while(fgets(line, 100, f) != NULL){
        // printf("%s", line);
        // i++;
        fgets(line, 100, f);
        s_write = strtok(line, split);
        s_pc    = strtok(NULL, split);
        s_instr = strtok(NULL, split);
        s_size  = strtok(NULL, split);
        s_addr  = strtok(NULL, split);
        s_esp   = strtok(NULL, split);
        printf("hello\n");
        //s_ebp   = strtok(NULL, " ");
        printf("%s\n", s_write);
    //}
    // printf("%d\n", i);
    
    return 0;
}