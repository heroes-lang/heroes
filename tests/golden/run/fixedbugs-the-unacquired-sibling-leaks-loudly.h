/* Two handles reached by two fields of the SAME record type — the shape that
 * made `check/reaches.hero`'s `gather` stop after the first one.
 *
 * The pool is `static` so the pair can be counted without an allocator, and the
 * header is shipped beside the case so it fires on all three platforms rather
 * than where a library happens to be installed. `static inline` and not
 * `static`: this header reaches a second translation unit that calls neither,
 * and C11 exempts an unused inline where it would warn on an unused static. */
#include <stdint.h>

typedef struct Slot Slot;

struct Slot {
    int64_t n;
};

typedef struct {
    Slot *s;
} Inner;

typedef struct {
    Inner a;
    Inner b;
} Pair;

static inline Slot *slot_take(int64_t n) {
    static struct Slot pool[4];
    static int64_t next = 0;
    pool[next].n = n;
    return &pool[next++];
}

static inline Pair pair_open(int64_t n) {
    Pair p;
    p.a.s = slot_take(n);
    p.b.s = slot_take(n + 1);
    return p;
}

static inline void slot_close(Slot *s) { (void)s; }

static inline int64_t slot_value(Slot *s) { return s->n; }
