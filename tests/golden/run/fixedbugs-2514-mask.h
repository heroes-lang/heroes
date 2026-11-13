/* Defect 2514, 2026-11-13: a typedef of a number, as Darwin's `sigset_t` is,
 * filled through its pointer, read and handed back by value. */
typedef unsigned int mask2514_t;

static inline int mask2514_fill(mask2514_t *m) {
    *m = 42u;
    return 0;
}

static inline int mask2514_add(mask2514_t *m, int bit) {
    if (bit < 1 || bit > 32) {
        return -1;
    }
    *m |= 1u << (bit - 1);
    return 0;
}

static inline mask2514_t mask2514_twice(mask2514_t m) {
    return m * 2u;
}
