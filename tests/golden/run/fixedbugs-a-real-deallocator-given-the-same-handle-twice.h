/* Defect 071's C side, and it is the case `abort-handle-given-back-twice.h`
 * wrote down and declined to run. That header says, in its own words:
 *
 *   "`slot_close` deliberately does nothing, so the second release below does
 *    not corrupt anything on this machine and the case can assert the COUNT
 *    rather than a crash. Against a real allocator the same program is a
 *    double free."
 *
 * True, written down on 2026-09-15, and never followed by the case that runs
 * it against one. This is that case: `blk_close` really frees, so the guard has
 * to speak BEFORE the second call rather than at exit — which is the whole of
 * defect 071. */
#include <stdint.h>
#include <stdlib.h>

typedef struct blk blk;

struct blk {
    int64_t n;
};

static inline blk *blk_open(int64_t n) {
    blk *b = (blk *)malloc(sizeof(blk));
    b->n = n;
    return b;
}

static inline void blk_close(blk *b) { free(b); }

static inline int64_t blk_value(blk *b) { return b->n; }
