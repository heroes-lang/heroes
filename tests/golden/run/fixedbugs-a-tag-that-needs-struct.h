/* Two struct tags with NO typedef, which is how `netdb.h` declares
 * `struct addrinfo` and how every linked list in every C header is declared:
 * the bare word `probe` names nothing in C, only `struct probe` does. Shipped
 * beside the case so it fires on all three platforms rather than where a
 * library happens to be installed.
 *
 * Two of them, because clang names every tag it refuses in one compile, and
 * the compiler has to learn BOTH from one round rather than one per round. */
#include <stdint.h>

struct probe {
    int64_t n;
};

struct gauge {
    int64_t n;
};

static inline struct probe *probe_open(int64_t n) {
    static struct probe pool[4];
    static int64_t next = 0;
    pool[next].n = n;
    return &pool[next++];
}

static inline int64_t probe_value(struct probe *p) { return p->n; }

static inline void probe_close(struct probe *p) { (void)p; }

static inline struct gauge *gauge_open(int64_t n) {
    static struct gauge pool[4];
    static int64_t next = 0;
    pool[next].n = n;
    return &pool[next++];
}

static inline int64_t gauge_read(struct gauge *g) { return g->n; }

static inline void gauge_close(struct gauge *g) { (void)g; }
