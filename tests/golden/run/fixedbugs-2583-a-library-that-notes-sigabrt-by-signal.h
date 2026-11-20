/* Defect 2583's twin on every platform (the author's ruling A of 2026-11-20,
 * lane-signals-2): the program's own C for its case, a library that installs,
 * before `main`, a SIGABRT handler by C11's `signal` that only counts what it
 * got and returns, so the runtime's handler finds it and calls it first; and
 * C11's `raise`. Nothing else, so every platform builds it, Windows among
 * them, whose C library has neither `sigaction` nor `fork`
 * (`fixedbugs-2583-a-library-that-notes-a-signal-and-returns.h` is POSIX's).
 * Nothing here allocates or prints. */
#include <signal.h>
#include <stdint.h>

static volatile sig_atomic_t twin_count = 0;

static void twin_handler(int sig) {
    (void)sig;
    twin_count++;
}

__attribute__((constructor)) static void twin_install(void) { signal(SIGABRT, twin_handler); }

/* SIGABRT raised once; how many the handler has noted. */
static inline int64_t twin_raise(void) {
    raise(SIGABRT);
    return (int64_t)twin_count;
}
