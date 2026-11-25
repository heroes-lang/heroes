/* fixedbugs-2752-levels.h: save points at several levels of one thread, and C
 * functions that `longjmp` to one of them over the frames of the program's
 * callbacks between (panel 258's R1; defects 2752, 2753 and 2756). C11's
 * `setjmp.h` alone, so every platform builds it. */
#include <setjmp.h>
#include <stdint.h>

typedef int64_t (*lv_cb_t)(int64_t);

static _Thread_local jmp_buf lv_env[8];
static _Thread_local int lv_level = 0;

/* Calls `cb(n)` at a new save level: its answer, or 7 where something jumped
 * to this level. */
static inline int64_t lv_try(lv_cb_t cb, int64_t n) {
    int at = lv_level;
    lv_level = at + 1;
    if (setjmp(lv_env[at]) == 0) {
        int64_t answer = cb(n);
        lv_level = at;
        return answer;
    }
    lv_level = at;
    return 7;
}

/* Jumps to the save point `up` levels above the newest, 0 the newest. */
static inline void lv_throw(int64_t up) { longjmp(lv_env[lv_level - 1 - (int)up], 1); }

/* A local lent in place, written, then jumped out of. */
static inline int32_t lv_put_throw(int32_t *x) {
    *x = 5;
    longjmp(lv_env[lv_level - 1], 1);
}

/* A buffer lent, written, then jumped out of the call it was lent to. */
static inline int32_t lv_fill_throw(unsigned char *md) {
    md[0] = 1;
    longjmp(lv_env[lv_level - 1], 1);
}

/* A buffer lent and written, then a callback at a new save level inside the
 * call it was lent to. */
static inline int64_t lv_fill_then(unsigned char *md, lv_cb_t cb, int64_t n) {
    md[0] = 2;
    return lv_try(cb, n);
}
