/* fixedbugs-2704-longjmp.h: a C jump that abandons the frame of a Heroes callback, as a
 * plain `longjmp` does (libpng's `png_error`), where the platform has no signals to
 * speak of. */
#include <setjmp.h>
#include <stdint.h>

typedef int64_t (*ab_cb_t)(int64_t);

static jmp_buf ab_env;

/* `longjmp`s to `ab_try`'s save point, over every frame between. */
static inline void ab_throw(void) { longjmp(ab_env, 1); }

/* Calls `cb`; where `cb` returns, -1; where something jumped over it, 7. */
static inline int64_t ab_try(ab_cb_t cb) {
    if (setjmp(ab_env) == 0) {
        cb(1);
        return -1;
    }
    return 7;
}

