/* Defect 322, panel 197's R2: a heap block read after it is freed, in a
 * header's `static inline`, so the read is compiled under the program's own
 * words and its frame stands under the line of the program that called it.
 * The value read is never used: what the program prints is the same whatever
 * the freed block holds, so the plain runs are judged by it, and the read
 * goes through a `volatile` so no level drops it. */
#include <stdint.h>
#include <stdlib.h>

static inline int64_t read_after_free(int64_t v) {
    int64_t *cell = (int64_t *)malloc(sizeof *cell);

    if (cell == NULL) {
        return 0;
    }
    *cell = v;
    free(cell);
    volatile int64_t seen = *(volatile int64_t *)cell;
    (void)seen;
    return 1;
}
