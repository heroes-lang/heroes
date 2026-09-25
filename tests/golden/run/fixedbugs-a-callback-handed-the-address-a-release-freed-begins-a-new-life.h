/* A pool with a FREE LIST, so the reuse is deterministic, and a release that
 * calls back into the program AFTER it has freed its node: the callback's
 * next node is the one just freed, handed out while the release is still
 * inside C (defect 098, on one thread). `sn_park` keeps a node for the
 * program to take back later; `sn_find` hands out a node with a reference on
 * it, as a lookup that may return an object it just made does. */
#include <stdint.h>

typedef struct sn sn;

struct sn {
    int64_t v;
    sn *next_free;
};

static sn sn_pool[8];
static sn *sn_free_list = 0;
static int64_t sn_next_fresh = 0;
static int64_t sn_made = 0;
static sn *sn_parked = 0;

static inline __attribute__((noinline)) sn *sn_new(void) {
    sn *n;
    if (sn_free_list != 0) {
        n = sn_free_list;
        sn_free_list = n->next_free;
    } else {
        n = &sn_pool[sn_next_fresh++];
    }
    sn_made++;
    n->v = sn_made;
    n->next_free = 0;
    return n;
}

static inline __attribute__((noinline)) void sn_free(sn *n) {
    n->v = -1;
    n->next_free = sn_free_list;
    sn_free_list = n;
}

static inline __attribute__((noinline)) int64_t sn_value(sn *n) { return n->v; }

static inline __attribute__((noinline)) int64_t sn_free_then(sn *n, int64_t (*then)(int64_t)) {
    sn_free(n);
    return then(0);
}

static inline __attribute__((noinline)) void sn_park(sn *n) { sn_parked = n; }

static inline __attribute__((noinline)) sn *sn_unpark(void) { return sn_parked; }

static inline __attribute__((noinline)) sn *sn_find(void) { return sn_new(); }

static inline __attribute__((noinline)) void sn_put(sn *n) { sn_free(n); }
