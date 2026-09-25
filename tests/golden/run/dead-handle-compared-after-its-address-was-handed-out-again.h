/* Panel 175's completeness critic, `u1_static`: one address, handed out again.
 * `f_open` returns the same static cell every time and `f_close` frees
 * nothing, so C's allocator is not what reuses the address here and the case
 * cannot depend on one (defect 077: two byte-identical binaries read 0 and
 * 134 by their code signature alone). */
#include <stdint.h>

typedef struct hh hh;

struct hh {
    int64_t n;
};

static inline __attribute__((noinline)) hh *f_open(void) {
    static hh cell;
    cell.n = 1;
    return &cell;
}

static inline __attribute__((noinline)) void f_close(hh *x) { (void)x; }
