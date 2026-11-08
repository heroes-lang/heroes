/* Defect 2286's callbacks: C calls each one back, so a convention C and the
 * program disagree on shows at run time. 256 KiB, past `LARGE` and small
 * enough for C's own frame to hold its by-value copies. */
#include <stdint.h>

typedef struct fixedbugs_2286_held {
    int64_t first;
    int64_t rest[(1 << 15) - 1];
} fixedbugs_2286_held;

static int64_t fixedbugs_2286_call_param(int64_t (*cb)(fixedbugs_2286_held), int64_t n) {
    fixedbugs_2286_held h = {0};
    h.first = n;
    return cb(h);
}

static int64_t fixedbugs_2286_call_ret(fixedbugs_2286_held (*cb)(int64_t), int64_t n) {
    fixedbugs_2286_held h = cb(n);
    return h.first + h.rest[(1 << 15) - 2];
}

static fixedbugs_2286_held fixedbugs_2286_held_make(int64_t n) {
    fixedbugs_2286_held h = {0};
    h.first = n;
    h.rest[(1 << 15) - 2] = 1;
    return h;
}
