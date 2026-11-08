/* The package `pt`'s own header (defect 2357): a struct the package's group
 * binds as a record, and the function that builds one. */
#include <stdint.h>

typedef struct pt_point {
    int64_t x;
    int64_t y;
} pt_point;

static inline pt_point pt_make(int64_t x, int64_t y) {
    pt_point p = { x, y };
    return p;
}
