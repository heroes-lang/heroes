/* A handle with a pool behind it, so the pair can be counted without an
 * allocator, and the header is shipped beside the case so it fires on all three
 * platforms rather than where a library happens to be installed.
 *
 * **`pair_make` is the point of this header.** It hands the handle back inside
 * a STRUCT FIELD rather than as the result, which is the one shape where an
 * unmarked producer still reaches the runtime: `unmarked_handle_producer` reads
 * results and `@` out-parameters and does not look inside a returned record.
 * Real C does this — `struct addrinfo` carries pointers a caller must free. */
#include <stdint.h>

typedef struct Slot Slot;

struct Slot {
    int64_t n;
};

typedef struct Pair {
    Slot *s;
    int64_t k;
} Pair;

static inline Slot *slot_open(int64_t n) {
    static struct Slot pool[4];
    static int64_t next = 0;
    pool[next].n = n;
    return &pool[next++];
}

static inline void slot_close(Slot *s) { (void)s; }

static inline int64_t slot_value(Slot *s) { return s->n; }

static inline Pair pair_make(int64_t n) {
    Pair p;
    p.s = slot_open(n);
    p.k = n;
    return p;
}
