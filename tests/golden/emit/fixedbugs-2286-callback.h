/* Defect 2286's emit case: a small record bound `partial` (so it takes the
 * route whatever its size) and a C function that takes a callback returning
 * it. */
#include <stdint.h>

typedef struct fixedbugs_2286_small {
    int64_t first;
    int64_t second;
} fixedbugs_2286_small;

static int64_t fixedbugs_2286_apply(fixedbugs_2286_small (*cb)(int64_t), int64_t n) {
    return cb(n).first;
}
