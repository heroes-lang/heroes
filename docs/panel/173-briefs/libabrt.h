/* Panel 173, ffi-pragmatist, question 4: the smallest C library that has an
 * opinion about SIGABRT. Two ways it can install its handler — from a call the
 * program makes (`lib_install`), or before `main` runs at all (the constructor,
 * which fires only when LIB_CTOR is set in the environment, so one binary
 * measures both orders). The handler writes one line and exits 77, a number no
 * signal produces, so whose handler ran is read off the exit code alone. */
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static void lib_abrt_handler(int sig) {
    (void)sig;
    static const char line[] = "library: my SIGABRT handler ran\n";
    write(2, line, sizeof line - 1);
    _exit(77);
}
static inline void lib_install(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = lib_abrt_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTRAP, &sa, NULL);
}
__attribute__((constructor)) static void lib_ctor(void) {
    if (getenv("LIB_CTOR")) lib_install();
}

/* The freeing routes, for questions 4 and 5. */
static inline void lib_free(const char *p) { free((void *)(uintptr_t)p); }
static inline void lib_abort(void) { abort(); }                      /* a library's assert */
static inline void lib_trap(void) { __builtin_trap(); }              /* a library's unreachable */
static inline void lib_double_free_own(void) { char *q = malloc(32); free(q); free(q); } /* C's own pointer */
static inline void lib_interior_own(void) { char *q = malloc(64); free(q + 16); }        /* C's own pointer */
