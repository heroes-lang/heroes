/* A pool that never hands an address out twice, big enough for the dead
 * set's whole window: 2^20 + 2 nodes of 8 bytes, static, so a read of a
 * freed node is silent and reads -1 (panel 177's item 2; the landing's
 * skeptic, probes `f4` and `f5`, 2026-09-25). */
#include <stdint.h>

typedef struct big big;

struct big {
    int64_t v;
};

static big big_pool[1048578];
static int64_t big_next = 0;

static inline __attribute__((noinline)) big *big_new(void) {
    big *b = &big_pool[big_next++];
    b->v = big_next;
    return b;
}

static inline __attribute__((noinline)) void big_free(big *b) { b->v = -1; }

static inline __attribute__((noinline)) int64_t big_value(big *b) { return b->v; }
