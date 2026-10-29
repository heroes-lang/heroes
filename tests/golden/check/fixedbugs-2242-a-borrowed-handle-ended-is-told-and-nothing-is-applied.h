/* Defect 2242's handle: panel 231's generated shape 5, an accumulator C
 * allocates and gives back with a call that frees it, and that a parent can
 * adopt, freeing it when the parent is given back. */
#include <stdint.h>
#include <stdlib.h>

typedef struct g5_acc g5_acc;
struct g5_acc { int64_t total; int64_t count; g5_acc *child; };

static inline g5_acc *g5_open(int64_t start) {
    g5_acc *a = (g5_acc *)malloc(sizeof *a);
    if (a == NULL) return NULL;
    a->total = start;
    a->count = 0;
    a->child = NULL;
    return a;
}

static inline int32_t g5_close(g5_acc *a) {
    int32_t n = (int32_t)a->count;
    if (a->child != NULL) free(a->child);
    free(a);
    return n;
}

static inline void g5_into(g5_acc *child, g5_acc *parent) {
    if (parent->child != NULL) free(parent->child);
    parent->child = child;
}
