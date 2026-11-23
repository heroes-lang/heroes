/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind_install.c — Darwin's route D-rebind, part of `parts/rebind.c`
 * (read its module doc first): the C library's own, taken before `main`, and
 * the installs that put the trampoline in every handler (panel 255's R1,
 * defects 2679 and 2702; design.md §1.12, §4.19). Needs the files `rebind.c`
 * names before it. */

#if defined(__APPLE__)

typedef int (*hero_rebind_action_fn)(int, const struct sigaction *, struct sigaction *);
typedef void (*(*hero_rebind_signal_fn)(int, void (*)(int)))(int);
typedef void (*hero_rebind_jump_fn)(int *, int);
typedef int (*hero_rebind_save_fn)(int *);
typedef int (*hero_rebind_sigsave_fn)(int *, int);
typedef int (*hero_rebind_get_fn)(void *);
typedef int (*hero_rebind_set_fn)(const void *);
typedef int (*hero_rebind_swap_fn)(void *, const void *);

static _Atomic(hero_rebind_action_fn) hero_rebind_real_sigaction = NULL;
static _Atomic(hero_rebind_signal_fn) hero_rebind_real_signal = NULL;
static _Atomic(hero_rebind_jump_fn) hero_rebind_real_siglongjmp = NULL;
static _Atomic(hero_rebind_jump_fn) hero_rebind_real_longjmp = NULL;
static _Atomic(hero_rebind_jump_fn) hero_rebind_real__longjmp = NULL;
static _Atomic(hero_rebind_save_fn) hero_rebind_real_setjmp = NULL;
static _Atomic(hero_rebind_save_fn) hero_rebind_real__setjmp = NULL;
static _Atomic(hero_rebind_sigsave_fn) hero_rebind_real_sigsetjmp = NULL;
static _Atomic(hero_rebind_get_fn) hero_rebind_real_getcontext = NULL;
static _Atomic(hero_rebind_set_fn) hero_rebind_real_setcontext = NULL;
static _Atomic(hero_rebind_swap_fn) hero_rebind_real_swapcontext = NULL;

typedef void *(*hero_rebind_dlsym_fn)(void *, const char *);
typedef int (*hero_rebind_vec_fn)(int, void *, void *);
typedef int (*hero_rebind_interrupt_fn)(int, int);

static _Atomic(hero_rebind_dlsym_fn) hero_rebind_real_dlsym = NULL;
static _Atomic(hero_rebind_signal_fn) hero_rebind_real_sigset = NULL;
static _Atomic(hero_rebind_signal_fn) hero_rebind_real_bsd_signal = NULL;
static _Atomic(hero_rebind_vec_fn) hero_rebind_real_sigvec = NULL;
static _Atomic(hero_rebind_interrupt_fn) hero_rebind_real_siginterrupt = NULL;


/* -- installs ---------------------------------------------------------- */

/* The kernel's own action for `sig`, asked raw, given back with this file's
 * trampoline where it runs a function; a default or an ignore is left as it
 * is. */
static void hero_rebind_retramp(int sig) {
    struct sigaction now;
    if (sig <= 0 || sig >= NSIG || __sigaction(sig, NULL, &now) != 0) return;
    if (now.sa_handler == SIG_DFL || now.sa_handler == SIG_IGN) return;
    struct __sigaction mine;
    memset(&mine, 0, sizeof mine);
    mine.__sigaction_u.__sa_handler = now.sa_handler;
    mine.sa_tramp = (void (*)(void *, int, int, siginfo_t *, void *))(void *)hero_rebind_tramp;
    mine.sa_mask = now.sa_mask;
    mine.sa_flags = now.sa_flags | HERO_REBIND_TOKEN_CHECKED;
    (void)__sigaction(sig, &mine, NULL);
}

static int hero_rebind_sigaction(int sig, const struct sigaction *act, struct sigaction *old) {
    int answer = atomic_load_explicit(&hero_rebind_real_sigaction, memory_order_relaxed)(sig, act, old);
    if (answer == 0 && act != NULL) hero_rebind_retramp(sig);
    return answer;
}

static void (*hero_rebind_signal(int sig, void (*handler)(int)))(int) {
    void (*before)(int) = atomic_load_explicit(&hero_rebind_real_signal, memory_order_relaxed)(sig, handler);
    hero_rebind_retramp(sig);
    return before;
}

/* THE C LIBRARY'S OTHER INSTALLERS, TRAMPOLINED AGAIN ONCE THEY RETURN
 * (defect 2522's row 3, the author's ruling C of 2026-11-20). `sigset`,
 * `bsd_signal`, `sigvec` and `siginterrupt` are libsystem_c's, and each
 * installs through libsystem_c's own `sigaction`, a call inside the shared
 * cache no rewrite reaches, so the kernel was handed libSystem's trampoline:
 * a handler any of them installed, or that `siginterrupt` re-installed after
 * `signal` had been trampolined, called the program's function through 24
 * frames, which ran inside the signal, 20 runs of 20 at `-O0`, `-O2` and
 * `--sanitize` on this Mac (lane-signals-2, 2026-11-20). The C library's own
 * call is made, and then the kernel's action is trampolined again, as an
 * install through `sigaction` is. A `struct sigvec` is handed on whole, never
 * read here. */
static void (*hero_rebind_sigset(int sig, void (*handler)(int)))(int) {
    void (*before)(int) = atomic_load_explicit(&hero_rebind_real_sigset, memory_order_relaxed)(sig, handler);
    hero_rebind_retramp(sig);
    return before;
}

static void (*hero_rebind_bsd_signal(int sig, void (*handler)(int)))(int) {
    void (*before)(int) = atomic_load_explicit(&hero_rebind_real_bsd_signal, memory_order_relaxed)(sig, handler);
    hero_rebind_retramp(sig);
    return before;
}

static int hero_rebind_sigvec(int sig, void *vec, void *old) {
    int answer = atomic_load_explicit(&hero_rebind_real_sigvec, memory_order_relaxed)(sig, vec, old);
    if (answer == 0 && vec != NULL) hero_rebind_retramp(sig);
    return answer;
}

static int hero_rebind_siginterrupt(int sig, int flag) {
    int answer = atomic_load_explicit(&hero_rebind_real_siginterrupt, memory_order_relaxed)(sig, flag);
    if (answer == 0) hero_rebind_retramp(sig);
    return answer;
}

#endif
