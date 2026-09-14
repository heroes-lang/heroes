/* A handle with a pool behind it, so the pair can be counted without an
 * allocator: `slot_open` hands one over, `slot_close` takes it back, and the
 * header is shipped beside the case so it fires on all three platforms rather
 * than where a library happens to be installed. */
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
