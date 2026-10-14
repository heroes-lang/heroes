/* Compiler-engineer, panel 215: a C function that returns a padded record by
   value. Its body zeroes every byte first, so the padding C holds is zero;
   what reaches the caller is what the ABI carries. noinline so -O1..-O3
   cannot fold the return away. */
#include <string.h>
#include <unistd.h>
struct cd { char c; double d; };
__attribute__((noinline)) static struct cd cd_make(void) {
    struct cd r; memset(&r, 0, sizeof r); r.c = 1; r.d = 2.0; return r;
}
static inline long w_cd(const struct cd *p) { return (long)write(1, p, sizeof *p); }
