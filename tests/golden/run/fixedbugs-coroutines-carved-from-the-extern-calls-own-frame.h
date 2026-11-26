/* fixedbugs-coroutines-carved-from-the-extern-calls-own-frame.h (panel 259's compiler-engineer, `below.h`): two
 * coroutines on quarters of an array in THIS extern call's own frame, below its Heroes caller, switched by the
 * assembly of the header it includes; the call starts both, resumes 1 then 0, and answers a * 100 + b. */
#include "fixedbugs-coroutines-switched-by-a-librarys-own-switch-on-stacks-carved-from-the-threads-own.h"
static inline int64_t ow_below(ow_cb_t f) {
    char stacks[OW_N * OW_STACK];
    ow_carved = stacks;
    ow_start(0, f);
    ow_start(1, f);
    int64_t b = ow_resume(1);
    int64_t a = ow_resume(0);
    ow_carved = NULL;
    __asm__ volatile("" ::"r"(stacks) : "memory");
    return a * 100 + b;
}
