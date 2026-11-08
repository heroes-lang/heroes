#include <stdint.h>
typedef struct heroes_held {
    int64_t first;
    int64_t rest[(1 << 24) - 1];
} heroes_held;
static heroes_held held_make(int64_t n) { heroes_held h = {0}; h.first = n; return h; }
static int64_t apply_ret(heroes_held (*cb)(int64_t), int64_t n) { (void)cb; return n; }
