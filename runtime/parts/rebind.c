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
 * WHAT IT DOES NOT REACH, said (defect 2522's rows): an installer or a save
 * point reached by `dlsym`, or made inside the shared cache (whose binds are
 * not rewritable; 52 cache images import a save point, 51 `sigaction`); a raw
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

/* -- the entries and the trampoline ------------------------------------ */

/* The trampolined handlers this thread is inside, innermost first, each living
 * in its trampoline's own frame; the innermost in the thread key's slot, read
 * with `pthread_getspecific`, which allocates nothing (a thread-local's first
 * access may, inside a signal: `parts/sigguard.c`). */
typedef struct HeroRebindEntry {
    struct HeroRebindEntry *prev;
} HeroRebindEntry;

static _Atomic pthread_key_t hero_rebind_key;
static _Atomic int hero_rebind_keyed = 0;

static HeroRebindEntry *hero_rebind_now(void) {
    if (!atomic_load_explicit(&hero_rebind_keyed, memory_order_acquire)) return NULL;
    return pthread_getspecific(atomic_load_explicit(&hero_rebind_key, memory_order_relaxed));
}

static void hero_rebind_set(HeroRebindEntry *e) {
    if (atomic_load_explicit(&hero_rebind_keyed, memory_order_acquire)) {
        pthread_setspecific(atomic_load_explicit(&hero_rebind_key, memory_order_relaxed), e);
    }
}

typedef void (*hero_rebind_catcher)(int, siginfo_t *, void *);

/* The kernel's call of the catcher, made here in its place, so its C type is
 * not checked, for `parts/siginstall.c`'s wrapper's reason. NOR IS ITS FRAME
 * INSTRUMENTED BY AddressSanitizer: it never returns, leaving by
 * `__sigreturn`, so redzones ASan laid around its entry were never taken
 * back, and a later frame that reused those bytes was told a
 * stack-buffer-overflow that was none (`fprintf` in the end of `main`'s
 * report, `the-platform-layer-runs-the-leak-check-after-a-stop` under
 * `--sanitize -O0`, 2026-11-19, ASan naming this frame's `me`). libSystem's
 * own `_sigtramp`, which this stands in for, is not instrumented either. */
__attribute__((no_sanitize("function", "address"))) static void hero_rebind_tramp(hero_rebind_catcher catcher, int style,
                                                                                  int sig, siginfo_t *info,
                                                                                  void *context, uintptr_t token) {
    (void)style;
    HeroRebindEntry me;
    me.prev = hero_rebind_now();
    hero_rebind_set(&me);
    catcher(sig, info, context);
    hero_rebind_set(me.prev);
    __sigreturn(context, HERO_REBIND_RETURN_FLAVOR, token);
    __builtin_trap();
}

/* The thunk's question (`parts/sigguard.c`). */
static int hero_rebind_inside(void) { return hero_rebind_now() != NULL; }

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

/* Read by `hero_rebind_makecontext`'s assembly by name, so neither static nor
 * atomic: written once, before `main`, and only read afterwards. */
void (*hero_rebind_real_makecontext)(void *, void (*)(void), int, ...) = NULL;

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

/* -- route J: the records ---------------------------------------------- */

/* A buffer, its thread, the entry live at its save, and its fingerprint once
 * read. Written and read with no lock: a writer claims a record by moving its
 * buffer word to 1, a reader trusts one only where the buffer word reads the
 * same before and after; any race is a miss, and a miss keeps every entry. A
 * buffer's home is a hash of it and its thread, probed HERO_REBIND_PROBES
 * deep. */
#define HERO_REBIND_RECORDS 4096
#define HERO_REBIND_PROBES 8
/* The bytes a fingerprint reads: Darwin arm64's `jmp_buf` is 48 `int`s. */
#define HERO_REBIND_PRINT_BYTES 192

typedef struct {
    _Atomic uintptr_t buf;
    _Atomic uintptr_t thread;
    _Atomic uintptr_t entry;
    _Atomic uint64_t print;
    _Atomic int printed;
} HeroRebindRecord;

static HeroRebindRecord hero_rebind_records[HERO_REBIND_RECORDS];

/* The one buffer this thread saved into and has not yet fingerprinted, in a
 * second key's slot. */
static _Atomic pthread_key_t hero_rebind_pending;
static _Atomic int hero_rebind_pending_keyed = 0;

static size_t hero_rebind_home(uintptr_t buf, uintptr_t thread) {
    uint64_t h = (uint64_t)buf * UINT64_C(0x9E3779B97F4A7C15) ^ (uint64_t)thread * UINT64_C(0xC2B2AE3D27D4EB4F);
    return (size_t)(h >> 52) & (HERO_REBIND_RECORDS - 1);
}

static HeroRebindRecord *hero_rebind_probe(size_t home, int i) {
    return &hero_rebind_records[(home + (size_t)i) & (HERO_REBIND_RECORDS - 1)];
}

static uint64_t hero_rebind_fingerprint(const void *buf) {
    const unsigned char *p = (const unsigned char *)buf;
    uint64_t h = UINT64_C(1469598103934665603);
    for (int i = 0; i < HERO_REBIND_PRINT_BYTES; i += 1) {
        h ^= p[i];
        h *= UINT64_C(1099511628211);
    }
    return h;
}

/* `r` claimed from `was` and written for this buffer, thread and entry. */
static void hero_rebind_write(HeroRebindRecord *r, uintptr_t was, uintptr_t buf, uintptr_t thread, uintptr_t entry) {
    if (!atomic_compare_exchange_strong(&r->buf, &was, (uintptr_t)1)) return;
    atomic_store(&r->thread, thread);
    atomic_store(&r->entry, entry);
    atomic_store(&r->printed, 0);
    atomic_store_explicit(&r->buf, buf, memory_order_release);
}

/* This thread's record of `buf`, or NULL. */
static HeroRebindRecord *hero_rebind_record(const void *buf) {
    uintptr_t b = (uintptr_t)buf;
    uintptr_t t = (uintptr_t)pthread_self();
    size_t home = hero_rebind_home(b, t);
    for (int i = 0; i < HERO_REBIND_PROBES; i += 1) {
        HeroRebindRecord *r = hero_rebind_probe(home, i);
        if (atomic_load_explicit(&r->buf, memory_order_acquire) == b && atomic_load(&r->thread) == t) return r;
    }
    return NULL;
}

/* The pending buffer fingerprinted: its save has returned, since this thread
 * is at another save point or jump. */
static void hero_rebind_settle(void) {
    if (!atomic_load_explicit(&hero_rebind_pending_keyed, memory_order_acquire)) return;
    pthread_key_t key = atomic_load_explicit(&hero_rebind_pending, memory_order_relaxed);
    void *buf = pthread_getspecific(key);
    if (buf == NULL) return;
    pthread_setspecific(key, NULL);
    HeroRebindRecord *r = hero_rebind_record(buf);
    if (r == NULL) return;
    atomic_store(&r->print, hero_rebind_fingerprint(buf));
    atomic_store(&r->printed, 1);
}

/* A save into `buf` with `entry` innermost: its record taken back where it has
 * one, else a free record near its home, else the home overwritten. */
static void hero_rebind_note(const void *buf, uintptr_t entry) {
    hero_rebind_settle();
    uintptr_t b = (uintptr_t)buf;
    uintptr_t t = (uintptr_t)pthread_self();
    size_t home = hero_rebind_home(b, t);
    HeroRebindRecord *free_one = NULL;
    int written = 0;
    for (int i = 0; i < HERO_REBIND_PROBES && !written; i += 1) {
        HeroRebindRecord *r = hero_rebind_probe(home, i);
        uintptr_t v = atomic_load_explicit(&r->buf, memory_order_acquire);
        if (v == b && atomic_load(&r->thread) == t) {
            hero_rebind_write(r, b, b, t, entry);
            written = 1;
        } else if (v == 0 && free_one == NULL) {
            free_one = r;
        }
    }
    if (!written) {
        if (free_one != NULL) {
            hero_rebind_write(free_one, 0, b, t, entry);
        } else {
            HeroRebindRecord *r = hero_rebind_probe(home, 0);
            uintptr_t v = atomic_load(&r->buf);
            if (v > 1) hero_rebind_write(r, v, b, t, entry);
        }
    }
    if (atomic_load_explicit(&hero_rebind_pending_keyed, memory_order_acquire)) {
        pthread_setspecific(atomic_load_explicit(&hero_rebind_pending, memory_order_relaxed), (void *)(uintptr_t)buf);
    }
}

/* `buf`'s record dropped, its context now naming another stack. */
static void hero_rebind_forget(const void *buf) {
    uintptr_t b = (uintptr_t)buf;
    uintptr_t t = (uintptr_t)pthread_self();
    size_t home = hero_rebind_home(b, t);
    for (int i = 0; i < HERO_REBIND_PROBES; i += 1) {
        HeroRebindRecord *r = hero_rebind_probe(home, i);
        uintptr_t v = b;
        if (atomic_load(&r->thread) == t) (void)atomic_compare_exchange_strong(&r->buf, &v, (uintptr_t)0);
    }
}

/* A jump to `buf`: outside every handler nothing to do; else the entries its
 * record holds, where it has one whose bytes are still the C library's;
 * else every entry kept. */
static void hero_rebind_jump(const void *buf) {
    hero_rebind_settle();
    if (hero_rebind_now() == NULL) return;
    HeroRebindRecord *r = hero_rebind_record(buf);
    if (r == NULL) return;
    if (atomic_load(&r->printed) && atomic_load(&r->print) != hero_rebind_fingerprint(buf)) return;
    uintptr_t b = (uintptr_t)buf;
    uintptr_t entry = atomic_load(&r->entry);
    if (atomic_load_explicit(&r->buf, memory_order_acquire) != b) return;
    hero_rebind_set((HeroRebindEntry *)entry);
}

/* -- the save points and the jumps -------------------------------------- */

/* Each save records, then reaches the C library's own by a tail call, so it
 * saves this function's caller and returns twice into it. */
static int hero_rebind_setjmp(int *env) {
    hero_rebind_note(env, (uintptr_t)hero_rebind_now());
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_setjmp, memory_order_relaxed)(env);
}

static int hero_rebind__setjmp(int *env) {
    hero_rebind_note(env, (uintptr_t)hero_rebind_now());
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real__setjmp, memory_order_relaxed)(env);
}

static int hero_rebind_sigsetjmp(int *env, int save) {
    hero_rebind_note(env, (uintptr_t)hero_rebind_now());
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_sigsetjmp, memory_order_relaxed)(env, save);
}

static int hero_rebind_getcontext(void *context) {
    hero_rebind_note(context, (uintptr_t)hero_rebind_now());
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_getcontext, memory_order_relaxed)(context);
}

static int hero_rebind_swapcontext(void *from, const void *to) {
    hero_rebind_note(from, (uintptr_t)hero_rebind_now());
    hero_rebind_jump(to);
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
    hero_rebind_jump(context);
    return atomic_load_explicit(&hero_rebind_real_setcontext, memory_order_relaxed)(context);
}

/* `makecontext` points its context at another stack, so the record goes. Its
 * arguments are variadic, which `musttail` refuses (clang: "may not be used
 * with variadic functions"), so the wrapper is written in assembly: the
 * argument registers kept, the forgetting called, then a branch to the C
 * library's own with the stack as the caller left it. */
void hero_rebind_forget_context(const void *context);
void hero_rebind_forget_context(const void *context) { hero_rebind_forget(context); }

#if defined(__aarch64__)
__attribute__((naked)) static void hero_rebind_makecontext(void *context, void (*start)(void), int count, ...) {
    __asm__ volatile("stp x29, x30, [sp, #-48]!\n\t"
                     "mov x29, sp\n\t"
                     "stp x0, x1, [sp, #16]\n\t"
                     "str x2, [sp, #32]\n\t"
                     "bl _hero_rebind_forget_context\n\t"
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
                     "call _hero_rebind_forget_context\n\t"
                     "add $8, %rsp\n\t"
                     "pop %rax\n\tpop %r9\n\tpop %r8\n\tpop %rcx\n\tpop %rdx\n\tpop %rsi\n\tpop %rdi\n\tpop %rbp\n\t"
                     "mov _hero_rebind_real_makecontext@GOTPCREL(%rip), %r11\n\t"
                     "jmp *(%r11)");
}
#else
#error parts/rebind.c names no makecontext wrapper for this machine
#endif

/* -- the rewrite of an image's slots ------------------------------------ */

/* The function a slot bound to `name` (Mach-O's spelling, a leading `_`)
 * is given instead, or NULL for a name this file leaves alone. */
static void *hero_rebind_replacement(const char *name) {
    static const struct {
        const char *name;
        void *with;
    } table[] = {
        {"_sigaction", (void *)hero_rebind_sigaction},
        {"_signal", (void *)hero_rebind_signal},
        {"_siglongjmp", (void *)hero_rebind_siglongjmp},
        {"_longjmp", (void *)hero_rebind_longjmp},
        {"__longjmp", (void *)hero_rebind__longjmp},
        {"_setcontext", (void *)hero_rebind_setcontext},
        {"_swapcontext", (void *)hero_rebind_swapcontext},
        {"_setjmp", (void *)hero_rebind_setjmp},
        {"__setjmp", (void *)hero_rebind__setjmp},
        {"_sigsetjmp", (void *)hero_rebind_sigsetjmp},
        {"_getcontext", (void *)hero_rebind_getcontext},
        {"_makecontext", (void *)hero_rebind_makecontext},
    };
    for (size_t i = 0; i < sizeof table / sizeof table[0]; i += 1) {
        if (strcmp(name, table[i].name) == 0) return table[i].with;
    }
    return NULL;
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
    pthread_key_t key;
    if (pthread_key_create(&key, NULL) == 0) {
        atomic_store_explicit(&hero_rebind_key, key, memory_order_relaxed);
        atomic_store_explicit(&hero_rebind_keyed, 1, memory_order_release);
    }
    if (pthread_key_create(&key, NULL) == 0) {
        atomic_store_explicit(&hero_rebind_pending, key, memory_order_relaxed);
        atomic_store_explicit(&hero_rebind_pending_keyed, 1, memory_order_release);
    }
    atomic_store(&hero_rebind_real_sigaction, (hero_rebind_action_fn)dlsym(RTLD_DEFAULT, "sigaction"));
    atomic_store(&hero_rebind_real_signal, (hero_rebind_signal_fn)dlsym(RTLD_DEFAULT, "signal"));
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
    _dyld_register_func_for_add_image(hero_rebind_image);
    for (int sig = 1; sig < NSIG; sig += 1) {
        if (sig != SIGKILL && sig != SIGSTOP) hero_rebind_retramp(sig);
    }
}

#endif
