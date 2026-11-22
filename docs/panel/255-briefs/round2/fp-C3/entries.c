/* -- the entries and the trampoline (panel 255's route C) ---------------- */

/* PANEL 255'S ROUTE C ON DARWIN, built by the ffi-pragmatist on Darwin arm64:
 * the entries are the trampolines' frames, kept as addresses in the claimed
 * thread's record (`parts/sigguard.c`), as B' keeps its wrappers' on Linux, so
 * nothing ever reads through an entry; a jump reads its target's stack
 * pointer out of the buffer it jumps to (the buffer lives: the C library
 * reads it next), decoded by a key calibrated before `main`, and drops the
 * entries that stack pointer proves left, by `parts/siginstall.c`'s rule. No
 * save point is rewritten and no record is kept. */

/* The claimed thread's record through the thread key, never a thread-local
 * access; NULL on a thread nothing claimed, where the thunk refuses anyway. */
static HeroSignalStack *hero_rebind_own(void) {
    if (atomic_load_explicit(&hero_signal_keyed, memory_order_acquire) != 2) return NULL;
    return pthread_getspecific(atomic_load_explicit(&hero_signal_key, memory_order_relaxed));
}

static bool hero_rebind_on_own(const HeroSignalStack *own, uintptr_t at) { return at >= own->lo && at < own->hi; }

/* `parts/siginstall.c`'s `hero_sig_prune`: the entries, from the innermost,
 * `at` proves left. */
static void hero_rebind_prune(HeroSignalStack *own, uintptr_t at) {
    bool here = hero_rebind_on_own(own, at);
    while (own->depth > 0) {
        uintptr_t w = own->entries[own->depth - 1];
        bool there = hero_rebind_on_own(own, w);
        bool left = (there == here && w <= at) || (!there && here);
        if (!left) break;
        own->depth -= 1;
    }
    if (own->unsure > own->depth) own->unsure = own->depth;
}

typedef void (*hero_rebind_catcher)(int, siginfo_t *, void *);

/* The kernel's call of the catcher, made here in its place (today's comment
 * on its type and its instrumentation stands): the entry pushed as B''s
 * wrapper pushes it, the catcher called, the entry popped. */
__attribute__((no_sanitize("function", "address"))) static void hero_rebind_tramp(hero_rebind_catcher catcher, int style,
                                                                                  int sig, siginfo_t *info,
                                                                                  void *context, uintptr_t token) {
    (void)style;
    HeroSignalStack *own = hero_rebind_own();
    int at = -1;
    if (own != NULL) {
        uintptr_t me = (uintptr_t)__builtin_frame_address(0);
        hero_rebind_prune(own, me);
        if (own->depth < HERO_SIG_DEPTH) {
            at = own->depth;
            own->entries[at] = me;
            atomic_signal_fence(memory_order_seq_cst);
            own->depth = at + 1;
            atomic_signal_fence(memory_order_seq_cst);
        }
    }
    catcher(sig, info, context);
    if (at >= 0) {
        atomic_signal_fence(memory_order_seq_cst);
        own->depth = at;
        if (own->unsure > at) own->unsure = 0;
    }
    __sigreturn(context, HERO_REBIND_RETURN_FLAVOR, token);
    __builtin_trap();
}

/* The thunk's question (`parts/sigguard.c`), `parts/siginstall.c`'s
 * `hero_sig_inside` asked from this frame. */
__attribute__((noinline)) static int hero_rebind_inside(void) {
    HeroSignalStack *own = hero_rebind_own();
    uintptr_t frame = (uintptr_t)__builtin_frame_address(0);
    if (own == NULL || own->depth == 0) return HERO_SIG_OUTSIDE;
    hero_rebind_prune(own, frame);
    bool here = hero_rebind_on_own(own, frame);
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        uintptr_t w = own->entries[i];
        if (hero_rebind_on_own(own, w) == here && w > frame) return i < own->unsure ? HERO_SIG_UNSURE : HERO_SIG_INSIDE;
    }
    return HERO_SIG_OUTSIDE;
}

