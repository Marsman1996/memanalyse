#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int global_value;

int main(void){
    printf("%d\n", global_value);
    int i;
    char *ptr1 = (char *)malloc(9 * sizeof(char));
    strcpy(ptr1, "good data");
    printf("%.*s\n", 10, ptr1);
    free(ptr1);
    char *ptr2 = (char *)malloc(9 * sizeof(char));
    strcpy(ptr2, "bad data");
    printf("%.*s\n", 9, ptr1);
    char num[3];
    strcpy(num,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
}

// int main(){
//     char *ptr;
//     int i;
//     i = 10;
//     ptr = (char *)malloc(i);
//     printf("TEST: 0x%08x\n", ptr);
//     //memset(ptr + 20, 0, 10);

//     ptr[0] = 'a';
//     ptr[1] = 'b';
//     ptr[2] = 'c';
//     ptr[3] = 'd';
//     ptr[4] = 'e';
    
//     char test;
//     test = ptr[0];
//     printf("%c\n", ptr[0]);
//     ptr[0] = ptr[4];
//     printf("%c\n", ptr[0]);

//     int *arr; arr = (int *)ptr;
//     arr[0] = 2;
//     arr[0] = arr[0] + 4;

//     free(ptr);

//     return 0;
// }
