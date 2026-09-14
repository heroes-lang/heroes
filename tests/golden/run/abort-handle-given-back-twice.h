/* A handle with a pool behind it, so the pair can be counted without an
 * allocator, and the header is shipped beside the case so it fires on all three
 * platforms rather than where a library happens to be installed.
 *
 * `slot_close` deliberately does nothing, so the second release below does not
 * corrupt anything on this machine and the case can assert the COUNT rather
 * than a crash. Against a real allocator the same program is a double free. */
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
