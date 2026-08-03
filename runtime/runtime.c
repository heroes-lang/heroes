/* runtime.c — the Heroes C runtime (design.md §4.20). M0: minimal. */

#include "heroes_runtime.h"

#include <stdio.h>
#include <stdlib.h>

_Noreturn void hero_panic(const char *msg) {
    fprintf(stderr, "panic: %s\n", msg);
    /* abort(), not exit(): a panic is a program error and should trip
     * debuggers and sanitizers, not look like a clean shutdown. */
    abort();
}

_Noreturn void hero_panic_overflow(void) {
    hero_panic("integer overflow");
}

_Noreturn void hero_unreachable(void) {
    hero_panic("entered unreachable code — this is a compiler bug, please report it");
}

void hero_print_int(int64_t v) {
    printf("%lld\n", (long long)v);
}
