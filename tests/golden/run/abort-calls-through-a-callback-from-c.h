/* Panel 253's R5: a C function of the program's own header that calls the
 * Heroes function it is handed. */
#include <stdint.h>

static inline int64_t abort_calls_through(int64_t (*f)(int64_t), int64_t x) {
    return f(x) + 1;
}
