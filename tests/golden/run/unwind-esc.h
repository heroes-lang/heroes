/* unwind-esc.h (panel 262's blind programs' esc.h, for step 214c of panel 258's R2): sw.h's coroutines, a save point around resuming one, and a jump back to that save point from inside the
 * coroutine, which leaves it for good: its function never returns.
 * The jump that leaves the coroutine is made as a library built apart makes
 * it: not instrumented under AddressSanitizer, and through a pointer whose type
 * says nothing of returning, so clang knows of no function here that never
 * returns. Instrumented, or seen as one that never returns, clang puts
 * `__asan_handle_no_return` before the call, in the leaving function at -O0
 * and in its Heroes caller at -O2, and on a stack from `malloc` ASan writes
 * its warning into the program's stderr whatever the runtime does (measured
 * on main de57f9129 with step 214a reverted, and at -O2 in step 214d). The
 * pointer is set at run time, so the jump goes through the runtime's hook as
 * the program's own `longjmp` does. */
#include <setjmp.h>
#include "unwind-sw.h"
static jmp_buf esc_env;
static void (*volatile esc_jump)(jmp_buf, int);
/* Resumes coroutine k under a save point; where the coroutine jumps back to it, -1. */
static int64_t esc_resume(int64_t k) {
    esc_jump = longjmp;
    if (setjmp(esc_env) == 0) return sw_resume(k);
    return -1;
}
/* Leaves the running coroutine for good, by a longjmp to the save point esc_resume made. */
__attribute__((no_sanitize("address"))) static inline void esc_leave(void) { esc_jump(esc_env, 1); }
/* Calls f(n) from C. */
static inline int64_t esc_call(sw_fn_t f, int64_t n) { return f(n); }
