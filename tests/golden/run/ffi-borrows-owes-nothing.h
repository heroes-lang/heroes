/* Two producers of one handle type, and they differ in the one way no C header
 * states: `slot_open` hands the caller a slot to give back, `slot_peek` hands
 * back one the pool keeps. Identical signatures, opposite obligations — the
 * shape panel 148's ffi seat found in twelve real headers of nineteen, and the
 * reason the obligation cannot be inferred from the type. */
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

/* The borrowed one: the same type, and the caller must NOT give it back. */
static inline Slot *slot_peek(void) {
    static struct Slot kept = { 100 };
    return &kept;
}

static inline void slot_close(Slot *s) { (void)s; }

static inline int64_t slot_value(Slot *s) { return s->n; }
