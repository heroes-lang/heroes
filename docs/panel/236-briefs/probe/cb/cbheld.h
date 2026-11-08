#include <stdint.h>
typedef struct heroes_held {
    int64_t first;
    int64_t rest[(1 << 24) - 1];
} heroes_held;
typedef struct big_named {
    int64_t a[600];
} big_named;
static int64_t apply_held(int64_t (*cb)(heroes_held), int64_t n) { (void)cb; return n; }
static int64_t apply_ret(heroes_held (*cb)(int64_t), int64_t n) { (void)cb; return n; }
static int64_t apply_named(int64_t (*cb)(big_named), int64_t n) { (void)cb; return n; }
