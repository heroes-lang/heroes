/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind.c — every handler a program's C installs entered through this
 * runtime's trampoline, and a jump out of a handler told from a jump within
 * it, on Darwin (route D-rebind, panel 249's ffi-pragmatist, repaired for
 * `--sanitize` and given route J's validated record by panel 251's R1,
 * ratified 2026-11-19; defect 2522; design.md §1.12, §4.19).
 *
 * WHY NOT `parts/siginstall.c`'s WAY. Darwin's two-level namespace binds a
 * library's call of `sigaction` to libSystem's own name, so a definition of
 * the runtime's reaches the program's own objects and no dylib: panel 249
 * measured every Darwin dylib missed, 9 of 9.
 *
 * HOW. Before `main`, and for every image dyld loads later (its add-image
 * callback), the EXECUTABLE rewrites, in every loaded image outside the shared
 * cache, the pointers its stubs load for `sigaction` and `signal`, and for the
 * setjmp and context families below, to functions of this file. An install
 * reaches `hero_rebind_sigaction`: the C library's own install, then the same
 * action handed to libsystem_kernel's `__sigaction` with this file's
 * trampoline, so the kernel enters the handler through `hero_rebind_tramp`,
 * which keeps the entries this thread is inside. The thunk asks whether any
 * stands (`hero_rebind_inside`, `parts/sigguard.c`). The runtime's own calls
 * are rewritten too, so `parts/sigguard.c`'s delivery probe learns this
 * trampoline's call as the delivery's.
 *
 * THE TRAMPOLINE IS libSystem's `_sigtramp`'S CONTRACT (panel 249's
 * ffi-pragmatist, `docs/panel/249-reports/ffi-pragmatist.md` § 3.1, read in
 * `_sigtramp`'s disassembly and run): the kernel calls it with the catcher,
 * the style, the signal, its information, the context and a token; it calls
 * the catcher with three arguments whatever the style, and returns by
 * `__sigreturn(context, 30, token)`. libSystem installs every handler with the
 * flag 0x400, which no SDK header names, and with it the kernel checks the
 * token, so the token is handed on. Both values are the kernel's interface,
 * named below and measured, never read from a header.
 *
 * ROUTE J, VALIDATED. The save points' slots (`setjmp`, `_setjmp`,
 * `sigsetjmp`, `getcontext`, the save half of `swapcontext`) record the
 * buffer, the thread and the innermost entry live at the save, then reach the
 * C library's own by a tail call, so it saves the caller; `makecontext`
 * forgets its context's record, the context now naming another stack. Each
 * jump's slot (`longjmp`, `_longjmp`, `siglongjmp`, `setcontext`, the jump
 * half of `swapcontext`) restores the entries recorded for its target, which
 * on one stack are exactly the entries shallower than it, and on two (an
 * alternate signal stack, a context) still the ones live at the save. The
 * entry, never the frame, because Darwin's `setjmp` mangles the saved stack
 * pointer. A record's buffer is fingerprinted at the thread's next save point
 * or jump, once the C library has filled it; a jump whose buffer no longer
 * reads as its fingerprint has no record (panel 251's critic, carrying the
 * compiler-engineer's validation here: `ijstale.c` ran the Heroes function
 * inside the signal 40 of 40 under the record as first built, and is refused
 * 40 of 40 under this one). A JUMP TO A BUFFER WITH NO RECORD KEEPS EVERY
 * ENTRY: a miss is a told refusal, never Heroes code inside a signal.
 *
 * THE `--sanitize` REPAIR (panel 251's ffi-pragmatist): an image holding an
 * `__interpose` section (ASan's and TSan's runtimes, Guard Malloc, any
 * `DYLD_INSERT_LIBRARIES` tool) is left unrewritten. Such an image calls the C
 * library's names on purpose, and the pointers this file takes by `dlsym` ARE
 * its wrappers (dyld answers a lookup with the interposer, even by handle), so
 * rewriting its slots made `sigaction` call itself until the stack ended,
 * every `--sanitize` program dead before `main`. And a re-install asks the
 * kernel raw (`__sigaction`), never `sigaction`: TSan answers `sigaction`
 * with the program's handler and keeps its own with the kernel, and wrapping
 * that answer displaced TSan's.
 *
 * EVERY POINTER TO THE C LIBRARY'S OWN IS TAKEN BEFORE `main`; nothing below
 * locks, allocates or calls `dlsym`, so each rewritten slot is
 * async-signal-safe, in a handler and in a forked child alike.
 *
 * `dlsym` AND THE C LIBRARY'S OTHER INSTALLERS (defect 2522's row 3, the
 * author's ruling C of 2026-11-20). The `dlsym` slot is rewritten too, and
 * answers this file's function for every name the table rewrites, so an
 * image that asks for `sigaction` by `dlsym` installs as one that calls it;
 * and `sigset`, `bsd_signal`, `sigvec` and `siginterrupt`, libsystem_c's,
 * which install through libsystem_c's own `sigaction` inside the shared
 * cache, have the kernel's action trampolined again once they return.
 *
 * WHAT IT DOES NOT REACH, said (defect 2522's rows): an installer or a save
 * point reached by a `dlsym` made inside the shared cache, or an install made
 * there on the cache's own account (whose binds are not rewritable; 52 cache
 * images import a save point, 51 `sigaction`); a raw
 * `__sigaction` naming a trampoline of its own; an interposer's own installs;
 * a record table of HERO_REBIND_RECORDS past its probes (a miss); a C++
 * exception out of a handler; whether dyld keeps the slots writable on the
 * next macOS (the CI's `macos-26`, and arm64e third-party slots, owed). */

#if defined(__APPLE__)

#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach/mach.h>
#include <setjmp.h>

/* libsystem_kernel's own, which no SDK header declares. */
extern int __sigaction(int, struct __sigaction *, struct sigaction *);
extern int __sigreturn(void *, int, uintptr_t);

/* The kernel's values `_sigtramp` uses (above): the install's flag under which
 * the kernel checks a handler's return token, and `__sigreturn`'s flavor. */
#define HERO_REBIND_TOKEN_CHECKED 0x400
#define HERO_REBIND_RETURN_FLAVOR 30

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

/* THE STACKS `makecontext` HANDS OUT (panel 255, C3): each context's stack,
 * read from the context the program hands `makecontext` (it lives: the C
 * library reads the same two fields next), kept until a later one overlaps
 * it. Two live stacks never share an address, so a range a later stack
 * overlaps is dropped, and an address inside a kept range lies on that stack.
 * Written under a spin flag (`makecontext` is no signal-safe call), read
 * slot by slot under a sequence word: a slot read while written is a stack
 * of unknown extent, the conservative answer. */
#define HERO_REBIND_STACKS 256
static _Atomic unsigned hero_rebind_stack_seq[HERO_REBIND_STACKS];
static _Atomic uintptr_t hero_rebind_stack_lo[HERO_REBIND_STACKS];
static _Atomic uintptr_t hero_rebind_stack_hi[HERO_REBIND_STACKS];
static _Atomic unsigned hero_rebind_stack_next = 0;
static atomic_flag hero_rebind_stack_writing = ATOMIC_FLAG_INIT;

static void hero_rebind_stack_put(int i, uintptr_t lo, uintptr_t hi) {
    atomic_fetch_add(&hero_rebind_stack_seq[i], 1);
    atomic_store(&hero_rebind_stack_lo[i], lo);
    atomic_store(&hero_rebind_stack_hi[i], hi);
    atomic_fetch_add(&hero_rebind_stack_seq[i], 1);
}

void hero_rebind_known_stack(const void *context);
void hero_rebind_known_stack(const void *context) {
    const ucontext_t *uc = (const ucontext_t *)context;
    uintptr_t lo = (uintptr_t)uc->uc_stack.ss_sp;
    uintptr_t hi = lo + (uintptr_t)uc->uc_stack.ss_size;
    if (lo == 0 || hi <= lo) return;
    while (atomic_flag_test_and_set(&hero_rebind_stack_writing)) {
    }
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        uintptr_t l = atomic_load(&hero_rebind_stack_lo[i]), h = atomic_load(&hero_rebind_stack_hi[i]);
        if (h > l && l < hi && lo < h) hero_rebind_stack_put(i, 0, 0);
    }
    unsigned at = atomic_fetch_add(&hero_rebind_stack_next, 1) % HERO_REBIND_STACKS;
    hero_rebind_stack_put((int)at, lo, hi);
    atomic_flag_clear(&hero_rebind_stack_writing);
}

/* Which stack an address lies on: the thread's own (0), the alternate stack
 * registered now (1), a stack `makecontext` handed out (3 + its slot, read
 * whole), or another of unknown extent (2), which no address proves left. */
#define HERO_REBIND_OWN 0
#define HERO_REBIND_ALT 1
#define HERO_REBIND_OTHER 2
#define HERO_REBIND_KNOWN 3

static int hero_rebind_stack_of(const HeroSignalStack *own, uintptr_t at) {
    if (at >= own->lo && at < own->hi) return HERO_REBIND_OWN;
    stack_t alt;
    if (sigaltstack(NULL, &alt) == 0 && (alt.ss_flags & SS_DISABLE) == 0 && at >= (uintptr_t)alt.ss_sp &&
        at < (uintptr_t)alt.ss_sp + alt.ss_size) {
        return HERO_REBIND_ALT;
    }
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        unsigned before = atomic_load(&hero_rebind_stack_seq[i]);
        uintptr_t l = atomic_load(&hero_rebind_stack_lo[i]), h = atomic_load(&hero_rebind_stack_hi[i]);
        if ((before & 1) != 0 || atomic_load(&hero_rebind_stack_seq[i]) != before) continue;
        if (at >= l && at < h) return HERO_REBIND_KNOWN + i;
    }
    return HERO_REBIND_OTHER;
}

/* The entries, from the innermost, `at` proves left: one deeper than `at` on
 * the same known stack (that stack has unwound past it); and, at a `longjmp`
 * alone, one on the alternate stack while `at` is on the thread's own (a jump
 * leaves the alternate stack's frames dead, where a context swap only
 * suspends them). An entry on a stack whose extent is unknown is never proved
 * left by an address: it stands, a told refusal. (Panel 255: `hero_sig_prune`'s
 * rule as first carried here let a coroutine call the program while the
 * handler that swapped to it was suspended, `q5/ctx2` how 2.) */
static void hero_rebind_prune(HeroSignalStack *own, uintptr_t at, bool by_longjmp) {
    int here = hero_rebind_stack_of(own, at);
    while (own->depth > 0) {
        uintptr_t w = own->entries[own->depth - 1];
        int there = hero_rebind_stack_of(own, w);
        bool left = (there == here && there != HERO_REBIND_OTHER && w <= at) ||
                    (by_longjmp && there == HERO_REBIND_ALT && here == HERO_REBIND_OWN);
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
        hero_rebind_prune(own, me, false);
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

/* The thunk's question (`parts/sigguard.c`): the entries this frame proves
 * left dropped, then any entry still standing refuses, on whatever stack (a
 * handler suspended by a context swap is still handling its signal), told as
 * unsure where a jump this runtime could not follow passed over it. */
__attribute__((noinline)) static int hero_rebind_inside(void) {
    HeroSignalStack *own = hero_rebind_own();
    uintptr_t frame = (uintptr_t)__builtin_frame_address(0);
    if (own == NULL || own->depth == 0) return HERO_SIG_OUTSIDE;
    hero_rebind_prune(own, frame, false);
    if (own->depth == 0) return HERO_SIG_OUTSIDE;
    return own->depth - 1 < own->unsure ? HERO_SIG_UNSURE : HERO_SIG_INSIDE;
}

/* -- the C library's own, taken before `main` -------------------------- */

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

/* -- route C: the target's stack pointer, decoded ------------------------ */

/* Darwin arm64's `_setjmp` (libsystem_platform, read in `dyld_info
 * -disassemble`): word 12 is `pacdb(sp, 0xcbed) ^ k`, `k` the process's
 * pointer-munge token; a context's stack pointer is its machine context's
 * `__ss.__sp`, `pacda(sp, 0xcbed)`, at byte 0x148 where `setcontext` reads it.
 * `xpacd` strips a signature where one stands (an arm64e process) and leaves
 * the pointer where none does (arm64, measured: the data keys sign nothing
 * there). `k` is taken from one save at a known stack pointer and verified
 * before `main` at two more stack pointers 1 MiB apart and on a second stack,
 * for each of the four saves and `getcontext`; where any check fails nothing
 * is decoded and every jump inside a handler keeps every entry, told. */
#define HERO_REBIND_SP_WORD 12
#define HERO_REBIND_CTX_SP 0x148
static uintptr_t hero_rebind_key_k = 0;
static _Atomic int hero_rebind_decodes = 0;

#if defined(__aarch64__)
static inline uintptr_t hero_rebind_strip(uintptr_t p) {
    __asm__("xpacd %0" : "+r"(p));
    return p;
}

/* The stack pointer a save is handed, stored by the stub that hands it. */
uintptr_t hero_rebind_cal_sp = 0;

/* Stores the stack pointer, then branches to `fn` with the caller's frame
 * untouched, so `fn` saves the stack pointer stored. */
__attribute__((naked)) static int hero_rebind_cal_call(void *buf, int mask, void *fn) {
    __asm__ volatile("mov x9, sp\n\t"
                     "adrp x10, _hero_rebind_cal_sp@GOTPAGE\n\t"
                     "ldr x10, [x10, _hero_rebind_cal_sp@GOTPAGEOFF]\n\t"
                     "str x9, [x10]\n\t"
                     "br x2");
}

/* Runs `fn` on the stack whose top is `top`. */
__attribute__((naked)) static void hero_rebind_on_stack(void *top, void (*fn)(void)) {
    __asm__ volatile("stp x29, x30, [sp, #-16]!\n\t"
                     "mov x29, sp\n\t"
                     "mov x9, sp\n\t"
                     "mov sp, x0\n\t"
                     "stp x9, x10, [sp, #-16]!\n\t"
                     "blr x1\n\t"
                     "ldp x9, x10, [sp], #16\n\t"
                     "mov sp, x9\n\t"
                     "ldp x29, x30, [sp], #16\n\t"
                     "ret");
}

static _Atomic int hero_rebind_cal_bad = 0;

/* Each of the four saves and `getcontext` at this stack pointer, decoded. */
__attribute__((noinline, no_sanitize("address"))) static void hero_rebind_cal_check(void) {
    _Alignas(16) unsigned char buf[1024];
    void *saves[4] = {(void *)atomic_load(&hero_rebind_real_setjmp), (void *)atomic_load(&hero_rebind_real__setjmp),
                      (void *)atomic_load(&hero_rebind_real_sigsetjmp), (void *)atomic_load(&hero_rebind_real_sigsetjmp)};
    for (int i = 0; i < 4; i += 1) {
        uintptr_t word;
        memset(buf, 0xA5, sizeof buf);
        if (saves[i] == NULL) {
            atomic_store(&hero_rebind_cal_bad, 1);
            continue;
        }
        (void)hero_rebind_cal_call(buf, i == 3, saves[i]);
        memcpy(&word, buf + HERO_REBIND_SP_WORD * sizeof(uintptr_t), sizeof word);
        if (hero_rebind_strip(word ^ hero_rebind_key_k) != hero_rebind_cal_sp) atomic_store(&hero_rebind_cal_bad, 1);
    }
    void *get = (void *)atomic_load(&hero_rebind_real_getcontext);
    if (get != NULL) {
        uintptr_t word;
        memset(buf, 0xA5, sizeof buf);
        (void)hero_rebind_cal_call(buf, 0, get);
        memcpy(&word, buf + HERO_REBIND_CTX_SP, sizeof word);
        if (hero_rebind_strip(word) != hero_rebind_cal_sp) atomic_store(&hero_rebind_cal_bad, 1);
    }
}

__attribute__((noinline, no_sanitize("address"))) static void hero_rebind_cal_deep(int n) {
    volatile unsigned char pad[64 * 1024];
    pad[0] = (unsigned char)n;
    if (n > 0) {
        hero_rebind_cal_deep(n - 1);
    } else {
        hero_rebind_cal_check();
    }
    (void)pad[0];
}

__attribute__((no_sanitize("address"))) static void hero_rebind_calibrate(void) {
    _Alignas(16) unsigned char buf[256];
    void *save = (void *)atomic_load(&hero_rebind_real__setjmp);
    if (save == NULL) return;
    memset(buf, 0xA5, sizeof buf);
    (void)hero_rebind_cal_call(buf, 0, save);
    uintptr_t word;
    memcpy(&word, buf + HERO_REBIND_SP_WORD * sizeof(uintptr_t), sizeof word);
    hero_rebind_key_k = word ^ hero_rebind_cal_sp;
    hero_rebind_cal_check();
    hero_rebind_cal_deep(16);
    size_t size = (size_t)1 << 18;
    void *stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (stack == MAP_FAILED) return;
    hero_rebind_on_stack((unsigned char *)stack + size, hero_rebind_cal_check);
    munmap(stack, size);
    if (!atomic_load(&hero_rebind_cal_bad)) atomic_store_explicit(&hero_rebind_decodes, 1, memory_order_release);
}
#else
static inline uintptr_t hero_rebind_strip(uintptr_t p) { return p; }
static void hero_rebind_calibrate(void) {}
#endif

/* A jump whose target's stack pointer is `sp`: outside every handler nothing
 * read; else the entries it proves left dropped; where nothing decodes, every
 * entry kept and marked as one this jump may have left. */
static void hero_rebind_leave(const void *target, size_t at, uintptr_t key, bool by_longjmp) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || own->depth == 0) return;
    if (!atomic_load_explicit(&hero_rebind_decodes, memory_order_acquire)) {
        own->unsure = own->depth;
        return;
    }
    uintptr_t word;
    memcpy(&word, (const unsigned char *)target + at, sizeof word);
    hero_rebind_prune(own, hero_rebind_strip(word ^ key), by_longjmp);
}

static void hero_rebind_jump(const void *env) {
    hero_rebind_leave(env, HERO_REBIND_SP_WORD * sizeof(uintptr_t), hero_rebind_key_k, true);
}

static void hero_rebind_jump_context(const void *context) { hero_rebind_leave(context, HERO_REBIND_CTX_SP, 0, false); }

/* `makecontext` names its context's stack, which becomes a known one. Its
 * arguments are variadic, which `musttail` refuses, so the wrapper is today's
 * assembly: the argument registers kept, the stack noted, then a branch to
 * the C library's own with the stack as the caller left it. */
void (*hero_rebind_real_makecontext)(void *, void (*)(void), int, ...) = NULL;

#if defined(__aarch64__)
__attribute__((naked)) static void hero_rebind_makecontext(void *context, void (*start)(void), int count, ...) {
    __asm__ volatile("stp x29, x30, [sp, #-48]!\n\t"
                     "mov x29, sp\n\t"
                     "stp x0, x1, [sp, #16]\n\t"
                     "str x2, [sp, #32]\n\t"
                     "bl _hero_rebind_known_stack\n\t"
                     "ldp x0, x1, [sp, #16]\n\t"
                     "ldr x2, [sp, #32]\n\t"
                     "ldp x29, x30, [sp], #48\n\t"
                     "adrp x16, _hero_rebind_real_makecontext@GOTPAGE\n\t"
                     "ldr x16, [x16, _hero_rebind_real_makecontext@GOTPAGEOFF]\n\t"
                     "ldr x16, [x16]\n\t"
                     "br x16");
}
#elif defined(__x86_64__)
__attribute__((naked)) static void hero_rebind_makecontext(void *context, void (*start)(void), int count, ...) {
    __asm__ volatile("push %rbp\n\tmov %rsp, %rbp\n\t"
                     "push %rdi\n\tpush %rsi\n\tpush %rdx\n\tpush %rcx\n\tpush %r8\n\tpush %r9\n\tpush %rax\n\t"
                     "sub $8, %rsp\n\t"
                     "call _hero_rebind_known_stack\n\t"
                     "add $8, %rsp\n\t"
                     "pop %rax\n\tpop %r9\n\tpop %r8\n\tpop %rcx\n\tpop %rdx\n\tpop %rsi\n\tpop %rdi\n\tpop %rbp\n\t"
                     "mov _hero_rebind_real_makecontext@GOTPCREL(%rip), %r11\n\t"
                     "jmp *(%r11)");
}
#else
#error parts/rebind.c names no makecontext wrapper for this machine
#endif

/* -- the jumps ------------------------------------------------------------ */

static int hero_rebind_swapcontext(void *from, const void *to) {
    hero_rebind_jump_context(to);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_swapcontext, memory_order_relaxed)(from,
                                                                                                                to);
}

__attribute__((noreturn)) static void hero_rebind_siglongjmp(int *env, int value) {
    hero_rebind_jump(env);
    atomic_load_explicit(&hero_rebind_real_siglongjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn)) static void hero_rebind_longjmp(int *env, int value) {
    hero_rebind_jump(env);
    atomic_load_explicit(&hero_rebind_real_longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn)) static void hero_rebind__longjmp(int *env, int value) {
    hero_rebind_jump(env);
    atomic_load_explicit(&hero_rebind_real__longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

static int hero_rebind_setcontext(const void *context) {
    hero_rebind_jump_context(context);
    return atomic_load_explicit(&hero_rebind_real_setcontext, memory_order_relaxed)(context);
}

/* -- the rewrite of an image's slots ------------------------------------ */

static void *hero_rebind_dlsym(void *handle, const char *name);

/* The functions a slot bound to a name (Mach-O's spelling, a leading `_`) is
 * given instead. */
static const struct {
    const char *name;
    void *with;
} hero_rebind_table[] = {
    {"_sigaction", (void *)hero_rebind_sigaction},
    {"_signal", (void *)hero_rebind_signal},
    {"_sigset", (void *)hero_rebind_sigset},
    {"_bsd_signal", (void *)hero_rebind_bsd_signal},
    {"_sigvec", (void *)hero_rebind_sigvec},
    {"_siginterrupt", (void *)hero_rebind_siginterrupt},
    {"_dlsym", (void *)hero_rebind_dlsym},
    {"_siglongjmp", (void *)hero_rebind_siglongjmp},
    {"_longjmp", (void *)hero_rebind_longjmp},
    {"__longjmp", (void *)hero_rebind__longjmp},
    {"_setcontext", (void *)hero_rebind_setcontext},
    {"_swapcontext", (void *)hero_rebind_swapcontext},
    {"_makecontext", (void *)hero_rebind_makecontext},
};

/* The function a slot bound to `name`, Mach-O's spelling, is given instead,
 * or NULL for a name this file leaves alone. */
static void *hero_rebind_replacement(const char *name) {
    for (size_t i = 0; i < sizeof hero_rebind_table / sizeof hero_rebind_table[0]; i += 1) {
        if (strcmp(name, hero_rebind_table[i].name) == 0) return hero_rebind_table[i].with;
    }
    return NULL;
}

/* The same, for C's spelling, the one `dlsym` is asked with. */
static void *hero_rebind_answer(const char *name) {
    for (size_t i = 0; i < sizeof hero_rebind_table / sizeof hero_rebind_table[0]; i += 1) {
        if (strcmp(name, hero_rebind_table[i].name + 1) == 0) return hero_rebind_table[i].with;
    }
    return NULL;
}

/* Whether the image `caller` lies in exports `name` itself, asked of that
 * image alone (`RTLD_FIRST`): an image asking for the definition after its
 * own, or for its own. */
static bool hero_rebind_caller_defines(const void *caller, const char *name, hero_rebind_dlsym_fn real) {
    Dl_info info;
    if (dladdr(caller, &info) == 0 || info.dli_fname == NULL) return false;
    void *self = dlopen(info.dli_fname, RTLD_NOLOAD | RTLD_FIRST);
    if (self == NULL) return false;
    void *own = real(self, name);
    dlclose(self);
    return own != NULL;
}

/* `dlsym` ANSWERED FOR THE NAMES ABOVE (defect 2522's row 3, the author's
 * ruling C of 2026-11-20). An image's `dlsym` of `sigaction`, by the C
 * library's handle, `RTLD_NEXT` or `RTLD_DEFAULT`, answered the C library's
 * own, never the rewritten slot, and a handler it installed called the
 * program's function through 24 frames, which ran inside the signal, 20 runs
 * of 20 at `-O0`, `-O2` and `--sanitize` on this Mac (lane-signals-2,
 * 2026-11-20). So every name the table rewrites is answered this file's
 * function, which is what the image's slot for it reaches, wherever the C
 * library would have answered one at all. Handed to the C library's `dlsym`
 * by a tail call, so it sees its caller as `RTLD_NEXT` and `RTLD_SELF` need:
 * every other name, and a lookup made by an image that exports the name
 * itself. An image that interposes is never rewritten, so its lookups reach
 * the C library's `dlsym` as they did. */
static void *hero_rebind_dlsym(void *handle, const char *name) {
    hero_rebind_dlsym_fn real = atomic_load_explicit(&hero_rebind_real_dlsym, memory_order_relaxed);
    void *ours = name != NULL ? hero_rebind_answer(name) : NULL;
    if (ours == NULL || hero_rebind_caller_defines(__builtin_return_address(0), name, real)) {
        __attribute__((musttail)) return real(handle, name);
    }
    return real(handle, name) != NULL ? ours : NULL;
}

/* Whether an image interposes (an `__interpose` section in any segment): its
 * calls of the C library's names are its own wrappers' way to the C library,
 * and are left as they are (the `--sanitize` repair, above). */
static bool hero_rebind_interposes(const struct mach_header_64 *mh) {
    const uint8_t *p = (const uint8_t *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i += 1) {
        const struct load_command *lc = (const struct load_command *)p;
        if (lc->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *seg = (const struct segment_command_64 *)lc;
            const struct section_64 *sec = (const struct section_64 *)(seg + 1);
            for (uint32_t j = 0; j < seg->nsects; j += 1) {
                if (strncmp(sec[j].sectname, "__interpose", 16) == 0) return true;
            }
        }
        p += lc->cmdsize;
    }
    return false;
}

/* Every lazy and non-lazy symbol pointer of the image whose symbol this file
 * replaces, rewritten, its page made writable by a copy first. An image of
 * the shared cache is left alone: its binds are not rewritable. */
static void hero_rebind_image(const struct mach_header *header, intptr_t slide) {
    const struct mach_header_64 *mh = (const struct mach_header_64 *)header;
    if ((mh->flags & MH_DYLIB_IN_CACHE) != 0 || hero_rebind_interposes(mh)) return;
    const struct segment_command_64 *linkedit = NULL;
    const struct symtab_command *st = NULL;
    const struct dysymtab_command *dy = NULL;
    const uint8_t *p = (const uint8_t *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i += 1) {
        const struct load_command *lc = (const struct load_command *)p;
        if (lc->cmd == LC_SEGMENT_64 && strcmp(((const struct segment_command_64 *)lc)->segname, SEG_LINKEDIT) == 0) {
            linkedit = (const struct segment_command_64 *)lc;
        } else if (lc->cmd == LC_SYMTAB) {
            st = (const struct symtab_command *)lc;
        } else if (lc->cmd == LC_DYSYMTAB) {
            dy = (const struct dysymtab_command *)lc;
        }
        p += lc->cmdsize;
    }
    if (linkedit == NULL || st == NULL || dy == NULL || dy->nindirectsyms == 0) return;
    uintptr_t base = (uintptr_t)slide + (uintptr_t)linkedit->vmaddr - (uintptr_t)linkedit->fileoff;
    const struct nlist_64 *syms = (const struct nlist_64 *)(base + st->symoff);
    const char *strs = (const char *)(base + st->stroff);
    const uint32_t *indirect = (const uint32_t *)(base + dy->indirectsymoff);
    p = (const uint8_t *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i += 1) {
        const struct load_command *lc = (const struct load_command *)p;
        p += lc->cmdsize;
        if (lc->cmd != LC_SEGMENT_64) continue;
        const struct segment_command_64 *seg = (const struct segment_command_64 *)lc;
        const struct section_64 *sec = (const struct section_64 *)(seg + 1);
        for (uint32_t j = 0; j < seg->nsects; j += 1, sec += 1) {
            uint32_t type = sec->flags & SECTION_TYPE;
            if (type != S_LAZY_SYMBOL_POINTERS && type != S_NON_LAZY_SYMBOL_POINTERS) continue;
            void **slots = (void **)((uintptr_t)slide + (uintptr_t)sec->addr);
            uint32_t n = (uint32_t)(sec->size / sizeof(void *));
            for (uint32_t k = 0; k < n; k += 1) {
                uint32_t si = indirect[sec->reserved1 + k];
                if ((si & (INDIRECT_SYMBOL_ABS | INDIRECT_SYMBOL_LOCAL)) != 0) continue;
                void *with = hero_rebind_replacement(strs + syms[si].n_un.n_strx);
                if (with == NULL) continue;
                (void)vm_protect(mach_task_self(), (vm_address_t)&slots[k], sizeof(void *), 0,
                                 VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
                slots[k] = with;
            }
        }
    }
}

/* -- before `main` ------------------------------------------------------- */

/* Priority 0, before every constructor of the program's own: the keys made,
 * the C library's own taken (by `RTLD_DEFAULT`, so under an interposer its
 * wrapper, which reaches the C library), every image rewritten now and every
 * one dyld loads later, and every handler already installed trampolined. */
__attribute__((constructor(0))) static void hero_rebind_start(void) {
    atomic_store(&hero_rebind_real_sigaction, (hero_rebind_action_fn)dlsym(RTLD_DEFAULT, "sigaction"));
    atomic_store(&hero_rebind_real_signal, (hero_rebind_signal_fn)dlsym(RTLD_DEFAULT, "signal"));
    atomic_store(&hero_rebind_real_sigset, (hero_rebind_signal_fn)dlsym(RTLD_DEFAULT, "sigset"));
    atomic_store(&hero_rebind_real_bsd_signal, (hero_rebind_signal_fn)dlsym(RTLD_DEFAULT, "bsd_signal"));
    atomic_store(&hero_rebind_real_sigvec, (hero_rebind_vec_fn)dlsym(RTLD_DEFAULT, "sigvec"));
    atomic_store(&hero_rebind_real_siginterrupt, (hero_rebind_interrupt_fn)dlsym(RTLD_DEFAULT, "siginterrupt"));
    atomic_store(&hero_rebind_real_dlsym, (hero_rebind_dlsym_fn)dlsym(RTLD_DEFAULT, "dlsym"));
    atomic_store(&hero_rebind_real_siglongjmp, (hero_rebind_jump_fn)dlsym(RTLD_DEFAULT, "siglongjmp"));
    atomic_store(&hero_rebind_real_longjmp, (hero_rebind_jump_fn)dlsym(RTLD_DEFAULT, "longjmp"));
    atomic_store(&hero_rebind_real__longjmp, (hero_rebind_jump_fn)dlsym(RTLD_DEFAULT, "_longjmp"));
    atomic_store(&hero_rebind_real_setjmp, (hero_rebind_save_fn)dlsym(RTLD_DEFAULT, "setjmp"));
    atomic_store(&hero_rebind_real__setjmp, (hero_rebind_save_fn)dlsym(RTLD_DEFAULT, "_setjmp"));
    atomic_store(&hero_rebind_real_sigsetjmp, (hero_rebind_sigsave_fn)dlsym(RTLD_DEFAULT, "sigsetjmp"));
    atomic_store(&hero_rebind_real_getcontext, (hero_rebind_get_fn)dlsym(RTLD_DEFAULT, "getcontext"));
    atomic_store(&hero_rebind_real_setcontext, (hero_rebind_set_fn)dlsym(RTLD_DEFAULT, "setcontext"));
    atomic_store(&hero_rebind_real_swapcontext, (hero_rebind_swap_fn)dlsym(RTLD_DEFAULT, "swapcontext"));
    hero_rebind_real_makecontext =
        (void (*)(void *, void (*)(void), int, ...))dlsym(RTLD_DEFAULT, "makecontext");
    hero_rebind_calibrate();
    _dyld_register_func_for_add_image(hero_rebind_image);
    for (int sig = 1; sig < NSIG; sig += 1) {
        if (sig != SIGKILL && sig != SIGSTOP) hero_rebind_retramp(sig);
    }
}

#endif
