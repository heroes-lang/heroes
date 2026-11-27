/* unwind-copied-catch.h (panel 258's R2, step 214d): a catch inside a
 * coroutine whose frames a stack-copying library copies in and out of one
 * region of the thread's own stack (the copying header of panel 259's
 * compiler-engineer). The catch yields after its jump lands; the other
 * coroutine then runs at the same addresses and returns from a C call; then
 * the catch, resumed, reads the message the callback its jump left lent it. */
#include <setjmp.h>
#include <string.h>
#include "fixedbugs-coroutines-copied-out-and-in-of-one-region-carved-from-the-threads-own.h"
static const char *cc_said[CP_N];
static jmp_buf cc_env[CP_N];
/* Through a pointer, so glibc's `nothrow` declaration of `longjmp` does not
 * compile the call as one that cannot unwind (`unwind-catch.h`'s reason). */
static __typeof__(&longjmp) volatile cc_jump;
static inline int64_t cc_try(ow_cb_t cb, int64_t k) {
    cc_jump = longjmp;
    if (setjmp(cc_env[k]) == 0) return cb(k);
    cp_yield();
    return (int64_t)strlen(cc_said[k]);
}
static inline void cc_fail(int64_t k, const char *msg) {
    cc_said[k] = msg;
    cc_jump(cc_env[k], 1);
}
