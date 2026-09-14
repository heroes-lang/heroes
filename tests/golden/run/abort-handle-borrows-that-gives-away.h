/* A handle with a pool behind it, so the pair can be counted without an
 * allocator, and the header is shipped beside the case so it fires on all three
 * platforms rather than where a library happens to be installed.
 *
 * **Nothing in this header says which of `slot_open` and `slot_peek` gives the
 * handle away**, and that is the whole point of the case beside it: the two are
 * the same C signature and clang cannot tell them apart, which is the count
 * panel 148 refused the inferred obligation on — twelve real headers of
 * nineteen declare exactly this pair. */
#include <stdint.h>

typedef struct Slot Slot;

struct Slot {
    int64_t n;
};

static inline Slot *slot_open(int64_t n) {
    static struct Slot pool[4];
    static int64_t next = 0;
    pool[next].n = n;
    return &pool[next++];
}

static inline void slot_close(Slot *s) { (void)s; }

static inline int64_t slot_value(Slot *s) { return s->n; }
