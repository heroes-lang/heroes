/* fixedbugs-buffers-lent-across-a-librarys-own-switch-on-carved-stacks.h (panel 259's compiler-engineer,
 * `lendown.h`): the coroutines of the header it includes, switched by its assembly on stacks carved from `ow_host`'s
 * frame, and lends held across those switches. C bumps a local lent to it in place; C writes into a buffer lent to
 * it, starts or yields a coroutine while that lend is open, and writes again once it is back. */
#include "fixedbugs-coroutines-switched-by-a-librarys-own-switch-on-stacks-carved-from-the-threads-own.h"
static inline void ow_bump(int64_t *n) { *n += 1; }
static inline void ow_start_lent(int64_t k, ow_cb_t f, unsigned char *buf) {
    buf[0] = (unsigned char)(10 + k);
    ow_start(k, f);
    buf[1] = (unsigned char)(20 + k);
}
static inline void ow_yield_lent(int64_t k, unsigned char *buf) {
    buf[0] = (unsigned char)(30 + k);
    ow_yield();
    buf[1] = (unsigned char)(40 + k);
}
