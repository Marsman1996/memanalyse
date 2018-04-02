#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "utils.h"

#define MAX_NUM_ERROR 65536

typedef enum {
    DOUBLE_FREE,
    OUT_BOUND_WRITE,
    OUT_BOUND_READ,
    USE_AFTER_FREE,
    NULL_POINTER_DEREFERENCE,
} error_type_t;

typedef struct {
    uint32_t pc_count;
    app_pc app_addr;
    app_pc real_esp; 
    app_pc real_ebp;
    uint32_t size;
    int write;
} error_store_t;

void error_store(app_pc app_addr, uint32_t size, app_pc real_esp, app_pc real_ebp, uint32_t pc_count, int write);
void error_output();