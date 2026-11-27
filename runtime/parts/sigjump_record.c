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
 * VALIDATED by a fingerprint of the buffer's bytes, copied by the kernel
 * wherever the buffer is mapped and never loaded by this thread, so a buffer
 * the program has given back since (freed, unmapped, its library closed) is
 * an answer and never a fault or a read of freed memory (defect 2679): a
 * copy the kernel refuses is UNREAD, and a buffer given back and filled again
 * by anything but the save its record names reads another fingerprint. A jump
 * to a buffer with no trusted record keeps every entry and marks them
 * `unsure`: the told refusal. Until defect 2867 the fingerprint was read only
 * of a buffer the runtime could prove lives (the thread's own stack, its
 * alternate stack, an image loaded before any constructor), the rule of the
 * days its bytes were loaded, so a context or a `jmp_buf` on the heap was
 * never trusted, and a correct program leaving its handler by a
 * `setcontext` to its runner's heap context (`b_0`, `cs_0`) was refused in
 * words naming a copy, a refill or a full table, causes it had none of.
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

/* FNV-1a over the buffer's bytes, odd, so 0 can mean not yet read and
 * `HERO_JMP_UNREAD` a buffer that could not be read. The bytes are copied by
 * the kernel (`process_vm_readv` of this process), never loaded by this
 * thread: the buffer may have been given back or made unreadable by the
 * program since the save (a heap block freed, a page unmapped or `mprotect`ed
 * to none, the registered alternate stack unmapped: a SIGSEGV or a
 * heap-use-after-free here at the next save point when the bytes were loaded,
 * measured on Linux arm64 with `HERO_JMP_FORCE_UNCALIBRATED` 3, defects 2679
 * and d-lane-d2745-1), and a copy the kernel refuses is an answer, UNREAD,
 * never a fault. Nor is it the program's access, so AddressSanitizer does not
 * see it.
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

/* Every pair not yet read, but `except`'s: read now, its save having
 * returned, by the kernel wherever its buffer is mapped; one the kernel
 * refuses is marked unread, so that no jump trusts it and its handler serial
 * still tells a jump back into a handler left since (as Darwin's record does,
 * `parts/rebind_record.c`; defect 2745: dropped, a context the handler saved
 * on the heap was forgotten, and the jump back into the handler from its
 * coroutine was told in other words than route C's, panel 255's `ax_1_0_0`
 * with `HERO_JMP_FORCE_UNCALIBRATED` 3 on Linux arm64). */
static void hero_jmp_read_pending(HeroSignalStack *own, uintptr_t except) {
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        uintptr_t b = own->jump_buf[i];
        if (b == 0 || b == except || own->jump_print[i] != 0) continue;
        own->jump_print[i] = hero_jmp_fingerprint(b, own->jump_size[i]);
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
 * pending buffer read by the kernel, so a refill inside the handler
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
