/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/siginstall.c — a wrapper of this runtime's around every handler a
 * program's C installs, on Linux and every POSIX system but Darwin (route B',
 * panel 249; adopted with route J by panel 251's R1, ratified 2026-11-19;
 * defect 2522; design.md §1.12, §4.19).
 *
 * WHAT IT IS FOR. `parts/sigguard.c`'s thunk asks whether a Heroes function
 * is running inside a signal. Its exact test and its walk see a handler that
 * calls the function directly or through a few frames with records; a
 * library's handler built without frame records, or one that reaches the
 * function through more than HERO_SIGNAL_WALK frames, passed both, and the
 * Heroes function ran inside the signal: 247's `fliblocale` killed in silence
 * on Darwin and run inside 1.35 to 2.17 million signals in Linux arm64, and
 * the tree's own three 2494 cases on the CI's Linux x86-64 leg at `-O2`.
 *
 * HOW. This runtime defines the C library's installers (`sigaction`, `signal`,
 * `siginterrupt`, and glibc's `bsd_signal`, `ssignal`, `__sigaction`,
 * `__sysv_signal`, `sysv_signal`, `sigset`, `sigignore`). An install of a
 * function keeps the program's handler and flags in a table and gives the
 * kernel this runtime's wrapper instead, with the program's own flags
 * (`SA_SIGINFO` added) and mask; a query answers the table's handler, so the
 * program reads back what it installed. The wrapper pushes its own frame's
 * address on the claimed thread's entries (`HeroSignalStack`,
 * `parts/sigguard.c`), calls the program's handler with the kernel's own
 * `siginfo_t` and context, and pops; the thunk refuses wherever an entry
 * stands (`hero_sig_inside`), whatever the handler kept of frame records.
 *
 * THE ENTRIES, AND WHAT DROPS ONE (`parts/sigentries.c`, panel 255's R3,
 * defect 2702): a handler's entry is dropped when its wrapper ends, and by
 * the jump or the swap this runtime sees (`parts/sigjump_save.c`) that
 * proves the handler left, judged by the stack the target lies on and never
 * by comparing two addresses on two stacks. No frame's address alone drops
 * an entry, neither the wrapper's nor the thunk's: a context switched where
 * this runtime cannot see it would pass for the same stack. A correct program
 * that jumped out of a handler by a jump this runtime sees and later called
 * a Heroes function deeper than the dead entry is not refused (panel 249's
 * twelve depth programs); one that left it by a jump this runtime does not
 * see is, in words that name that jump.
 *
 * THE C LIBRARY'S OWN `sigaction` is found by the C library's own
 * `dlsym(RTLD_NEXT)` (`parts/siglookup.c`) before `main` (a constructor),
 * never inside a signal; a shared library's
 * constructor that installs before this one runs finds it there, once. Every
 * step below is an atomic load or store or one system call, so each
 * definition is async-signal-safe, in a handler and in a forked child alike
 * (`parts/run.c`'s child, `platform/hero_proc.c`'s).
 *
 * WHAT IT DOES NOT REACH, said (defect 2522's rows): a raw `rt_sigaction`
 * system call, a `sigaction` a library reaches by `dlvsym` (`dlsym` is
 * answered with this file's since the author's ruling C of 2026-11-20,
 * `parts/siglookup.c`), glibc's own internal installs, a thread nothing claimed (the thunk refuses there in
 * defect 2492's words), and Darwin, whose two-level namespace binds a
 * library's call to libSystem's own name (`parts/rebind.c` is Darwin's). A
 * program library that defines one of these names is told
 * `ffi_defined_elsewhere` (panel 239's member, which reads the link's own
 * duplicate). */

#if !defined(_WIN32) && !defined(__APPLE__)

#include <dlfcn.h>

typedef int (*hero_sig_action_fn)(int, const struct sigaction *, struct sigaction *);
typedef void (*hero_sig_handler_fn)(int);
typedef void (*hero_sig_info_fn)(int, siginfo_t *, void *);

/* The program's own action per signal, as the kernel would hold it if this
 * runtime were not there: the handler (0 where the kernel holds no wrapper of
 * ours) and its flags. */
static _Atomic uintptr_t hero_sig_handlers[NSIG];
static _Atomic int hero_sig_flags[NSIG];

/* glibc's `_sigintr`, mirrored: the signals `siginterrupt` asked to
 * interrupt, which `signal` then installs without `SA_RESTART`. */
static _Atomic uint64_t hero_sig_interrupting = 0;

static _Atomic(hero_sig_action_fn) hero_sig_real_action = NULL;

/* `parts/siglookup.c`, below: the definition after the executable's, asked of
 * the C library's own `dlsym`, which `dlsym` itself would answer with this
 * runtime's. */
static void *hero_dl_next(const char *name);

static void hero_sig_find(void) {
    if (atomic_load_explicit(&hero_sig_real_action, memory_order_acquire) != NULL) return;
    void *found = hero_dl_next("sigaction");
    if (found == NULL) {
        hero_signal_say("panic: the C library's own sigaction was not found\n");
        _exit(134);
    }
    atomic_store_explicit(&hero_sig_real_action, (hero_sig_action_fn)found, memory_order_release);
}

__attribute__((constructor)) static void hero_sig_find_early(void) { hero_sig_find(); }

/* The C library's own `sigaction`: `parts/sigguard.c`'s delivery probes go
 * through it, never through the definitions below. */
static int hero_sig_real(int sig, const struct sigaction *act, struct sigaction *old) {
    hero_sig_find();
    return atomic_load_explicit(&hero_sig_real_action, memory_order_acquire)(sig, act, old);
}

/* Whether `act` gives the kernel a function to run, rather than a default,
 * an ignore or a hold. */
static bool hero_sig_runs_code(const struct sigaction *act) {
    if ((act->sa_flags & SA_SIGINFO) != 0) return act->sa_sigaction != NULL;
    hero_sig_handler_fn h = act->sa_handler;
    if (h == SIG_DFL || h == SIG_IGN || h == SIG_ERR || h == NULL) return false;
#if defined(SIG_HOLD)
    if (h == SIG_HOLD) return false;
#endif
    return true;
}

static void hero_sig_wrapper(int sig, siginfo_t *info, void *context);

/* The kernel's answer made the program's: where it holds the wrapper, the
 * table's handler and flags stand in its place. */
static void hero_sig_answer(int sig, struct sigaction *old) {
    if ((old->sa_flags & SA_SIGINFO) == 0 || old->sa_sigaction != hero_sig_wrapper) return;
    uintptr_t h = atomic_load_explicit(&hero_sig_handlers[sig], memory_order_acquire);
    int flags = atomic_load_explicit(&hero_sig_flags[sig], memory_order_relaxed);
    old->sa_flags = (old->sa_flags & ~SA_SIGINFO) | (flags & SA_SIGINFO);
    if ((flags & SA_SIGINFO) != 0) {
        old->sa_sigaction = (hero_sig_info_fn)h;
    } else {
        old->sa_handler = h == 0 ? SIG_DFL : (hero_sig_handler_fn)h;
    }
}

/* `parts/sigjump_record.c`: under route A, the pending buffers read where
 * they provably live, at a handler's begin (`x_unseen0`: a refill by a save no
 * slot sees, with no save or jump between, is told by its bytes). */
static void hero_jmp_observe(HeroSignalStack *own);

/* What the kernel runs for every handler a program's C installed: the entry
 * pushed (no prune by this wrapper's own frame, rule 5), the handler called as
 * the program asked, the entry dropped with every entry pushed after it that
 * nothing suspends (`parts/sigentries.c`). The signal fences order the entry
 * before the depth that publishes it, for a nested signal on this thread.
 *
 * THE CALL IS THE KERNEL'S, SO ITS TYPE IS NOT CHECKED: the kernel calls a
 * handler's address whatever C type its program gave it, and a handler a
 * Heroes binding hands over is a thunk typed by the binding (`ptr` for the
 * `siginfo_t *`), which UndefinedBehaviorSanitizer's `function` check called
 * a mismatch under `--sanitize` once this wrapper made the call (Linux arm64,
 * 2026-11-19, `fixedbugs-2494-a-heroes-function-with-siginfo-...`), where the
 * kernel's own call is never checked. */
__attribute__((no_sanitize("function"))) static void hero_sig_wrapper(int sig, siginfo_t *info, void *context) {
    if (sig <= 0 || sig >= NSIG) return;
    uintptr_t h = atomic_load_explicit(&hero_sig_handlers[sig], memory_order_acquire);
    int flags = atomic_load_explicit(&hero_sig_flags[sig], memory_order_relaxed);
    if ((flags & (int)SA_RESETHAND) != 0) atomic_store_explicit(&hero_sig_handlers[sig], 0, memory_order_release);
    if (h == 0) return;
    HeroSignalStack *own = hero_sig_own();
    uint64_t mine = 0;
    bool lost = false;
    if (own != NULL) {
        hero_jmp_observe(own);
        uintptr_t me = (uintptr_t)__builtin_frame_address(0);
        uintptr_t al = 0, ah = 0;
        if (!hero_sig_alt_now(&al, &ah) || me < al || me >= ah) al = ah = 0;
        if (!hero_sig_push(own, me, al, ah, 0, &mine)) {
            own->lost += 1;
            lost = true;
        }
    }
    if ((flags & SA_SIGINFO) != 0) {
        ((hero_sig_info_fn)h)(sig, info, context);
    } else {
        ((hero_sig_handler_fn)h)(sig);
    }
    if (own != NULL) {
        if (mine != 0) {
            for (int i = own->depth - 1; i >= 0; i -= 1) {
                HeroSigEntry *e = &own->entries[i];
                if (e->dead) continue;
                if (e->serial == mine || (e->serial > mine && !hero_sig_pinned(e))) hero_sig_drop(own, i);
            }
        }
        if (lost) own->lost -= 1;
    }
}

int sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict old) {
    if (sig <= 0 || sig >= NSIG) return hero_sig_real(sig, act, old);
    if (act == NULL) {
        int answer = hero_sig_real(sig, NULL, old);
        if (answer == 0 && old != NULL) hero_sig_answer(sig, old);
        return answer;
    }
    struct sigaction before;
    int answer;
    if (!hero_sig_runs_code(act)) {
        answer = hero_sig_real(sig, act, &before);
        if (answer == 0) {
            hero_sig_answer(sig, &before);
            atomic_store_explicit(&hero_sig_handlers[sig], 0, memory_order_release);
        }
    } else {
        uintptr_t h = (act->sa_flags & SA_SIGINFO) != 0 ? (uintptr_t)act->sa_sigaction : (uintptr_t)act->sa_handler;
        struct sigaction mine = *act;
        mine.sa_flags |= SA_SIGINFO;
        mine.sa_sigaction = hero_sig_wrapper;
        uintptr_t was_h = atomic_load_explicit(&hero_sig_handlers[sig], memory_order_acquire);
        int was_flags = atomic_load_explicit(&hero_sig_flags[sig], memory_order_relaxed);
        atomic_store_explicit(&hero_sig_flags[sig], act->sa_flags, memory_order_relaxed);
        atomic_store_explicit(&hero_sig_handlers[sig], h, memory_order_release);
        answer = hero_sig_real(sig, &mine, &before);
        if (answer != 0) {
            atomic_store_explicit(&hero_sig_flags[sig], was_flags, memory_order_relaxed);
            atomic_store_explicit(&hero_sig_handlers[sig], was_h, memory_order_release);
        } else if ((before.sa_flags & SA_SIGINFO) != 0 && before.sa_sigaction == hero_sig_wrapper) {
            before.sa_flags = (before.sa_flags & ~SA_SIGINFO) | (was_flags & SA_SIGINFO);
            if ((was_flags & SA_SIGINFO) != 0) {
                before.sa_sigaction = (hero_sig_info_fn)was_h;
            } else {
                before.sa_handler = was_h == 0 ? SIG_DFL : (hero_sig_handler_fn)was_h;
            }
        }
    }
    if (answer == 0 && old != NULL) *old = before;
    return answer;
}

static bool hero_sig_interrupts(int sig) {
    return sig > 0 && sig < 64 && (atomic_load_explicit(&hero_sig_interrupting, memory_order_relaxed) & (1ull << sig)) != 0;
}

/* glibc 2.41's `__bsd_signal` (`sysdeps/posix/signal.c`), through the
 * definition above: the signal masked in its own handler, `SA_RESTART`
 * unless `siginterrupt` asked otherwise. */
static hero_sig_handler_fn hero_sig_bsd(int sig, hero_sig_handler_fn h) {
    if (h == SIG_ERR || sig <= 0 || sig >= NSIG) {
        errno = EINVAL;
        return SIG_ERR;
    }
    struct sigaction act;
    struct sigaction old;
    memset(&act, 0, sizeof act);
    act.sa_handler = h;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, sig);
    act.sa_flags = hero_sig_interrupts(sig) ? 0 : SA_RESTART;
    if (sigaction(sig, &act, &old) < 0) return SIG_ERR;
    return old.sa_handler;
}

hero_sig_handler_fn signal(int sig, hero_sig_handler_fn h) { return hero_sig_bsd(sig, h); }

int siginterrupt(int sig, int interrupt) {
    struct sigaction act;
    if (sig <= 0 || sig >= NSIG || sigaction(sig, NULL, &act) < 0) return -1;
    if (interrupt) {
        atomic_fetch_or_explicit(&hero_sig_interrupting, 1ull << sig, memory_order_relaxed);
        act.sa_flags &= ~SA_RESTART;
    } else {
        atomic_fetch_and_explicit(&hero_sig_interrupting, ~(1ull << sig), memory_order_relaxed);
        act.sa_flags |= SA_RESTART;
    }
    return sigaction(sig, &act, NULL) < 0 ? -1 : 0;
}

hero_sig_handler_fn bsd_signal(int sig, hero_sig_handler_fn h) { return hero_sig_bsd(sig, h); }

hero_sig_handler_fn ssignal(int sig, hero_sig_handler_fn h) { return hero_sig_bsd(sig, h); }

int __sigaction(int sig, const struct sigaction *act, struct sigaction *old);
int __sigaction(int sig, const struct sigaction *act, struct sigaction *old) { return sigaction(sig, act, old); }

/* glibc's `__sysv_signal`: one-shot, unmasked, interrupting. */
hero_sig_handler_fn __sysv_signal(int sig, hero_sig_handler_fn h);
hero_sig_handler_fn __sysv_signal(int sig, hero_sig_handler_fn h) {
    if (h == SIG_ERR || sig <= 0 || sig >= NSIG) {
        errno = EINVAL;
        return SIG_ERR;
    }
    struct sigaction act;
    struct sigaction old;
    memset(&act, 0, sizeof act);
    act.sa_handler = h;
    sigemptyset(&act.sa_mask);
    act.sa_flags = (int)(SA_RESETHAND | SA_NODEFER);
    if (sigaction(sig, &act, &old) < 0) return SIG_ERR;
    return old.sa_handler;
}

hero_sig_handler_fn sysv_signal(int sig, hero_sig_handler_fn h) { return __sysv_signal(sig, h); }

/* glibc's `sigset` (`sysdeps/posix/sigset.c`). */
hero_sig_handler_fn sigset(int sig, hero_sig_handler_fn disp) {
    if (disp == SIG_ERR || sig <= 0 || sig >= NSIG) {
        errno = EINVAL;
        return SIG_ERR;
    }
    sigset_t one;
    sigset_t was;
    struct sigaction act;
    struct sigaction old;
    sigemptyset(&one);
    sigaddset(&one, sig);
    if (disp == SIG_HOLD) {
        if (sigprocmask(SIG_BLOCK, &one, &was) < 0) return SIG_ERR;
        if (sigismember(&was, sig)) return SIG_HOLD;
        if (sigaction(sig, NULL, &old) < 0) return SIG_ERR;
        return old.sa_handler;
    }
    memset(&act, 0, sizeof act);
    act.sa_handler = disp;
    sigemptyset(&act.sa_mask);
    if (sigaction(sig, &act, &old) < 0) return SIG_ERR;
    if (sigprocmask(SIG_UNBLOCK, &one, &was) < 0) return SIG_ERR;
    return sigismember(&was, sig) ? SIG_HOLD : old.sa_handler;
}

int sigignore(int sig) {
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    return sigaction(sig, &act, NULL);
}

#endif
