/* Defect 2337's binding: a C function that takes a callback and calls it, so
 * the program binds a callback and every function whose address it takes is
 * in the set a thunk is written for. */
#include <stdint.h>

static int64_t fixedbugs_2337_apply(int64_t (*cb)(int64_t), int64_t n) { return cb(n); }
