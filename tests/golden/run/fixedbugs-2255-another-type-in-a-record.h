/* Defect 2255: as `fixedbugs-2255-another-type.h`, the `tb` handed out inside a
 * returned struct. */
typedef struct ta ta;
typedef struct tb tb;
typedef struct holder { tb *b; long long n; } holder;
static long long arena[8];
static inline __attribute__((noinline)) ta *a_open(void) { arena[0] = 1; return (ta *)arena; }
static inline __attribute__((noinline)) void a_close(ta *x) { (void)x; arena[0] = 0; }
static inline __attribute__((noinline)) holder b_holder(void) { arena[0] = 7; holder h = {(tb *)arena, 1}; return h; }
static inline __attribute__((noinline)) long long b_read(tb *x) { return *(long long *)x; }
