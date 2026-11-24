/* lendjump.h: a C function that fills a buffer the program lends it and then longjmps out of the
 * lending call, over the frame of the Heroes callback that made it. */
#include <setjmp.h>
#include <stdint.h>
typedef int64_t (*lj_cb_t)(int64_t);
static jmp_buf lj_env;
static inline int32_t lj_fill(unsigned char *md) { md[0] = 1; return 0; }
static inline int64_t lj_try(lj_cb_t cb) {
    if (setjmp(lj_env) == 0) { cb(1); return -1; }
    return 7;
}
