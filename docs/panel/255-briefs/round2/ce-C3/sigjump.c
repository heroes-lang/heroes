/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump.c — a jump out of a signal handler told from a jump within
 * it, on Linux and every POSIX system but Darwin (route J, panel 249's R3;
 * panel 251's R1; panel 255, route C with A beneath it, defect 2679; defect
 * 2522; design.md §1.12). PANEL 255 DRAFT, the compiler-engineer's build.
 *
 * WHAT IT TELLS. `parts/siginstall.c` keeps, per claimed thread, the frames of
 * the wrappers it runs around a program's C handlers. A handler left by a
 * jump leaves its entry, and the entries alone cannot tell a jump OUT of the
 * handler (the entry is dead) from a jump WITHIN it (the entry is live). This
 * file tells them apart at the jump, by the stack pointer the jump will land
 * at: the entries deeper than it are dropped, by `hero_sig_prune`'s rule.
 *
 * C: THE TARGET'S OWN STACK POINTER, read from the buffer the jump is given.
 * At a jump that buffer lives (the C library reads it next), so one word of
 * it is read and nothing else; glibc keeps the stack pointer there mangled by
 * a key of the process. The key is taken before `main` and VERIFIED, never
 * searched: a probe in assembly fills every callee-saved register with a
 * pattern, saves through the C library's own `setjmp`, `_setjmp`,
 * `__sigsetjmp` and `getcontext` at two frames 0x9230 bytes apart, and the
 * key is taken only where every word it knows reads as known (the stack
 * pointer's word at both frames, a second mangled word under the same key,
 * the patterns where glibc keeps them unmangled; panel 255's Q2 measured the
 * layout on glibc 2.41 and 2.43, arm64 and x86-64). A context's stack pointer
 * is a field of `ucontext_t`, never mangled. The jump reads what the C
 * library will install, so a refill by `memcpy`, a copy, a save this file
 * never saw, a record overwritten: each lands where its bytes say, and is
 * judged there.
 *
 * A: WHERE THE KEY IS NOT VERIFIED (glibc's four other architectures, a
 * glibc whose layout moved, a probe forced to fail), the save points record
 * which buffer they filled and the frame of the call, as panel 251's R1 did,
 * and the fingerprint that validates a record is read ONLY of a buffer the
 * runtime can prove lives: on the thread's own stack, its current alternate
 * stack, or a writable segment of an image loaded before any library's
 * constructor ran (read at `.preinit_array`, so none of them can be
 * unloaded). A pending buffer anywhere else loses its record unread, and a
 * jump to it keeps every entry: the told refusal. So no route of this file
 * reads memory the program gave back (defect 2679).
 *
 * A JUMP TO A BUFFER WITH NO RECORD, UNDER A, KEEPS EVERY ENTRY: the error is
 * then a told refusal (R1), never Heroes code running inside a signal.
 *
 * THE C LIBRARY'S OWN are found by the C library's own `dlsym(RTLD_NEXT)`
 * (`parts/siglookup.c`) before `main`, never inside a handler. A save point
 * reaches the C library's own by a tail call (`musttail`), so the C library
 * saves this function's CALLER and returns twice into it. Under
 * AddressSanitizer a jump first calls `__asan_handle_no_return`, as ASan's own
 * interceptor would, since a definition here displaces it.
 *
 * WHAT IT DOES NOT REACH, said: `makecontext`'s new stack (a context it
 * starts runs on a stack neither the thread's own nor an alternate one, and
 * `hero_sig_prune` compares it with an alternate stack's entries by address);
 * a C++ exception or `_Unwind_*` out of a handler (no jump is called).
 * Under A only: a buffer refilled between its save and its jump by a save
 * this file never saw with no other save or jump between. */

#if !defined(_WIN32) && !defined(__APPLE__)

#include <link.h>
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
void __asan_init(void);
#endif
#endif

/* How many bytes of a context the fingerprint reads (A). */
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
    atomic_store_explicit(&hero_ctx_real_swap, (hero_ctx_swap_fn)hero_jmp_next("swapcontext"), memory_order_release);
}

/* ---- C: the key, verified before `main`, and the one word a jump reads ---- */

/* 1 once the key is verified; written once, before `main`, by the
 * constructor below, and read with an acquire after. */
static _Atomic int hero_jmp_decoding = 0;
static _Atomic uintptr_t hero_jmp_key = 0;

#if !defined(HERO_JMP_NO_DECODE) && (defined(__aarch64__) || defined(__x86_64__))
#define HERO_JMP_DECODES 1

/* What the probe saw: the stack pointer the C library's save was called at,
 * the address it returns to, and (arm64) the probe's frame pointer. */
typedef struct {
    uintptr_t sp;
    uintptr_t ret;
    uintptr_t fp;
    uintptr_t unused;
} HeroJmpProbe;

void hero_jmp_probe(void *buf, void *save, HeroJmpProbe *out);
extern const uintptr_t hero_jmp_patterns[];

#if defined(__aarch64__)
/* glibc's aarch64 `jmpbuf-offsets.h`: x19 to x28 in words 0 to 9 and x29 in
 * 10 as they are, the link register in 11 and the stack pointer in 13 each
 * `^` the pointer guard. Measured, not assumed (panel 255's Q2). */
#define HERO_JMP_SP_WORD 13
#define HERO_JMP_ROT 0
__asm__(".section .rodata\n.p2align 3\n.globl hero_jmp_patterns\n.hidden hero_jmp_patterns\n"
        "hero_jmp_patterns:\n"
        "  .quad 0x4845524f45530013, 0x4845524f45530014, 0x4845524f45530015, 0x4845524f45530016\n"
        "  .quad 0x4845524f45530017, 0x4845524f45530018, 0x4845524f45530019, 0x4845524f4553001a\n"
        "  .quad 0x4845524f4553001b, 0x4845524f4553001c\n"
        ".text\n.p2align 2\n.globl hero_jmp_probe\n.hidden hero_jmp_probe\n.type hero_jmp_probe, %function\n"
        "hero_jmp_probe:\n"
        "  stp x29, x30, [sp, #-96]!\n"
        "  mov x29, sp\n"
        "  stp x19, x20, [sp, #16]\n"
        "  stp x21, x22, [sp, #32]\n"
        "  stp x23, x24, [sp, #48]\n"
        "  stp x25, x26, [sp, #64]\n"
        "  stp x27, x28, [sp, #80]\n"
        "  sub sp, sp, #32\n"
        "  adrp x9, hero_jmp_patterns\n"
        "  add x9, x9, :lo12:hero_jmp_patterns\n"
        "  ldp x19, x20, [x9, #0]\n"
        "  ldp x21, x22, [x9, #16]\n"
        "  ldp x23, x24, [x9, #32]\n"
        "  ldp x25, x26, [x9, #48]\n"
        "  ldp x27, x28, [x9, #64]\n"
        "  mov x9, sp\n"
        "  str x9, [x2, #0]\n"
        "  adr x9, 1f\n"
        "  str x9, [x2, #8]\n"
        "  str x29, [x2, #16]\n"
        "  mov x9, x1\n"
        "  mov x1, #0\n"
        "  blr x9\n"
        "1:\n"
        "  add sp, sp, #32\n"
        "  ldp x19, x20, [sp, #16]\n"
        "  ldp x21, x22, [sp, #32]\n"
        "  ldp x23, x24, [sp, #48]\n"
        "  ldp x25, x26, [sp, #64]\n"
        "  ldp x27, x28, [sp, #80]\n"
        "  ldp x29, x30, [sp], #96\n"
        "  ret\n"
        ".size hero_jmp_probe, .-hero_jmp_probe\n");
#else
/* glibc's x86-64 `jmpbuf-offsets.h`: rbx, r12 to r15 in words 0, 2 to 5 as
 * they are; rbp in 1, the stack pointer in 6 and the return address in 7
 * each `^` the pointer guard and then rotated left by 17. Measured, not
 * assumed (panel 255's Q2). */
#define HERO_JMP_SP_WORD 6
#define HERO_JMP_ROT 17
__asm__(".section .rodata\n.p2align 3\n.globl hero_jmp_patterns\n.hidden hero_jmp_patterns\n"
        "hero_jmp_patterns:\n"
        "  .quad 0x4845524f45530003, 0x4845524f45530005, 0x4845524f4553000c, 0x4845524f4553000d\n"
        "  .quad 0x4845524f4553000e, 0x4845524f4553000f\n"
        ".text\n.globl hero_jmp_probe\n.hidden hero_jmp_probe\n.type hero_jmp_probe, @function\n"
        "hero_jmp_probe:\n"
        "  push %rbp\n"
        "  push %rbx\n"
        "  push %r12\n"
        "  push %r13\n"
        "  push %r14\n"
        "  push %r15\n"
        "  sub $8, %rsp\n"
        "  lea hero_jmp_patterns(%rip), %rax\n"
        "  mov 0(%rax), %rbx\n"
        "  mov 8(%rax), %rbp\n"
        "  mov 16(%rax), %r12\n"
        "  mov 24(%rax), %r13\n"
        "  mov 32(%rax), %r14\n"
        "  mov 40(%rax), %r15\n"
        "  mov %rsp, 0(%rdx)\n"
        "  lea 1f(%rip), %rax\n"
        "  mov %rax, 8(%rdx)\n"
        "  mov %rsi, %r11\n"
        "  xor %esi, %esi\n"
        "  call *%r11\n"
        "1:\n"
        "  add $8, %rsp\n"
        "  pop %r15\n"
        "  pop %r14\n"
        "  pop %r13\n"
        "  pop %r12\n"
        "  pop %rbx\n"
        "  pop %rbp\n"
        "  ret\n"
        ".size hero_jmp_probe, .-hero_jmp_probe\n");
#endif

static uintptr_t hero_jmp_ror(uintptr_t v, unsigned r) {
    return r == 0 ? v : (v >> r) | (v << (8 * sizeof v - r));
}

/* The stack pointer a jump to `env` will install: one word of it read. */
static uintptr_t hero_jmp_sp_of(const struct __jmp_buf_tag *env) {
    return hero_jmp_ror(((const uintptr_t *)env)[HERO_JMP_SP_WORD], HERO_JMP_ROT) ^
           atomic_load_explicit(&hero_jmp_key, memory_order_relaxed);
}

static uintptr_t hero_jmp_context_sp(const ucontext_t *uc) {
#if defined(__aarch64__)
    return (uintptr_t)uc->uc_mcontext.sp;
#else
    return (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
#endif
}

/* One save through the probe, and whether every word the probe knows reads
 * as known: under the key `*key` once one is taken (`*have`), else under the
 * key the stack pointer's word gives. */
__attribute__((noinline)) static bool hero_jmp_try(void *save, bool context, uintptr_t *key, bool *have) {
    union {
        struct __jmp_buf_tag jb[1];
        ucontext_t uc;
    } b;
    memset(&b, 0, sizeof b);
    HeroJmpProbe p = {0, 0, 0, 0};
    hero_jmp_probe(&b, save, &p);
    if (context) return hero_jmp_context_sp(&b.uc) == p.sp;
    const uintptr_t *w = (const uintptr_t *)b.jb;
    uintptr_t k = hero_jmp_ror(w[HERO_JMP_SP_WORD], HERO_JMP_ROT) ^ p.sp;
    if (*have && k != *key) return false;
    const uintptr_t *pat = hero_jmp_patterns;
#if defined(__aarch64__)
    for (int i = 0; i < 10; i += 1) {
        if (w[i] != pat[i]) return false;
    }
    if (w[10] != p.fp || (w[11] ^ k) != p.ret) return false;
#else
    if (w[0] != pat[0] || (hero_jmp_ror(w[1], 17) ^ k) != pat[1]) return false;
    for (int i = 2; i < 6; i += 1) {
        if (w[i] != pat[i]) return false;
    }
    if ((hero_jmp_ror(w[7], 17) ^ k) != p.ret) return false;
#endif
    *key = k;
    *have = true;
    return true;
}

/* The same, from a frame 0x9230 bytes further down the stack. */
__attribute__((noinline)) static bool hero_jmp_try_far(void *save, bool context, uintptr_t *key, bool *have) {
    volatile char *pad = __builtin_alloca(0x9230);
    pad[0] = 0;
    bool ok = hero_jmp_try(save, context, key, have);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return ok;
}

static void hero_jmp_calibrate(void) {
    void *saves[3] = {(void *)atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed),
                      (void *)atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed),
                      (void *)atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed)};
    uintptr_t key = 0;
    bool have = false;
    bool ok = true;
    for (int i = 0; i < 3 && ok; i += 1) {
        ok = hero_jmp_try(saves[i], false, &key, &have) && hero_jmp_try_far(saves[i], false, &key, &have);
    }
    void *get = (void *)atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed);
    ok = ok && hero_jmp_try(get, true, &key, &have) && hero_jmp_try_far(get, true, &key, &have);
#if defined(HERO_JMP_FORCE_UNCALIBRATED)
    ok = false;
#endif
    if (!ok) return;
    atomic_store_explicit(&hero_jmp_key, key, memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_decoding, 1, memory_order_release);
}
#else
static void hero_jmp_calibrate(void) {}
static uintptr_t hero_jmp_sp_of(const struct __jmp_buf_tag *env) {
    (void)env;
    return 0;
}
static uintptr_t hero_jmp_context_sp(const ucontext_t *uc) {
    (void)uc;
    return 0;
}
#endif

__attribute__((constructor)) static void hero_jmp_find_early(void) {
    hero_jmp_find();
    hero_jmp_calibrate();
}

/* ---- A: the record, read only where the memory provably lives ---- */

/* The writable segments of the images loaded before any library's
 * constructor ran: the executable, its libraries and what `LD_PRELOAD` names.
 * None of them is ever unloaded. Written once at `.preinit_array`, before
 * any thread, and only read after. */
#define HERO_JMP_IMAGES 64
static _Atomic uintptr_t hero_jmp_image_lo[HERO_JMP_IMAGES];
static _Atomic uintptr_t hero_jmp_image_hi[HERO_JMP_IMAGES];
static _Atomic int hero_jmp_images = 0;

static int hero_jmp_image(struct dl_phdr_info *info, size_t size, void *data) {
    (void)size;
    (void)data;
    for (int i = 0; i < info->dlpi_phnum; i += 1) {
        const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD || (ph->p_flags & PF_W) == 0) continue;
        int n = atomic_load_explicit(&hero_jmp_images, memory_order_relaxed);
        if (n == HERO_JMP_IMAGES) return 1;
        uintptr_t lo = (uintptr_t)info->dlpi_addr + (uintptr_t)ph->p_vaddr;
        atomic_store_explicit(&hero_jmp_image_lo[n], lo, memory_order_relaxed);
        atomic_store_explicit(&hero_jmp_image_hi[n], lo + (uintptr_t)ph->p_memsz, memory_order_relaxed);
        atomic_store_explicit(&hero_jmp_images, n + 1, memory_order_release);
    }
    return 0;
}

static void hero_jmp_images_read(void) {
#if defined(HERO_JMP_ASAN)
    __asan_init();
#endif
    dl_iterate_phdr(hero_jmp_image, NULL);
}
__attribute__((section(".preinit_array"), used)) static void (*hero_jmp_images_early)(void) = hero_jmp_images_read;

/* Whether `size` bytes at `at` lie wholly inside memory this thread cannot
 * have given back: its own stack, its current alternate stack, or an image
 * loaded before any constructor. */
static bool hero_jmp_lives(const HeroSignalStack *own, uintptr_t at, uint32_t size) {
    uintptr_t end = at + size;
    if (end < at) return false;
    if (at >= own->lo && end <= own->hi) return true;
    int n = atomic_load_explicit(&hero_jmp_images, memory_order_acquire);
    for (int i = 0; i < n; i += 1) {
        if (at >= atomic_load_explicit(&hero_jmp_image_lo[i], memory_order_relaxed) &&
            end <= atomic_load_explicit(&hero_jmp_image_hi[i], memory_order_relaxed)) {
            return true;
        }
    }
    stack_t alt;
    if (sigaltstack(NULL, &alt) == 0 && (alt.ss_flags & SS_DISABLE) == 0) {
        uintptr_t lo = (uintptr_t)alt.ss_sp;
        if (at >= lo && end <= lo + alt.ss_size) return true;
    }
    return false;
}

/* FNV-1a over the buffer's bytes, never 0, so 0 can mean not yet read. */
static uint64_t hero_jmp_fingerprint(uintptr_t buf, uint32_t size) {
    const unsigned char *p = (const unsigned char *)buf;
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    for (uint32_t i = 0; i < size; i += 1) h = (h ^ p[i]) * UINT64_C(0x100000001b3);
    return h | 1;
}

/* Every pair not yet read, but `except`'s: read now where its buffer
 * provably lives, its save having returned; dropped unread where it may not. */
static void hero_jmp_read_pending(HeroSignalStack *own, uintptr_t except) {
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        uintptr_t b = own->jump_buf[i];
        if (b == 0 || b == except || own->jump_print[i] != 0) continue;
        if (hero_jmp_lives(own, b, own->jump_size[i])) {
            own->jump_print[i] = hero_jmp_fingerprint(b, own->jump_size[i]);
        } else {
            own->jump_buf[i] = 0;
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

/* Before a jump to `buf`: the entries its target leaves, dropped. Under C by
 * the stack pointer it will install; under A where its record stands and its
 * bytes are the C library's, otherwise none, every entry standing marked as
 * one this jump may have left (`unsure`). */
static void hero_jmp_leave(const void *buf, bool context) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || own->depth == 0) return;
    if (atomic_load_explicit(&hero_jmp_decoding, memory_order_acquire) != 0) {
        hero_sig_prune(own, context ? hero_jmp_context_sp((const ucontext_t *)buf)
                                    : hero_jmp_sp_of((const struct __jmp_buf_tag *)buf));
        return;
    }
    uintptr_t key = (uintptr_t)buf;
    hero_jmp_read_pending(own, key);
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        if (own->jump_buf[i] != key) continue;
        bool refilled = own->jump_print[i] != 0 && own->jump_print[i] != hero_jmp_fingerprint(key, own->jump_size[i]);
        if (!refilled) {
            hero_sig_prune(own, own->jump_frame[i]);
            return;
        }
        break;
    }
    own->unsure = own->depth;
}

/* Whether the save points must record: only where the key is not verified. */
static bool hero_jmp_recording(void) { return atomic_load_explicit(&hero_jmp_decoding, memory_order_acquire) == 0; }

static _Noreturn void hero_jmp_go(hero_jmp_jump_fn real, struct __jmp_buf_tag *env, int val) {
#if defined(HERO_JMP_ASAN)
    __asan_handle_no_return();
#endif
    real(env, val);
    __builtin_unreachable();
}

/* THE SAVE POINTS: under A the frame noted, then the C library's own reached
 * by a branch; under C the branch alone. The names are parenthesised so
 * glibc's macros of them stay out. */
int(setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(_setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(__sigsetjmp)(struct __jmp_buf_tag env[1], int savemask) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_sigsave_fn real = atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env, savemask);
}

int(getcontext)(ucontext_t *ucp) {
    hero_jmp_find();
    if (hero_jmp_recording()) hero_jmp_note(ucp, (uintptr_t)__builtin_frame_address(0), HERO_JMP_CONTEXT_BYTES);
    hero_ctx_get_fn real = atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed);
    __attribute__((musttail)) return real(ucp);
}

/* THE JUMPS. Each finds the C library's own first, as a save point does. */
void(longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_longjmp, memory_order_relaxed), env, val);
}

void(_longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real__longjmp, memory_order_relaxed), env, val);
}

void(siglongjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_siglongjmp, memory_order_relaxed), env, val);
}

void __longjmp_chk(struct __jmp_buf_tag env[1], int val);
void __longjmp_chk(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_chk, memory_order_relaxed), env, val);
}

int(setcontext)(const ucontext_t *ucp) {
    hero_jmp_find();
    hero_jmp_leave(ucp, true);
    return atomic_load_explicit(&hero_ctx_real_set, memory_order_relaxed)(ucp);
}

/* Saves into `oucp` from this wrapper's own frame, which stays live while the
 * other context runs and returns through it, then leaves for `ucp`. */
int(swapcontext)(ucontext_t *restrict oucp, const ucontext_t *restrict ucp) {
    hero_jmp_find();
    if (hero_jmp_recording()) hero_jmp_note(oucp, (uintptr_t)__builtin_frame_address(0), HERO_JMP_CONTEXT_BYTES);
    hero_jmp_leave(ucp, true);
    return atomic_load_explicit(&hero_ctx_real_swap, memory_order_relaxed)(oucp, ucp);
}

#endif
