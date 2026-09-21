/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/panic.c — stopping, and the three unconditional printers.
 *
 * Every abort in the language funnels through `hero_panic`: an out-of-range
 * index, integer overflow, a `.must()` on an error, a slice that splits a
 * character. It flushes stdout first, because the interesting half of a failing
 * program is what it printed before it stopped.
 *
 * design.md §4.14 (arithmetic edges abort), §4.20.
 */

#include <signal.h>

/* THE RUNTIME HAS SPOKEN, and this is the one bit that says so. Every death
 * the runtime initiates is a line on stderr followed by `hero_abort()`, which
 * is SIGABRT, which reaches the lease handler in parts/os.c. That handler
 * reports a C `free` of live leased bytes, and it must stay silent when the
 * death is ours: panel 173 measured a second, false `panic:` line under an
 * out-of-range index, a stack exhaustion and a failed `assert` while a lease
 * was live. Fifteen sites in five files aborted directly; they are one funnel
 * now, so the flag cannot be forgotten at the sixteenth. `sig_atomic_t` and
 * `abort()` are both on the async-signal-safe list, and stack.c's handler is
 * one of the callers. */
static volatile sig_atomic_t hero_runtime_spoke = 0;

static _Noreturn void hero_abort(void) {
    hero_runtime_spoke = 1;
    abort();
}

_Noreturn void hero_panic(const char *msg) {
    fflush(stdout);
    fprintf(stderr, "panic: %s\n", msg);
    hero_abort();
}
_Noreturn void hero_panic_overflow(void) { hero_panic("integer overflow"); }
_Noreturn void hero_unreachable(void) {
    hero_panic("entered unreachable code — this is a compiler bug, please report it");
}

void hero_print_int(int64_t v) { printf("%lld", (long long)v); }
/* See `hero_uint_to_str`: `u64` above 2^63 prints negative through the signed one. */
void hero_print_uint(uint64_t v) { printf("%llu", (unsigned long long)v); }
void hero_print_bool(bool v) { fputs(v ? "true" : "false", stdout); }
void hero_print_end(void) { putchar('\n'); }

/* Which test `heroes test` asked for. In the runtime rather than the generated C
 * because CLAUDE.md §7 lets a translation unit include `heroes_runtime.h` and
 * nothing else — `strtol` would need `<stdlib.h>`, and a second header in every
 * generated unit is the collision surface §4.19 spends the FFI budget avoiding.
 *
 * A missing or unreadable argument is a compiler bug, not a user error: the only
 * caller is `heroes test`, which always passes an index it computed itself. */
int64_t hero_test_index(int argc, char **argv) {
    if (argc < 2 || argv[1] == NULL) hero_panic("no test index — this is a compiler bug");
    int64_t n = 0;
    for (const char *p = argv[1]; *p != '\0'; p++) {
        if (*p < '0' || *p > '9') hero_panic("bad test index — this is a compiler bug");
        /* Signed overflow is UB, and CLAUDE.md §7's rule for the generated code
         * is that arithmetic aborts through `__builtin_*_overflow` rather than
         * wrapping — the runtime's own arithmetic owes the same. Every digit was
         * validated and the accumulation was not (2026-08-12, sweep 001). */
        if (__builtin_mul_overflow(n, (int64_t)10, &n)
            || __builtin_add_overflow(n, (int64_t)(*p - '0'), &n)) {
            hero_panic("bad test index — this is a compiler bug");
        }
    }
    return n;
}
