/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump.c — the C library's own save points and jumps, found before
 * `main`, and the one word of a jump's target that tells a handler left from
 * a handler jumped within (route J, panel 249's R3; panel 251's R1; panel
 * 255's R1 and R3, defects 2679 and 2702; defect 2522; design.md §1.12). On
 * Linux and every POSIX system but Darwin, whose route is `parts/rebind.c`.
 * The route is split by concern over six files, included in this order by
 * `runtime.c`, after `sigentries.c` (the handlers a thread is inside and the
 * rules that drop one) and `siginstall.c`:
 *
 *   sigjump.c         the C library's own functions, the accessor's state
 *   sigjump_probe.c   the assembly the calibration saves through
 *   sigjump_decode.c  the key, verified before `main`; the stack pointer a
 *                     jump will install
 *   sigjump_record.c  route A, the record, where the key is not verified
 *   sigjump_save.c    the save points and the jumps themselves
 *   sigjump_make.c    `makecontext`, which learns a context's stack
 *
 * WHAT IT TELLS. `parts/siginstall.c` keeps, per claimed thread, an entry for
 * each wrapper it runs around a program's C handlers. A handler left by a
 * jump leaves its entry, and the entries alone cannot tell a jump OUT of the
 * handler (the entry is dead) from a jump WITHIN it (the entry is live). The
 * jump is told apart at the jump, by the stack pointer it will land at and the
 * stack that lies on: `parts/sigentries.c`'s rules 4 to 7.
 *
 * ROUTE C: THE TARGET'S OWN STACK POINTER, read from the buffer the jump is
 * given. At a jump that buffer lives (the C library reads it next), so one
 * word of it is read and nothing else; glibc keeps the stack pointer there
 * mangled by a key of the process. The key is taken before `main` and
 * VERIFIED, never searched (`sigjump_decode.c`). A context's stack pointer
 * is a field of `ucontext_t`, never mangled. The jump reads what the C
 * library will install, so a refill by `memcpy`, a copy, a save this file
 * never saw, a record overwritten: each lands where its bytes say, and is
 * judged there. On this route no buffer a save filled is read at any other
 * moment, so none can have been given back (defect 2679).
 *
 * ROUTE A, WHERE THE KEY IS NOT VERIFIED (glibc's four other architectures, a
 * glibc whose layout moved, a build forced to it by
 * `HERO_JMP_FORCE_UNCALIBRATED`): the save points record which buffer they
 * filled and a frame just below the stack pointer they saved, and the
 * fingerprint that validates a record is copied by the kernel, so a buffer
 * given back since is an answer and never a fault (`sigjump_record.c`, defects
 * 2679 and 2867). A trusted record's frame is judged by
 * `sigentries.c`'s rules 4 to 7 as route C's decoded target is (defect 2745);
 * a jump to a buffer with no trusted record keeps every entry: a told refusal
 * (R1), never Heroes code running inside a signal.
 *
 * WHICH ROUTE IS ON is `hero_signal_jump_route`: bit 1 when the `setjmp`
 * family decodes, bit 2 when contexts do, so 3 is route C for both and 0 is
 * route A for both. The same accessor, with the same bits, names Darwin's
 * route.
 *
 * THE C LIBRARY'S OWN are found by the C library's own `dlsym(RTLD_NEXT)`
 * (`parts/siglookup.c`) before `main`, never inside a handler; a save point
 * reached earlier (a shared library's constructor) finds them there, once. A
 * save point reaches the C library's own by a tail call (`musttail`), so the
 * C library saves this function's CALLER and returns twice into it. Under
 * AddressSanitizer a jump first calls `__asan_handle_no_return`, as ASan's own
 * interceptor would, since a definition here displaces it.
 *
 * WHAT IT DOES NOT REACH, said: a save the runtime never sees, which a
 * function found by walking an image's symbol table by hand reaches (`dlsym`
 * and `dlvsym` answer this file's, `parts/siglookup.c`, since the author's
 * ruling C of 2026-11-20 and defect d-p259-clerk-1);
 * a handler left by a switch or a jump this file never sees (an inline
 * `__builtin_longjmp`, hand-written assembly, a C++ exception or
 * `_Unwind_*`), which keeps its entry and is told in words that name it; a
 * coroutine left by such a switch that saves into another context than the
 * one it was switched to, leaving that one as it was, which is judged ended
 * (`sigentries.c`, the life of a range; defects d-lane-d2745-1 and 2851); and
 * the one residue panel 255's R3 leaves by design, `rr_2`, a handler left by a
 * jump and landed in again from a coroutine on another range (no range's
 * bottom is recorded where the thread leaves it). A program library defining
 * one of these ten names is told `ffi_defined_elsewhere` (panel 239's member). */

#if !defined(_WIN32) && !defined(__APPLE__)

#include <link.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <ucontext.h>

typedef int (*hero_jmp_save_fn)(struct __jmp_buf_tag *);
typedef int (*hero_jmp_sigsave_fn)(struct __jmp_buf_tag *, int);
typedef void (*hero_jmp_jump_fn)(struct __jmp_buf_tag *, int);
typedef int (*hero_ctx_get_fn)(ucontext_t *);
typedef int (*hero_ctx_set_fn)(const ucontext_t *);
typedef void (*hero_ctx_make_fn)(ucontext_t *, void (*)(void), int, ...);

/* `swapcontext` IS DECLARED WITH MORE THAN ITS PARAMETERS (defect
 * d-gcc-seed-1): glibc's `<ucontext.h>` writes `__INDIRECT_RETURN` after them,
 * which says a call to it may come back by an indirect branch, so the caller
 * needs a landing pad after the call where branch protection is on. Where the
 * compiler knows the attribute it is `indirect_return`, else `returns_twice`
 * where a shadow stack is the default, else nothing (`bits/indirect-return.h`,
 * read in glibc 2.41 for aarch64 and x86-64). gcc counts `indirect_return` in
 * the function's type: gcc 15.2 on aarch64 refused this runtime's definition
 * without it as *conflicting types* (the CI's Linux arm64 leg, run
 * 45654270318), both types printed alike. So the definition carries the
 * library's own macro (`sigjump_save.c`), whatever it expands to; a C library
 * older than the macro declares nothing there. clang 22 knows no
 * `indirect_return`, so under clang the definition is what it was. And the
 * pointer to the C library's own has that declaration's type, read from it,
 * so a call through it is compiled as a call to `swapcontext` is. */
#if defined(__INDIRECT_RETURN)
#define HERO_CTX_SWAP_RETURN __INDIRECT_RETURN
#else
#define HERO_CTX_SWAP_RETURN
#endif
typedef __typeof__(&(swapcontext)) hero_ctx_swap_fn;

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define HERO_JMP_ASAN 1
void __asan_handle_no_return(void);
void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
void __asan_init(void);
#endif
#endif

/* Whether route C can be built here: the layout of `jmp_buf` and the probe's
 * assembly are known for glibc's two architectures alone. */
#if !defined(HERO_JMP_NO_DECODE) && (defined(__aarch64__) || defined(__x86_64__))
#define HERO_JMP_DECODES 1
#endif

/* How `makecontext` is wrapped, so the stacks it hands out are known on every
 * architecture (rule 7, `sigjump_make.c`): in assembly on those two, which
 * keeps every argument whatever their count, and in C on glibc's others,
 * which passes on a bounded count (defect d-lane-ctx-after-1: until then those
 * had no wrapper and no stack `makecontext` handed out was known, so a
 * coroutine started by `setcontext` ran the program's function inside a
 * signal). `HERO_JMP_MAKE_IN_C` builds the C wrapper on those two as well, as
 * glibc's other architectures build: the simulation, with `HERO_JMP_NO_DECODE`
 * (contexts and jumps on route A, as there), run on Linux arm64. */
#if !defined(HERO_JMP_MAKE_IN_C) && (defined(__aarch64__) || defined(__x86_64__))
#define HERO_JMP_MAKE_ASM 1
#endif

/* How many bytes of a context the fingerprint reads (route A): its saved
 * registers lie in the first of them on both Linux architectures. */
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
static _Atomic(hero_ctx_make_fn) hero_ctx_real_make = NULL;

static void *hero_jmp_next(const char *name) {
    void *found = hero_dl_next(name);
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
    atomic_store_explicit(&hero_ctx_real_make, (hero_ctx_make_fn)hero_jmp_next("makecontext"), memory_order_relaxed);
    atomic_store_explicit(&hero_ctx_real_swap, (hero_ctx_swap_fn)hero_jmp_next("swapcontext"), memory_order_release);
}

/* 1 once the key is verified; written once, before `main`, by the
 * constructor in `sigjump_decode.c`, and read with an acquire after.
 * `hero_ctx_decoding` is the same for a context's stack-pointer field
 * (public, never mangled). */
static _Atomic int hero_jmp_decoding = 0;
static _Atomic int hero_ctx_decoding = 0;
static _Atomic uintptr_t hero_jmp_key = 0;

int hero_signal_jump_route(void);
int hero_signal_jump_route(void) {
    return (atomic_load_explicit(&hero_jmp_decoding, memory_order_acquire) != 0 ? 1 : 0) |
           (atomic_load_explicit(&hero_ctx_decoding, memory_order_acquire) != 0 ? 2 : 0);
}

#endif
