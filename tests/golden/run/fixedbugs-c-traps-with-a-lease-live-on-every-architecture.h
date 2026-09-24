/* A C function that executes a trap instruction: `brk` on arm64, which is
 * SIGTRAP, and `ud2` on x86-64, which is SIGILL. noinline so the trap is inside
 * C and not folded into the caller. */
static __attribute__((noinline)) void c_trap(const char *s) { (void)s; __builtin_trap(); }
