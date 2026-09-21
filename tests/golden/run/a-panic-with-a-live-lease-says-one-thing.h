/* Panel 172's `panic_lease`: a Heroes panic while a lease is live. C frees
 * nothing here; the runtime's own abort must not be reported as C's. */
static inline void look(const char *s) { (void)s; }
