/* Defect 2255 (panel 232's ffi-pragmatist, `xtype.h`): one static arena, two C
 * types: C gives back a `ta` and later hands out a `tb` at the same address, as a
 * library reusing its own freed block for another kind of object would. */
typedef struct ta ta;
typedef struct tb tb;
static long long arena[8];
static inline __attribute__((noinline)) ta *a_open(void) { arena[0] = 1; return (ta *)arena; }
static inline __attribute__((noinline)) void a_close(ta *x) { (void)x; arena[0] = 0; }
static inline __attribute__((noinline)) tb *b_get(void) { arena[0] = 7; return (tb *)arena; }
static inline __attribute__((noinline)) long long b_read(tb *x) { return *(long long *)x; }
