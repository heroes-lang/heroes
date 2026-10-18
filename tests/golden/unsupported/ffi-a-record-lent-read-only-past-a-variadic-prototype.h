/* Panel 217's R3(c), M-readonly-lend: a variadic function writing through
   every struct pointer it reads with va_arg, the shape of the critic's
   va_record_lent, which printed 103 from a `=` binding at exit 0. */
#include <stdarg.h>
#include <stdint.h>

struct tally { int64_t n; };

static inline int64_t tally_bump(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    for (int32_t i = 0; i < k; i++) {
        struct tally *t = va_arg(ap, struct tally *);
        t->n += 100;
    }
    va_end(ap);
    return 0;
}
