/* A handle with a pool behind it, so the shapes can be declared without an
 * allocator, and the header is shipped beside the case so it is read on all
 * three platforms rather than where a library happens to be installed.
 *
 * Every struct here exists to put a handle somewhere the rule could not see
 * before 2026-09-14: in a field, in a field of a field, in a fixed array, and
 * through an out-parameter. Real C does all four — `struct addrinfo` carries
 * pointers a caller must free, and raylib's `Font` is returned BY VALUE
 * carrying two owned pointers released by one `UnloadFont`. */
#include <stdint.h>

typedef struct Slot Slot;
typedef struct Conn Conn;

struct Slot {
    int64_t n;
};

struct Conn {
    int64_t m;
};

typedef struct Pair {
    Slot *s;
    int64_t k;
} Pair;

typedef struct Outer {
    Pair p;
    int64_t z;
} Outer;

typedef struct Four {
    Slot *a[4];
} Four;

static inline Slot *slot_open(int64_t n) {
    static struct Slot pool[8];
    static int64_t next = 0;
    pool[next].n = n;
    return &pool[next++];
}

static inline void slot_close(Slot *s) { (void)s; }

static inline void conn_close(Conn *c) { (void)c; }

static inline Pair pair_make(int64_t n) {
    Pair p;
    p.s = slot_open(n);
    p.k = n;
    return p;
}

static inline Outer outer_make(int64_t n) {
    Outer o;
    o.p = pair_make(n);
    o.z = n;
    return o;
}

static inline Four four_make(int64_t n) {
    Four f = {{0}};
    for (int i = 0; i < 4; i++) f.a[i] = slot_open(n);
    return f;
}

static inline void pair_out(int64_t n, Pair *out) { *out = pair_make(n); }
