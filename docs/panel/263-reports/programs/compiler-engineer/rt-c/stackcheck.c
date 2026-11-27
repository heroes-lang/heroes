/* parts/stackcheck.c: panel 263's compiler-engineer's prototype of route (c),
 * the slow path of `hero_stack_check` (heroes_runtime.h). Unlanded. */

/* Span 0: every thread's first check misses, and the miss measures. */
_Thread_local HeroStackWindow hero_stack_window = {0, 0};

/* What a check leaves below itself: the frames of the runtime's own C a
 * checked function calls and the stop's, never a Heroes frame (each is
 * checked). Overridable for the panel's runs. A quarter of a stack smaller
 * than four times it. */
#if !defined(HERO_STACK_RESERVE)
#define HERO_STACK_RESERVE ((uintptr_t)16 << 10)
#endif

static _Noreturn void hero_stack_check_refuse(const char *what) {
    const char *const said[] = {
        "panic: stack exhausted in ",
        what,
        " (a stack check at its entry: the stack it runs on has less than its reserve left)",
    };
    hero_signal_stop(said, sizeof said / sizeof said[0]);
}

void hero_stack_check_miss(const char *what, uintptr_t sp) {
    uintptr_t lo = 0, hi = 0;
    bool known = false;
#if defined(HERO_SIG_WRAPS)
    known = hero_sig_known_range(sp, &lo, &hi);
#elif defined(HERO_SIG_REBIND)
    known = hero_rebind_known_range(sp, &lo, &hi);
#endif
    if (!known) {
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
        lo = hero_stack_walk_lo;
        hi = hero_stack_walk_hi;
#else
        lo = hero_stack_lo;
        hi = hero_stack_hi;
#endif
        known = hi > lo && sp >= lo && sp < hi;
    }
    if (!known) {
        /* A stack no wrapped `makecontext` named and not the thread's own:
         * today's behaviour, unchecked, until a switch wrapper resets. */
        hero_stack_window.base = 0;
        hero_stack_window.span = UINTPTR_MAX;
        if (getenv("HERO_P263_SAY") != NULL) fprintf(stderr, "p263: %s on an unknown stack, sp %p\n", what, (void *)sp);
        return;
    }
    uintptr_t reserve = HERO_STACK_RESERVE;
    if (reserve > (hi - lo) / 4) reserve = (hi - lo) / 4;
    if (getenv("HERO_P263_SAY") != NULL)
        fprintf(stderr, "p263: %s miss, sp %p, range [%p, %p), reserve %lu\n", what, (void *)sp, (void *)lo, (void *)hi,
                (unsigned long)reserve);
    if (sp < lo + reserve) hero_stack_check_refuse(what);
    hero_stack_window.base = lo + reserve;
    hero_stack_window.span = hi - (lo + reserve);
}
