/* runtime.c — the Heroes C runtime (design.md §4.20). M0: minimal.
 * M5a: print's segment printers, and a panic that flushes. */

#include "heroes_runtime.h"

#include <stdio.h>
#include <stdlib.h>

_Noreturn void hero_panic(const char *msg) {
    /* Flush first, and the reason is measured rather than defensive: on Darwin
     * six bytes of buffered stdout survived abort() through a pipe, and glibc
     * >= 2.27 does not flush on abort either. Without this line a golden test
     * for a program that prints and then aborts is platform-dependent — which
     * is the one thing the double-emit and two-configuration harnesses exist to
     * rule out (panel 020). */
    fflush(stdout);
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

/* print's contract (panel 006, now stated in the spec): no separator between
 * values, exactly one trailing newline. So a segment printer writes only its
 * own value and `hero_print_end` owns the line ending.
 *
 * Why the runtime owns the newline and not the emitter: the emitter would need
 * putchar, and `#include <stdio.h>` in every generated translation unit is a
 * header-collision surface on every FFI program (CLAUDE.md §7). Measured — the
 * emitter-owned variant is `error: call to undeclared function 'putchar'`. */

void hero_print_int(int64_t v) {
    printf("%lld", (long long)v);
}

void hero_print_bool(bool v) {
    fputs(v ? "true" : "false", stdout);
}

void hero_print_end(void) {
    putchar('\n');
}
