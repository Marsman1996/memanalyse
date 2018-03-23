#include "utils.h"

typedef struct _malloc_entry_t {
    app_pc start;
    //app_pc real_start;
    app_pc end;
    //alloc_type_t alloc_type;
    void *data;
} malloc_entry_t;