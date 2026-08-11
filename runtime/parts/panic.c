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

_Noreturn void hero_panic(const char *msg) {
    fflush(stdout);
    fprintf(stderr, "panic: %s\n", msg);
    abort();
}
_Noreturn void hero_panic_overflow(void) { hero_panic("integer overflow"); }
_Noreturn void hero_unreachable(void) {
    hero_panic("entered unreachable code — this is a compiler bug, please report it");
}

void hero_print_int(int64_t v) { printf("%lld", (long long)v); }
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
        n = n * 10 + (*p - '0');
    }
    return n;
}
