/* Defect 2584: a C library that raises SIGILL at itself, with no handler of
 * its own, so the Heroes runtime's lease handler tells the death.
 *
 * POSIX: `raise(SIGILL)`. Darwin delivers it with `si_code` 2 and `si_pid` 0,
 * as it delivers a `udf`, and the runtime's line named a trap instruction the
 * program never ran.
 *
 * Windows has no raised SIGILL the runtime takes: there the program runs the
 * illegal instruction itself, an exception the runtime's filter of last resort
 * tells by its own words, which name no trap either. */
#include <signal.h>

#if defined(_WIN32)
static __attribute__((noinline)) void raised_ill(void) { __builtin_trap(); }
#else
static __attribute__((noinline)) void raised_ill(void) { raise(SIGILL); }
#endif
