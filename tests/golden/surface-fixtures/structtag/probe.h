/* A struct tag with no typedef — `struct probe` and never `probe` — declared in
 * one module and USED in another. The second module's translation unit spells
 * the handle in the prototypes it declares for the first, so a qualifier the
 * compiler learns from clang has to reach every unit and not only the one that
 * carries the group (defect 037, 2026-09-15). */
#include <stdint.h>

struct probe {
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
