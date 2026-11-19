/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump.c — a jump out of a signal handler told from a jump within
 * it, by a validated record of each buffer the C library's save points fill,
 * on Linux and every POSIX system but Darwin (route J, panel 249's R3; its
 * validated record, panel 251's R1, ratified 2026-11-19; defect 2522;
 * design.md §1.12).
 *
 * WHAT IT TELLS. `parts/siginstall.c` keeps, per claimed thread, the frames of
 * the wrappers it runs around a program's C handlers. A handler left by a
 * jump leaves its entry, and the entries alone cannot tell a jump OUT of the
 * handler (the entry is dead) from a jump WITHIN it (the entry is live). This
 * file tells them apart at the jump: the C library's save points are defined
 * here, each recording which buffer it filled and the frame of the call that
 * filled it before it branches to the C library's own; a jump drops exactly
 * the entries whose wrapper lies deeper than the frame its buffer recorded,
 * by `hero_sig_prune`'s rule. The names: the save points `setjmp`, `_setjmp`,
 * `__sigsetjmp` (glibc's `sigsetjmp` is a macro for it) and `getcontext`; the
 * jumps `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk` (the fortified
 * one) and `setcontext`; and `swapcontext`, which saves and jumps.
 *
 * THE RECORD IS VALIDATED, and the record as first built was not (panel 251's
 * compiler-engineer, `ijstale.c`): a buffer recorded at a shallow frame that
 * returns, later refilled inside a handler by `memcpy` from a buffer of the
 * handler's own and jumped to, kept its old shallow frame, so the jump within
 * the handler dropped the handler's live entry, and the Heroes function it
 * then reached through twelve frames ran inside the signal, 80 runs of 80 on
 * both Linux images. So each pair also keeps a fingerprint of its buffer's
 * bytes, read at this thread's next save point or jump, once the C library has
 * filled it and returned; at a jump, a buffer whose bytes no longer read as its
 * fingerprint has no record.
 *
 * A JUMP TO A BUFFER WITH NO RECORD KEEPS EVERY ENTRY: one overwritten in a
 * full record, copied by `memcpy`, filled by the C library inside itself, or
 * on a thread nothing claimed. The error is then a told refusal (R1), never
 * Heroes code running inside a signal; its price is defect 2522's row, a
 * correct program that copies a buffer and jumps OUT of a handler through the
 * copy, refused with words that say a handler is running when none is.
 *
 * THE C LIBRARY'S OWN are found by `dlsym(RTLD_NEXT)` before `main` (a
 * constructor beside `parts/siginstall.c`'s), never inside a handler; a save
 * point reached before that constructor (a shared library's constructor) finds
 * them there, once. A save point reaches the C library's own by a tail call
 * (`musttail`), so the C library saves this function's CALLER and returns
 * twice into it, measured to end in a branch on both Linux architectures at
 * `-O0` and `-O2` under `flags()` (the compiler-engineer's `objdump -d`). Under
 * AddressSanitizer a jump first calls `__asan_handle_no_return`, as ASan's own
 * interceptor would, since a definition here displaces it.
 *
 * WHAT IT DOES NOT REACH, said (defect 2522's rows): a buffer refilled between
 * its save and its jump by a save this file never saw (an inline
 * `__builtin_setjmp` on x86-64, the C library's own internal saves) with no
 * other save or jump between; `makecontext`'s new stack (variadic, so not
 * defined here; a context it starts runs on a stack neither the thread's own
 * nor an alternate one); a C++ exception or `_Unwind_*` out of a handler (no
 * jump is called, so `hero_sig_prune` alone drops the entry later, which errs
 * toward refusal). A program library defining one of these ten names is told
 * `ffi_defined_elsewhere` (panel 239's member). */

#if !defined(_WIN32) && !defined(__APPLE__)

#include <setjmp.h>
#include <ucontext.h>

typedef int (*hero_jmp_save_fn)(struct __jmp_buf_tag *);
typedef int (*hero_jmp_sigsave_fn)(struct __jmp_buf_tag *, int);
typedef void (*hero_jmp_jump_fn)(struct __jmp_buf_tag *, int);
typedef int (*hero_ctx_get_fn)(ucontext_t *);
typedef int (*hero_ctx_set_fn)(const ucontext_t *);
typedef int (*hero_ctx_swap_fn)(ucontext_t *, const ucontext_t *);

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define HERO_JMP_ASAN 1
void __asan_handle_no_return(void);
#endif
#endif

/* How many bytes of a context the fingerprint reads: its saved registers lie
 * in the first of them on both Linux architectures. */
#define HERO_JMP_CONTEXT_BYTES 256

static _Atomic(hero_jmp_save_fn) hero_jmp_real_setjmp = NULL;
static _Atomic(hero_jmp_save_fn) hero_jmp_real__setjmp = NULL;
static _Atomic(hero_jmp_sigsave_fn) hero_jmp_real_sigsetjmp = NULL;
static _Atomic(hero_jmp_jump_fn) hero_jmp_real_longjmp = NULL;
static _Atomic(hero_jmp_jump_fn) hero_jmp_real__longjmp = NULL;
static _Atomic(hero_jmp_jump_fn) hero_jmp_real_siglongjmp = NULL;
static _Atomic(hero_jmp_jump_fn) hero_jmp_real_chk = NULL;
static _Atomic(hero_ctx_get_fn) hero_ctx_real_get = NULL;
static _Atomic(hero_ctx_set_fn) hero_ctx_real_set = NULL;
static _Atomic(hero_ctx_swap_fn) hero_ctx_real_swap = NULL;

static void *hero_jmp_next(const char *name) {
    void *found = dlsym(RTLD_NEXT, name);
    if (found == NULL) {
        hero_signal_say("panic: the C library's own ");
        hero_signal_say(name);
        hero_signal_say(" was not found\n");
        _exit(134);
    }
    return found;
}

/* `swapcontext`'s pointer is published last, with a release, so one found
 * there means all are. */
static void hero_jmp_find(void) {
    if (atomic_load_explicit(&hero_ctx_real_swap, memory_order_acquire) != NULL) return;
    atomic_store_explicit(&hero_jmp_real_setjmp, (hero_jmp_save_fn)hero_jmp_next("setjmp"), memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_real__setjmp, (hero_jmp_save_fn)hero_jmp_next("_setjmp"), memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_real_sigsetjmp, (hero_jmp_sigsave_fn)hero_jmp_next("__sigsetjmp"),
                          memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_real_longjmp, (hero_jmp_jump_fn)hero_jmp_next("longjmp"), memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_real__longjmp, (hero_jmp_jump_fn)hero_jmp_next("_longjmp"), memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_real_siglongjmp, (hero_jmp_jump_fn)hero_jmp_next("siglongjmp"),
                          memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_real_chk, (hero_jmp_jump_fn)hero_jmp_next("__longjmp_chk"), memory_order_relaxed);
    atomic_store_explicit(&hero_ctx_real_get, (hero_ctx_get_fn)hero_jmp_next("getcontext"), memory_order_relaxed);
    atomic_store_explicit(&hero_ctx_real_set, (hero_ctx_set_fn)hero_jmp_next("setcontext"), memory_order_relaxed);
    atomic_store_explicit(&hero_ctx_real_swap, (hero_ctx_swap_fn)hero_jmp_next("swapcontext"), memory_order_release);
}

__attribute__((constructor)) static void hero_jmp_find_early(void) { hero_jmp_find(); }

/* FNV-1a over the buffer's bytes, never 0, so 0 can mean not yet read. */
static uint64_t hero_jmp_fingerprint(uintptr_t buf, uint32_t size) {
    const unsigned char *p = (const unsigned char *)buf;
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    for (uint32_t i = 0; i < size; i += 1) h = (h ^ p[i]) * UINT64_C(0x100000001b3);
    return h | 1;
}

/* Every pair not yet read, but `except`'s, read now: its save has returned,
 * since this thread is at another save point or jump. */
static void hero_jmp_read_pending(HeroSignalStack *own, uintptr_t except) {
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        uintptr_t b = own->jump_buf[i];
        if (b != 0 && b != except && own->jump_print[i] == 0) {
            own->jump_print[i] = hero_jmp_fingerprint(b, own->jump_size[i]);
        }
    }
}

/* The record: a buffer's pair taken back where it has one, else the oldest
 * overwritten. Only this thread writes it; a handler interrupting a write here
 * finds the pair's buffer still 0, which matches no jump. */
static void hero_jmp_note(const void *buf, uintptr_t frame, uint32_t size) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL) return;
    uintptr_t key = (uintptr_t)buf;
    hero_jmp_read_pending(own, key);
    int slot = -1;
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        if (own->jump_buf[i] == key) {
            slot = i;
            break;
        }
    }
    if (slot < 0) {
        slot = (int)(own->jump_next % HERO_SIG_JUMPS);
        own->jump_next += 1;
    }
    own->jump_buf[slot] = 0;
    atomic_signal_fence(memory_order_seq_cst);
    own->jump_frame[slot] = frame;
    own->jump_print[slot] = 0;
    own->jump_size[slot] = size;
    atomic_signal_fence(memory_order_seq_cst);
    own->jump_buf[slot] = key;
}

/* Before a jump to `buf`: the entries its target leaves, dropped, where its
 * record stands and its bytes are the C library's; otherwise none. */
static void hero_jmp_leave(const void *buf) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || own->depth == 0) return;
    uintptr_t key = (uintptr_t)buf;
    hero_jmp_read_pending(own, key);
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        if (own->jump_buf[i] != key) continue;
        bool refilled = own->jump_print[i] != 0 && own->jump_print[i] != hero_jmp_fingerprint(key, own->jump_size[i]);
        if (!refilled) hero_sig_prune(own, own->jump_frame[i]);
        return;
    }
}

static _Noreturn void hero_jmp_go(hero_jmp_jump_fn real, struct __jmp_buf_tag *env, int val) {
#if defined(HERO_JMP_ASAN)
    __asan_handle_no_return();
#endif
    real(env, val);
    __builtin_unreachable();
}

/* THE SAVE POINTS: the frame noted, then the C library's own reached by a
 * branch. The names are parenthesised so glibc's macros of them stay out. */
int(setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(_setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(__sigsetjmp)(struct __jmp_buf_tag env[1], int savemask) {
    hero_jmp_find();
    hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    hero_jmp_sigsave_fn real = atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env, savemask);
}

int(getcontext)(ucontext_t *ucp) {
    hero_jmp_find();
    hero_jmp_note(ucp, (uintptr_t)__builtin_frame_address(0), HERO_JMP_CONTEXT_BYTES);
    hero_ctx_get_fn real = atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed);
    __attribute__((musttail)) return real(ucp);
}

/* THE JUMPS. Each finds the C library's own first, as a save point does: a
 * jump made before this file's constructor ran (another library's
 * constructor) finds them there, and after it the find is one load. */
void(longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_longjmp, memory_order_relaxed), env, val);
}

void(_longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real__longjmp, memory_order_relaxed), env, val);
}

void(siglongjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_siglongjmp, memory_order_relaxed), env, val);
}

void __longjmp_chk(struct __jmp_buf_tag env[1], int val);
void __longjmp_chk(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_chk, memory_order_relaxed), env, val);
}

int(setcontext)(const ucontext_t *ucp) {
    hero_jmp_find();
    hero_jmp_leave(ucp);
    return atomic_load_explicit(&hero_ctx_real_set, memory_order_relaxed)(ucp);
}

/* Saves into `oucp` from this wrapper's own frame, which stays live while the
 * other context runs and returns through it, then leaves for `ucp`. */
int(swapcontext)(ucontext_t *restrict oucp, const ucontext_t *restrict ucp) {
    hero_jmp_find();
    hero_jmp_note(oucp, (uintptr_t)__builtin_frame_address(0), HERO_JMP_CONTEXT_BYTES);
    hero_jmp_leave(ucp);
    return atomic_load_explicit(&hero_ctx_real_swap, memory_order_relaxed)(oucp, ucp);
}

#endif
