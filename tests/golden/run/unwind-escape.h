/* unwind-escape.h (panel 262's probe escape.h, the compiler-engineer's of panel 259, for step 214c of panel 258's R2): own.h's coroutines on stacks carved from ow_host's frame, and a
 * coroutine that leaves for good by a longjmp to the save point its host made, which lies BELOW the coroutine's
 * frames on the thread's stack (the host's frame is under the array). Then a callback, called after, must run.
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
#include "unwind-own.h"
static jmp_buf es_env;
static void (*volatile es_jump)(jmp_buf, int);
/* Resumes coroutine k under a save point; where the coroutine jumps back to it, -1. */
static int64_t es_resume(int64_t k) {
    es_jump = longjmp;
    if (setjmp(es_env) == 0) return ow_resume(k);
    return -1;
}
__attribute__((no_sanitize("address"))) static inline void es_escape(void) { es_jump(es_env, 1); }
static inline int64_t es_call(ow_cb_t f, int64_t n) { return f(n); }
