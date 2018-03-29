#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 注意前闭后开
#define LIBC_SHIFT 0x1ab1c8
#define LIBC_DATA_SHIFT_START 0x1ad040
#define LIBC_DATA_SHIFT_END   0x1adec0
#define LIBC_BSS_SHIFT_START 0x1adec0
//#define LIBC_BSS_SHIFT_END 就是结尾了
#define OUT 0x8049f08
#define OUT_DATA_START 0x804a020
#define OUT_DATA_END   0x804a028
#define OUT_BSS_START 0x804a028
//#define OUT_BSS_END 就是结尾了