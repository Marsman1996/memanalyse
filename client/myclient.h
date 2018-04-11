#include "dr_api.h"
#include "drmgr.h"
#include "drutil.h"
#include "drreg.h"
#include "drx.h"
#include "drwrap.h"
#include "hashtable.h"
#include <stddef.h> /* for offsetof */
#include <stdint.h>
#include <memory.h>
//#include "drreg.h"

#define APP_BLOCK_BITS 16
//64KB
#define APP_BLOCK_SIZE (1 << APP_BLOCK_BITS)
//1 byte
#define REDZONE_SIZE 1

//65536
#define SHADOW_TABLE_ENTRIES (1 << (32 - APP_BLOCK_BITS))
#define MAX_NUM_SPECIAL_BLOCKS 8
#define SHADOW_GRANULARITY 4

#define SHADOW_UNADDRESSABLE    0x1
#define SHADOW_UNDEFINED        0x3
#define SHADOW_DEFINED          0x0
#define SHADOW_REDZONE          0x2

#define SHADOW_DWORD_UNADDRESSABLE 0x55
#define SHADOW_DWORD_UNDEFINED     0xff
#define SHADOW_DWORD_DEFINED       0x00
#define SHADOW_DWORD_REDZONE       0xaa

#define SHADOW_TABLE_INDEX(addr) ((uint32_t)(addr) >> APP_BLOCK_BITS)

#define MAX_NUM_ERROR 1024

#define ALIGN_BACKWARD(x, alignment) \
        (((ptr_uint_t)x) & (~((ptr_uint_t)(alignment)-1)))
#define ALIGN_FORWARD(x, alignment) \
        ((((ptr_uint_t)x) + (((ptr_uint_t)alignment)-1)) & \
         (~(((ptr_uint_t)alignment)-1)))

#define POINTER_OVERFLOW_ON_ADD(ptr, add) \
    (((ptr_uint_t)(ptr)) + (add) < ((ptr_uint_t)(ptr)))

#define APP_RANGE_LOOP(app_addr, app_size, app_blk_base, app_blk_end, app_src_end,\
                       start, end, iter_size, loop_body) do {                     \
    app_src_end = app_addr + (app_size - 1); /* closed, to avoid overflow */      \
    for (app_blk_base = (app_pc) ALIGN_BACKWARD(app_addr, shadow_map->app_block_size);   \
         /* While the end is closed, we need to detect the loop end, so we rely   \
          * on the closed end never being aligned and thus allowing a < here      \
          * and an overflow check on the increment.                               \
          */                                                                      \
         app_blk_base < app_src_end;                                              \
         app_blk_base = POINTER_OVERFLOW_ON_ADD(app_blk_end, 1) ?                 \
             app_src_end : app_blk_end + 1) {                                     \
        app_blk_end = app_blk_base + (shadow_map->app_block_size - 1);                   \
        start       = app_addr > app_blk_base ? app_addr : app_blk_base;          \
        end         = app_src_end  < app_blk_end  ? app_src_end  : app_blk_end;   \
        iter_size   = (end - start + 1);                                          \
        loop_body                                                                 \
    }                                                                             \
} while (0);

#define MINSERT instrlist_meta_preinsert


typedef enum {
    /* For Linux and for Cygwin, and for any other allocator connected via
     * a to-be-implemented API (PR 406756)
     */
    /* Typically only one of these size routines is provided */
    HEAP_ROUTINE_SIZE_USABLE,
    HEAP_ROUTINE_SIZE_REQUESTED,
    HEAP_ROUTINE_MALLOC,
    HEAP_ROUTINE_REALLOC,
    HEAP_ROUTINE_FREE,
    /* BSD libc calloc simply calls malloc and then zeroes out
     * the resulting memory: thus, nothing special for us to watch.
     * But glibc calloc does its own allocating.
     */
    HEAP_ROUTINE_CALLOC,
    HEAP_ROUTINE_POSIX_MEMALIGN,
    HEAP_ROUTINE_MEMALIGN,
    HEAP_ROUTINE_VALLOC,
    HEAP_ROUTINE_PVALLOC,
    /* On Windows, we must watch debug operator delete b/c it reads
     * malloc's headers (i#26).  On both platforms we want to watch
     * the operators to find mismatches (i#123).
     */
    HEAP_ROUTINE_NEW,
    HEAP_ROUTINE_NEW_ARRAY,
    HEAP_ROUTINE_DELETE,
    HEAP_ROUTINE_DELETE_ARRAY,
    /* Malloc replacement needs to distinguish these */
    HEAP_ROUTINE_NEW_NOTHROW,
    HEAP_ROUTINE_NEW_ARRAY_NOTHROW,
    HEAP_ROUTINE_DELETE_NOTHROW,
    HEAP_ROUTINE_DELETE_ARRAY_NOTHROW,
    /* Group label for routines that might read heap headers but
     * need no explicit argument modification
     */
    HEAP_ROUTINE_STATS,
    /* Group label for un-handled routine */
    HEAP_ROUTINE_NOT_HANDLED,
    /* Should collapse these two once have aligned-malloc routine support */
    HEAP_ROUTINE_NOT_HANDLED_NOTIFY,

    HEAP_ROUTINE_LAST = HEAP_ROUTINE_NOT_HANDLED_NOTIFY,

    HEAP_ROUTINE_COUNT,
    HEAP_ROUTINE_INVALID,
} routine_type_t;

typedef struct _possible_alloc_routine_t {
    const char *name;
    routine_type_t type;
} possible_alloc_routine_t;

static const possible_alloc_routine_t possible_libc_routines[] = {
    { "malloc_usable_size", HEAP_ROUTINE_SIZE_USABLE},
    { "malloc",             HEAP_ROUTINE_MALLOC},
    { "realloc",            HEAP_ROUTINE_REALLOC},
    { "free",               HEAP_ROUTINE_FREE},
    { "calloc",             HEAP_ROUTINE_CALLOC},
    /* for cfree we ignore 2 extra args if there are any, as glibc itself does */
    { "cfree",              HEAP_ROUTINE_FREE },
    //int posix_memalign (void **memptr, size_t alignment, size_t size);
    { "posix_memalign",     HEAP_ROUTINE_POSIX_MEMALIGN},
    //void * memalign (size_t boundary, size_t size) 
    { "memalign",           HEAP_ROUTINE_MEMALIGN},
    //void * valloc (size_t size) 
    { "valloc",             HEAP_ROUTINE_VALLOC},
    { "pvalloc",            HEAP_ROUTINE_PVALLOC },
    /* We do not change args or return val for these: we simply allow
     * them to access heap headers.  Returned stats will be inflated
     * by redzones: oh well.
     * XXX i#94: add -replace_malloc support for these.
     */
    { "mallopt",              HEAP_ROUTINE_STATS },
    { "mallinfo",             HEAP_ROUTINE_STATS },
    { "malloc_stats",         HEAP_ROUTINE_STATS },
    { "malloc_trim",          HEAP_ROUTINE_STATS },
    { "malloc_get_state",     HEAP_ROUTINE_STATS },
    /* XXX i#94: not supported yet */
    { "malloc_set_state",     HEAP_ROUTINE_NOT_HANDLED },
    { "independent_calloc",   HEAP_ROUTINE_NOT_HANDLED },
    { "independent_comalloc", HEAP_ROUTINE_NOT_HANDLED },
    /* i#267: support tcmalloc, though not yet on Windows (b/c the late
     * injection there requires heap walking which is not easy for tcmalloc).
     */
    { "tc_malloc_size",    HEAP_ROUTINE_SIZE_USABLE },
    { "tc_malloc",         HEAP_ROUTINE_MALLOC },
    { "tc_realloc",        HEAP_ROUTINE_REALLOC },
    { "tc_free",           HEAP_ROUTINE_FREE },
    { "tc_calloc",         HEAP_ROUTINE_CALLOC },
    { "tc_cfree",          HEAP_ROUTINE_FREE },
    { "tc_posix_memalign", HEAP_ROUTINE_POSIX_MEMALIGN },
    { "tc_memalign",       HEAP_ROUTINE_MEMALIGN },
    { "tc_valloc",         HEAP_ROUTINE_VALLOC },
    { "tc_mallopt",        HEAP_ROUTINE_STATS },
    { "tc_mallinfo",       HEAP_ROUTINE_STATS },
    /* TCMallocGuard::TCMallocGuard() static init calls internal routines directly
     * (requires syms, but w/o we'll fail brk and tcmalloc will just use mmap).
     */
    { "(anonymous namespace)::do_malloc", HEAP_ROUTINE_MALLOC },
    { "(anonymous namespace)::do_memalign", HEAP_ROUTINE_MEMALIGN },
    /* We ignore the callback arg */
    { "(anonymous namespace)::do_free_with_callback", HEAP_ROUTINE_FREE },
    /* i#1740: ld.so uses __libc_memalign.  We include the rest for
     * completeness.
     */
    { "__libc_malloc",   HEAP_ROUTINE_MALLOC},
    { "__libc_realloc",  HEAP_ROUTINE_REALLOC},
    { "__libc_free",     HEAP_ROUTINE_FREE},
    { "__libc_calloc",   HEAP_ROUTINE_CALLOC},
    { "__libc_memalign", HEAP_ROUTINE_MEMALIGN },
    { "__libc_valloc",   HEAP_ROUTINE_VALLOC },
    { "__libc_pvalloc",  HEAP_ROUTINE_PVALLOC },
    { "__libc_mallopt",  HEAP_ROUTINE_STATS },
    { "__libc_mallinfo", HEAP_ROUTINE_STATS },
};

#define POSSIBLE_LIBC_ROUTINE_NUM \
    (sizeof(possible_libc_routines)/sizeof(possible_libc_routines[0]))

typedef enum {
    ALLOC_TYPE_MALLOC,
    ALLOC_TYPE_NEW,
    ALLOC_TYPE_NEW_ARRAY,
} alloc_type_t;

//Shadow Mem region结构
typedef struct {
    app_pc start;
    size_t size;
    uint* bitmap;
} region_t;

typedef struct {
    byte *start;
    byte *end;
    uint32_t value;
    size_t value_size;
} sp_blk_t;

typedef struct {
    uint32_t shift;
    uint32_t app_block_size;
    uint32_t shadow_block_size;
    int *shadow_table;
    uint32_t shadow_block_alloc_size;
    
    uint32_t redzone_size;
    uint32_t redzone_value;

    uint32_t default_value;
    sp_blk_t default_block;

    uint32_t special_block_num;
    sp_blk_t special_blocks[MAX_NUM_SPECIAL_BLOCKS];
} map_t;

typedef uint* bitmap_t;

typedef struct _malloc_entry_t {
    app_pc start;
    //app_pc real_start;
    app_pc end;
    alloc_type_t alloc_type;
    void *data;
} malloc_entry_t;

typedef struct {
    uint32_t size;
    app_pc ptr;
    bool lock;
    bool skip;
    malloc_entry_t *e_link;
} arg_t;

//--------------------------------------------------------------------------------------------
typedef struct _mem_ref_t {
    ushort size; /* mem ref size */
    ushort type; /* instr opcode */
    ushort write;
    uint32_t pc_count;
    app_pc eip;
    app_pc addr; /* mem ref addr */
    app_pc esp;
} mem_ref_t;

#define MAX_NUM_MEM_REFS 4096
#define MEM_BUF_SIZE (sizeof(mem_ref_t) * MAX_NUM_MEM_REFS)
#define WRT_BUF_SIZE (MAX_NUM_MEM_REFS * 32)

typedef struct {
    file_t     log;
    FILE      *logf;
    reg_id_t   reg_addr;
} per_thread_t;

static void  *mutex;    /* for multithread support */
static uint64 num_refs; /* keep a global instruction reference count */

/* Allocated TLS slot offsets */
enum {
    INSTRACE_TLS_OFFS_BUF_PTR,
    INSTRACE_TLS_COUNT, /* total number of TLS slots allocated */
};
static reg_id_t reg_pc_count;
static uint     reg_pc_offs;
static int      reg_pc_idx;
#define TLS_SLOT(tls_base, enum_val) (void **)((byte *)(tls_base)+tls_offs+(enum_val))
#define BUF_PTR(tls_base) *(ins_ref_t **)TLS_SLOT(tls_base, INSTRACE_TLS_OFFS_BUF_PTR)

//----------------------------------------------------------------------------------------------

typedef enum {
    DOUBLE_FREE,
    OUT_BOUND_WRITE,
    OUT_BOUND_READ,
    USE_AFTER_FREE,
    NULL_POINTER_DEREFERENCE,
} error_type_t;

static void
event_thread_context_init(void *drcontext, bool new_depth);

static void
event_thread_context_exit(void *drcontext, bool thread_exit);

static void event_module_load(void *drcontext, const module_data_t *info, bool loaded);

void shadow_block_add_redzone(app_pc app_addr, uint32_t app_size);

byte* shadow_special_block_create(app_pc app_start, app_pc app_end, uint32_t val);

byte* shadow_block_create();

void shadow_block_replace(app_pc app_block_base);

uint32_t shadow_get_byte(app_pc addr);

void shadow_copy_range(app_pc old_start, app_pc new_start, uint32_t size);

void shadow_write_range(app_pc start, app_pc end, uint val);

void shadow_block_write_range(app_pc app_addr, uint32_t app_size, uint32_t val);

void shadow_block_write_byte(app_pc app_addr, uint32_t val);

void malloc_entry_add(app_pc start, app_pc end, alloc_type_t alloc_type);

void shadow_init();

uint32_t shadow_is_in_special_block(byte *app_addr);

bool shadow_is_in_default_block(byte *shadow_addr);

app_pc shadow_table_app_to_shadow(uint32_t addr);

uint shadow_value_byte_to_dword(uint32_t val);

static dr_emit_flags_t event_app_analysis(void *drcontext, void *tag, instrlist_t *bb, bool for_trace, bool translating, void **user_data);

static dr_emit_flags_t
event_app_instruction(  void *drcontext, void *tag, instrlist_t *bb, 
                        instr_t *instr, bool for_trace, 
                        bool translating, void *user_data);

static dr_emit_flags_t
event_bb_app2app(void *drcontext, void *tag, instrlist_t *bb, bool for_trace, bool translating);

static void event_thread_init(void *drcontext);

static void event_thread_exit(void *drcontext);

int tolower(int c){
    if (c >= 'A' && c <= 'Z')
        return (c - ('A' - 'a'));
    return c;
}

bool
text_matches_pattern(const char *text, const char *pattern, bool ignore_case){
    /* Match text with pattern and return the result.
     * The pattern may contain '*' and '?' wildcards.
     */
    const char *cur_text = text,
               *text_last_asterisk = NULL,
               *pattern_last_asterisk = NULL;
    char cmp_cur, cmp_pat;
    while (*cur_text != '\0') {
        cmp_cur = *cur_text;
        cmp_pat = *pattern;
        if (ignore_case) {
            /* XXX DRi#943: toupper is better, for int18n, and we need to call
             * islower() first to be safe for all tolower() implementations.
             * Even better would be switching to our own locale-independent case
             * folding.
             */
            cmp_cur = (char) tolower(cmp_cur);
            cmp_pat = (char) tolower(cmp_pat);
        }
        if (*pattern == '*') {
            while (*++pattern == '*') {
                /* Skip consecutive '*'s */
            }
            if (*pattern == '\0') {
                /* the pattern ends with a series of '*' */

                return true;
            }
            text_last_asterisk = cur_text;
            pattern_last_asterisk = pattern;
        } else if ((cmp_cur == cmp_pat) || (*pattern == '?')) {
            ++cur_text;
            ++pattern;
        } else if (text_last_asterisk != NULL) {
            /* No match. But we have seen at least one '*', so go back
             * and try at the next position.
             */
            pattern = pattern_last_asterisk;
            cur_text = text_last_asterisk++;
        } else {

            return false;
        }
    }
    while (*pattern == '*')
        ++pattern;

    return *pattern == '\0';
}

// bool text_match(const char *text, const char *pattern){
//     int i;
//     for(i = 0; pattern[i] != "*"; i++){
//         if(text[i] == '\0' || text[i] != pattern[i])
//             return false;
//     }
//     return true;
// }

void print_special_block(map_t *shadow_map){
    int i;
    dr_fprintf(STDERR, "--------------- \n");
    for(i = 0; i < shadow_map->special_block_num; i++){
        dr_fprintf(STDERR, "%d: special block\n", i + 1);        
        dr_fprintf(STDERR, "start is 0x%08x \n", shadow_map->special_blocks[i].start);
        dr_fprintf(STDERR, "end is 0x%08x \n", shadow_map->special_blocks[i].end);
    }
    dr_fprintf(STDERR, "--------------- \n");
}

void print_for_test(app_pc ptr){
    int i, j;
    //ptr--;
    dr_fprintf(STDERR, "--------------- \n");
    dr_fprintf(STDERR, "shadow is \n");
    ptr = ptr - 16;
    for(j = 0; j < 5; j ++){
        for(i = 0; i < 16; i++, ptr++)
            dr_fprintf(STDERR, "%x", shadow_get_byte((app_pc)ptr));
        dr_fprintf(STDERR, "\n");    
    }
    dr_fprintf(STDERR, "--------------- \n");
}

#define BUFFER_SIZE_BYTES(buf)      sizeof(buf)
#define BUFFER_SIZE_ELEMENTS(buf)   (BUFFER_SIZE_BYTES(buf) / sizeof((buf)[0]))
#define BUFFER_LAST_ELEMENT(buf)    (buf)[BUFFER_SIZE_ELEMENTS(buf) - 1]
#define NULL_TERMINATE_BUFFER(buf)  BUFFER_LAST_ELEMENT(buf) = 0
#define DISPLAY_STRING(msg) dr_printf("%s\n", msg);

file_t
log_file_open(client_id_t id, void *drcontext,
              const char *path, const char *name, uint flags)
{
    file_t log;
    char log_dir[MAXIMUM_PATH];
    char buf[MAXIMUM_PATH];
    size_t len;
    char *dirsep;

    DR_ASSERT(name != NULL);
    len = dr_snprintf(log_dir, BUFFER_SIZE_ELEMENTS(log_dir), "%s",
                      path == NULL ? dr_get_client_path(id) : path);
    DR_ASSERT(len > 0);
    NULL_TERMINATE_BUFFER(log_dir);
    dirsep = log_dir + len - 1;
    if (path == NULL /* removing client lib */ ||
        /* path does not have a trailing / and is too large to add it */
        (*dirsep != '/'  &&
         len == BUFFER_SIZE_ELEMENTS(log_dir) - 1)) {
        for (dirsep = log_dir + len;
             *dirsep != '/' ;
             dirsep--)
            DR_ASSERT(dirsep > log_dir);
    }
    /* remove trailing / if necessary */
    if (*dirsep == '/')
        *dirsep = 0;
    else if (sizeof(log_dir) > (dirsep + 1 - log_dir)/sizeof(log_dir[0]))
        *(dirsep + 1) = 0;
    NULL_TERMINATE_BUFFER(log_dir);
    /* we do not need call drx_init before using drx_open_unique_appid_file */
    log = drx_open_unique_appid_file(log_dir, dr_get_process_id(),
                                     name, "log", flags,
                                     buf, BUFFER_SIZE_ELEMENTS(buf));
    if (log != INVALID_FILE) {
        char msg[MAXIMUM_PATH];
        len = dr_snprintf(msg, BUFFER_SIZE_ELEMENTS(msg), "Data file %s created", buf);
        DR_ASSERT(len > 0);
        NULL_TERMINATE_BUFFER(msg);
        dr_log(drcontext, DR_LOG_ALL, 1, "%s", msg);
#ifdef SHOW_RESULTS
        DISPLAY_STRING(msg);
#endif /* SHOW_RESULTS */
    }
    return log;
}

void
log_stream_close(FILE *f)
{
    fclose(f); /* closes underlying fd too for all platforms */
}

FILE *
log_stream_from_file(file_t f)
{
    return fdopen(f, "w");
}