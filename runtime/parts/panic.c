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
