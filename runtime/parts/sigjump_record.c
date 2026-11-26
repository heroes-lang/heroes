/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump_record.c — route A: the records of the save points, for a
 * family whose decode did not verify (panel 255's R1 and R3, defects 2679,
 * 2702 and 2745; panel 251's R1; design.md §1.12). Needs `sigjump.c` and
 * `sigjump_decode.c`.
 *
 * WHERE THE KEY IS NOT VERIFIED the save points record, per claimed thread,
 * which buffer they filled, the serial of the innermost handler standing at
 * the save (0 for none), and a frame just below the stack pointer the save
 * wrote (`hero_jmp_below`: the frame of a function the save point calls,
 * which lies on the same stack as its caller and below it). A record is
 * VALIDATED by a fingerprint of the buffer's bytes, and the fingerprint is
 * read ONLY of a buffer the runtime can prove lives: on the thread's own
 * stack, its current alternate stack, or a writable segment of an image
 * loaded before any library's constructor ran (read at `.preinit_array`, so
 * none of them can be unloaded). A pair whose buffer may have been given back
 * is never read and never trusted (defect 2679), and a jump to a buffer with
 * no trusted record keeps every entry and marks them `unsure`: the told
 * refusal.
 *
 * WHERE THE RECORD IS TRUSTED the jump lands where the save left it, so its
 * frame is judged by `sigentries.c`'s rules 4 to 7 as route C judges a
 * decoded target (defect 2745): the stack it lies on, a coroutine's first,
 * then whether it lies above an entry on that stack. A bound below the true
 * target leaves no entry the true target would keep: the two lie less than a
 * signal frame apart, so a handler's frame between them was delivered above
 * the save point's caller, whose frame a jump then reaches only after it
 * returned. What a bound
 * cannot say is whether the landing lies BELOW the jumping frame; the record
 * says it by naming a handler that no longer stands (a jump back into a
 * handler left earlier, shape R). Until defect 2745 the record's serial
 * alone judged a trusted jump, every entry pushed after the save left
 * whatever stack the save was made on, and a jump into a coroutine's own
 * buffer from a handler (panel 255's A'') ran the program's function inside
 * the signal.
 *
 * WHERE THE KEY IS VERIFIED nothing of this is read: `sigjump_save.c` judges
 * the stack pointer the jump itself will install, by `sigentries.c`'s rules. */

#if !defined(_WIN32) && !defined(__APPLE__)


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

/* The stack-pointer word of the context at `context`, copied by the kernel
 * wherever it is mapped, so a context given back since is an answer and never
 * a fault (defect 2850: a coroutine the C library starts through `uc_link` is
 * judged ended by its context naming what it named when the swap began);
 * false where no context field decodes or the kernel refuses the copy. */
static bool hero_jmp_context_word(const void *context, uintptr_t *word) {
    uintptr_t at = context != NULL ? hero_jmp_context_sp_at((const ucontext_t *)context) : 0;
    if (at == 0) return false;
    struct iovec into = {word, sizeof *word};
    struct iovec from = {(void *)at, sizeof *word};
    return process_vm_readv(getpid(), &into, 1, &from, 1, 0) == (ssize_t)sizeof *word;
}

/* Whether the coroutine a `swapcontext` switched to may still be alive once
 * the switch has returned, by the context it switched to: a coroutine that
 * yields saves itself there, by a save this runtime may never see (a switch
 * written in assembly, or found by walking an image's symbol table by hand),
 * so a stack pointer other than `entered`, the one it named at the switch, is
 * one witness of its life beside the switches made from its range
 * (`sigentries.c`). Copied by the kernel wherever the context is mapped
 * (`hero_jmp_context_word`), never loaded, so a context given back since is an
 * answer and never a fault (defect d-lane-d2745-1); and where nothing can be
 * read, or the swap returned on another thread, nothing proves the coroutine
 * ended, so it is held alive: a told refusal at worst. Until defect 2851 a
 * context that did not provably live (a heap block, a mapping) was not read
 * and its coroutine was judged ended, so one that yielded by a switch the
 * runtime never sees, saving into its heap context, was judged gone, and a
 * handler's jump into it, on a stack carved from the thread's own, ran the
 * program's function inside the signal. */
static bool hero_jmp_context_moved(HeroSignalStack *own, const ucontext_t *ucp, uintptr_t entered) {
    uintptr_t word = 0;
    if (own == NULL || hero_sig_own() != own || !hero_jmp_context_word(ucp, &word)) return true;
    return word != entered;
}

/* FNV-1a over the buffer's bytes, odd, so 0 can mean not yet read and
 * `HERO_JMP_UNREAD` a buffer that could not be read. The bytes are copied by
 * the kernel (`process_vm_readv` of this process), never loaded by this
 * thread: memory that provably lives may still have been made unreadable by
 * the program since the save (a static page `mprotect`ed to none, or the
 * registered alternate stack unmapped: a SIGSEGV here at the next save point,
 * measured on Linux arm64 with `HERO_JMP_FORCE_UNCALIBRATED` 3, defect
 * d-lane-d2745-1's class), and a copy the kernel refuses is an answer,
 * UNREAD, never a fault.
 * A system refusing the call itself (a seccomp filter) reads every buffer as
 * UNREAD: a told refusal at its jump. */
#define HERO_JMP_UNREAD 2
#define HERO_JMP_PRINT_MAX 512
static uint64_t hero_jmp_fingerprint(uintptr_t buf, uint32_t size) {
    unsigned char copy[HERO_JMP_PRINT_MAX];
    if (size > sizeof copy) return HERO_JMP_UNREAD;
    struct iovec into = {copy, size};
    struct iovec from = {(void *)buf, size};
    if (process_vm_readv(getpid(), &into, 1, &from, 1, 0) != (ssize_t)size) return HERO_JMP_UNREAD;
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    for (uint32_t i = 0; i < size; i += 1) h = (h ^ copy[i]) * UINT64_C(0x100000001b3);
    return h | 1;
}

/* Every pair not yet read, but `except`'s: read now where its buffer
 * provably lives, its save having returned; marked unread where it may not,
 * its bytes never touched, so that no jump trusts it and its handler serial
 * still tells a jump back into a handler left since (as Darwin's record does,
 * `parts/rebind_record.c`; defect 2745: dropped, a context the handler saved
 * on the heap was forgotten, and the jump back into the handler from its
 * coroutine was told in other words than route C's, panel 255's `ax_1_0_0`
 * with `HERO_JMP_FORCE_UNCALIBRATED` 3 on Linux arm64). */
static void hero_jmp_read_pending(HeroSignalStack *own, uintptr_t except) {
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        uintptr_t b = own->jump_buf[i];
        if (b == 0 || b == except || own->jump_print[i] != 0) continue;
        if (hero_jmp_lives(own, b, own->jump_size[i])) {
            own->jump_print[i] = hero_jmp_fingerprint(b, own->jump_size[i]);
        } else {
            own->jump_print[i] = HERO_JMP_UNREAD;
        }
    }
}

/* A frame just below its caller's stack pointer, on the same stack: the
 * bound route A records of where a save point's jump will land. Never
 * inlined, so the frame is its own. */
__attribute__((noinline)) static uintptr_t hero_jmp_below(void) {
    uintptr_t frame = (uintptr_t)__builtin_frame_address(0);
    __asm__ volatile("" ::: "memory");
    return frame;
}

/* The record: a buffer's pair taken back where it has one, else the oldest
 * overwritten. Only this thread writes it; a handler interrupting a write here
 * finds the pair's buffer still 0, which matches no jump. */
static void hero_jmp_note(const void *buf, uint32_t size, uintptr_t frame) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || buf == NULL) return;
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
    own->jump_entry[slot] = hero_sig_innermost(own);
    own->jump_print[slot] = 0;
    own->jump_size[slot] = size;
    own->jump_sp[slot] = frame;
    atomic_signal_fence(memory_order_seq_cst);
    own->jump_buf[slot] = key;
}

/* `buf`'s record dropped, its context now naming another stack (called by
 * `makecontext`'s wrapper, `sigjump_make.c`). */
static void hero_jmp_forget(const void *buf) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL) return;
    uintptr_t key = (uintptr_t)buf;
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        if (own->jump_buf[i] == key) own->jump_buf[i] = 0;
    }
}

/* Under route A at a handler's begin (`parts/siginstall.c`'s wrapper): every
 * pending buffer read where it provably lives, so a refill inside the handler
 * by a save this file never saw, with no save or jump between, is told by its
 * bytes (`x_unseen0` ran inside the signal under panel 255's R1 and A). */
static void hero_jmp_observe(HeroSignalStack *own) {
    if (atomic_load_explicit(&hero_jmp_decoding, memory_order_acquire) == 0 ||
        atomic_load_explicit(&hero_ctx_decoding, memory_order_acquire) == 0) {
        hero_jmp_read_pending(own, 0);
    }
}

/* The record of `buf` this thread trusts, or -1: one with its bytes still the
 * C library's own, which no later write by the program refilled. */
static int hero_jmp_trusted(HeroSignalStack *own, uintptr_t key) {
    hero_jmp_read_pending(own, key);
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        if (own->jump_buf[i] != key) continue;
        if (own->jump_print[i] == HERO_JMP_UNREAD) return -1;
        bool refilled = own->jump_print[i] != 0 && own->jump_print[i] != hero_jmp_fingerprint(key, own->jump_size[i]);
        return refilled ? -1 : i;
    }
    return -1;
}

/* Whether a record naming `entry` names a handler that no longer stands: the
 * save was made inside a handler the thread has left since, so a jump to it
 * lands in frames already left (shape R). */
static bool hero_jmp_left_since(const HeroSignalStack *own, uint64_t entry) {
    if (entry == 0) return false;
    for (int i = 0; i < own->depth; i += 1) {
        if (!own->entries[i].dead && own->entries[i].serial == entry) return false;
    }
    return true;
}

/* Route A outside every handler, the record not trusted: the target's record
 * asked, nothing read of any buffer; a record naming an entry is a save made
 * inside a handler, and a jump to it now re-enters that handler's frames,
 * whether or not its bytes could be fingerprinted (a refusal is the safe side
 * of a stale record). */
static void hero_jmp_reenter_by_record(HeroSignalStack *own, const void *buf) {
    uintptr_t key = (uintptr_t)buf;
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        if (own->jump_buf[i] != key || own->jump_entry[i] == 0) continue;
        uint64_t serial = 0;
        if (!hero_sig_push(own, 0, 0, 0, 2, &serial)) own->lost += 1;
        return;
    }
}

/* Route A's target for a jump to `buf` (rules 4 to 7, defect 2745): true
 * where the record is trusted, with `*t` the frame it holds and `*back`
 * whether it names a handler left since. */
static bool hero_jmp_target(HeroSignalStack *own, const void *buf, uintptr_t *t, bool *back) {
    int slot = hero_jmp_trusted(own, (uintptr_t)buf);
    if (slot < 0 || own->jump_sp[slot] == 0) return false;
    *t = own->jump_sp[slot];
    *back = hero_jmp_left_since(own, own->jump_entry[slot]);
    return true;
}

/* A jump to `buf` no trusted record follows: every entry kept, marked unsure
 * inside a handler; outside every handler, a re-entry where its record names
 * one. */
static void hero_jmp_unfollowed(HeroSignalStack *own, const void *buf, bool standing) {
    if (standing) {
        hero_sig_mark_unsure(own);
    } else {
        hero_jmp_reenter_by_record(own, buf);
    }
}

/* The target of a jump to `buf` where the record is trusted; where it is not,
 * the jump told unfollowed, and false. */
static bool hero_jmp_recorded(HeroSignalStack *own, const void *buf, bool standing, uintptr_t *t, bool *back) {
    if (hero_jmp_target(own, buf, t, back)) return true;
    hero_jmp_unfollowed(own, buf, standing);
    return false;
}

/* Route A's landing at `t`, a bound of the target the record trusts: a
 * re-entry where the record names a handler left since, or the landing lies
 * on the alternate stack from outside it. */
static void hero_jmp_landed(HeroSignalStack *own, uintptr_t t, bool back) {
    hero_sig_reentered(own, t, back, (uintptr_t)__builtin_frame_address(0));
}

#endif
