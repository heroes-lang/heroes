/* Defect 2255: as `fixedbugs-2255-another-type.h`, the `tb` handed out through
 * an out-parameter. */
typedef struct ta ta;
typedef struct tb tb;
static long long arena[8];
static inline __attribute__((noinline)) ta *a_open(void) { arena[0] = 1; return (ta *)arena; }
static inline __attribute__((noinline)) void a_close(ta *x) { (void)x; arena[0] = 0; }
static inline __attribute__((noinline)) void b_into(tb **out) { arena[0] = 7; *out = (tb *)arena; }
static inline __attribute__((noinline)) long long b_read(tb *x) { return *(long long *)x; }
