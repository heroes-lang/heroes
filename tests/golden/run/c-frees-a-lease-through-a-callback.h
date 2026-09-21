/* Panel 172's `b_callback`: the disposer arrives as a VALUE, which is
 * measurement 037's own give-away construct with a lease where the `malloc`
 * was. No word on `take_cb`'s parameter could be true: it is the callback that
 * frees, and it is an argument. */
#include <stdlib.h>
#include <stdint.h>
static inline void eat(const char *s) { free((void *)(uintptr_t)s); }
static inline void take_cb(const char *s, void (*cb)(const char *)) { cb(s); }
