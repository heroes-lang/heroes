/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump_record.c — route A: the records of the save points, for a
 * family whose decode did not verify (panel 255's R1 and R3, defects 2679 and
 * 2702; panel 251's R1; design.md §1.12). Needs `sigjump.c` and
 * `sigjump_decode.c`.
 *
 * WHERE THE KEY IS NOT VERIFIED the save points record, per claimed thread,
 * which buffer they filled and the serial of the innermost handler standing at
 * the save (0 for none): the handler's own order, never an address on a stack.
 * A record is VALIDATED by a fingerprint of the buffer's bytes, and the
 * fingerprint is read ONLY of a buffer the runtime can prove lives: on the
 * thread's own stack, its current alternate stack, or a writable segment of an
 * image loaded before any library's constructor ran (read at
 * `.preinit_array`, so none of them can be unloaded). A pair whose buffer may
 * have been given back loses its record unread (defect 2679), and a jump to a
 * buffer with no trusted record keeps every entry and marks them `unsure`: the
 * told refusal. Where the record is trusted every entry pushed after its save
 * that nothing suspends is left, and a record naming an entry that no longer
 * stands is a jump back into a handler left earlier (shape R).
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
static void hero_jmp_note(const void *buf, uint32_t size) {
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
    atomic_signal_fence(memory_order_seq_cst);
    own->jump_buf[slot] = key;
}

/* `buf`'s record dropped, its context now naming another stack. */
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
        bool refilled = own->jump_print[i] != 0 && own->jump_print[i] != hero_jmp_fingerprint(key, own->jump_size[i]);
        return refilled ? -1 : i;
    }
    return -1;
}

/* Route A at a jump to `buf` while a handler stands (rules 5 and 6 obeyed):
 * where the record is trusted, every entry pushed after its save that nothing
 * pins is left, and the entry it names runs again; where the entry it names no
 * longer stands, the jump re-enters a handler left earlier, which stands again
 * as an entry, unsure; where no record is trusted, every entry is kept and
 * marked unsure. */
static void hero_jmp_by_record(HeroSignalStack *own, const void *buf) {
    int slot = hero_jmp_trusted(own, (uintptr_t)buf);
    if (slot < 0) {
        hero_sig_mark_unsure(own);
        return;
    }
    uint64_t entry = own->jump_entry[slot];
    bool named = entry == 0;
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        HeroSigEntry *e = &own->entries[i];
        if (e->dead) continue;
        if (e->serial == entry) {
            named = true;
            hero_sig_resumed(e);
        }
        if (e->serial > entry && !hero_sig_pinned(e)) hero_sig_drop(own, i);
    }
    if (!named) {
        uint64_t serial = 0;
        if (!hero_sig_push(own, 0, 0, 0, 2, &serial)) own->lost += 1;
    }
}

/* Route A outside every handler: the target's record asked, nothing read of
 * any buffer; a record naming an entry is a save made inside a handler, and a
 * jump to it now re-enters that handler's frames, whether or not its bytes
 * could be fingerprinted (a refusal is the safe side of a stale record). */
static void hero_jmp_reenter_by_record(HeroSignalStack *own, const void *buf) {
    uintptr_t key = (uintptr_t)buf;
    for (int i = 0; i < HERO_SIG_JUMPS; i += 1) {
        if (own->jump_buf[i] != key || own->jump_entry[i] == 0) continue;
        uint64_t serial = 0;
        if (!hero_sig_push(own, 0, 0, 0, 2, &serial)) own->lost += 1;
        return;
    }
}

#endif
