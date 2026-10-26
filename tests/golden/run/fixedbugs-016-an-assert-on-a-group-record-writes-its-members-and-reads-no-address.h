/* Defect 016's group shapes (panel 230's R2): a struct with a fixed array, a
   pointer and a C string, and a union of two integers as wide as it, so that
   `==` is defined on both. `static inline` and `noinline` as every header in
   this directory. */
#include <stdint.h>

typedef struct Pair {
    int32_t id;
    float pair[2];
    void *raw;
    const char *name;
} Pair;

typedef union Num {
    int32_t i;
    uint32_t u;
} Num;

static inline __attribute__((noinline)) Pair pair_make(int32_t id) {
    Pair p = {0};
    p.id = id;
    p.pair[0] = 0.5f;
    p.pair[1] = 1.5f;
    p.raw = 0;
    p.name = "pair";
    return p;
}
