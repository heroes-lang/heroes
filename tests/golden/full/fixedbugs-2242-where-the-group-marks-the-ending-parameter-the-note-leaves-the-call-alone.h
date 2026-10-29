/* Defect 2242's handle: panel 231's generated shape 5, an accumulator C
 * allocates, given back by a call that frees it or by one that frees it
 * through the caller's pointer and empties that pointer. */
#include <stdint.h>
#include <stdlib.h>

typedef struct g5_acc g5_acc;
struct g5_acc { int64_t total; int64_t count; };

static inline g5_acc *g5_open(int64_t start) {
    g5_acc *a = (g5_acc *)malloc(sizeof *a);
    if (a == NULL) return NULL;
    a->total = start;
    a->count = 0;
    return a;
}

static inline int32_t g5_close(g5_acc *a) {
    int32_t n = (int32_t)a->count;
    free(a);
    return n;
}

static inline void g5_free(g5_acc **a) {
    free(*a);
    *a = NULL;
}
