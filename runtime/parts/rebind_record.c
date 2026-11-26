/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind_record.c — Darwin's route D-rebind, part of `parts/rebind.c`
 * (read its module doc first): route A: the records of the save points, for a
 * family whose decode did not verify (panel 255's R1, defects 2679, 2702 and
 * 2745; design.md §1.12, §4.19). Needs the files `rebind.c` names before it.
 *
 * A trusted record's jump is judged by rules 4 to 7 as route C judges a
 * decoded target (defect 2745), on the frame the save point recorded just
 * below the stack pointer the C library saved, on the same stack
 * (`hero_rebind_below`): a bound below the true target leaves no entry the
 * true target would keep, since the two lie less than a signal frame apart and
 * a handler's frame between them was delivered above the save point's caller,
 * whose frame a jump reaches only after it returned. Whether the landing lies
 * below the jumping frame, which a bound cannot say, is the record naming a
 * handler no longer standing. Until defect 2745 the serial alone judged it,
 * every entry pushed after the save left whatever stack the save was made on,
 * and panel 255's A'' ran the program's function inside the signal. */

#if defined(__APPLE__)

/* A buffer, its thread, the serial of the innermost entry standing at its
 * save (0 for none), the frame below the stack pointer it saved, and its
 * fingerprint once read. Written and read with no
 * lock: a writer claims a record by moving its buffer word to 1, a reader
 * trusts one only where the buffer word reads the same before and after; any
 * race is a miss, and a miss keeps every entry. A buffer's home is a hash of
 * it and its thread, probed HERO_REBIND_PROBES deep. */
#define HERO_REBIND_RECORDS 4096
#define HERO_REBIND_PROBES 8
/* The bytes a fingerprint reads: Darwin arm64's `jmp_buf` is 48 `int`s. */
#define HERO_REBIND_PRINT_BYTES 192

typedef struct {
    _Atomic uintptr_t buf;
    _Atomic uintptr_t thread;
    _Atomic uint64_t entry;
    _Atomic uintptr_t sp;
    _Atomic uint64_t print;
    _Atomic int printed;
} HeroRebindRecord;

static HeroRebindRecord hero_rebind_records[HERO_REBIND_RECORDS];

/* The one buffer this thread saved into and has not yet fingerprinted, in a
 * key's slot. */
static _Atomic pthread_key_t hero_rebind_pending;
static _Atomic int hero_rebind_pending_keyed = 0;

static size_t hero_rebind_home(uintptr_t buf, uintptr_t thread) {
    uint64_t h = (uint64_t)buf * UINT64_C(0x9E3779B97F4A7C15) ^ (uint64_t)thread * UINT64_C(0xC2B2AE3D27D4EB4F);
    return (size_t)(h >> 52) & (HERO_REBIND_RECORDS - 1);
}

static HeroRebindRecord *hero_rebind_probe(size_t home, int i) {
    return &hero_rebind_records[(home + (size_t)i) & (HERO_REBIND_RECORDS - 1)];
}

/* A fingerprint read only of a buffer that provably lives (rule 3), into
 * `*print`; false where it could not be read. The bytes are copied by the
 * kernel (`mach_vm_read_overwrite` of this task), never loaded by this thread:
 * memory that provably lives may still have been made unreadable by the
 * program since the save (a static page `mprotect`ed to none: a Bus error at
 * the next save point, measured with `HERO_JMP_FORCE_UNCALIBRATED` 3 on this
 * Mac, defect d-lane-d2745-1's class), and a copy the kernel refuses is an
 * answer, never a fault. Nor is it a program's access, so AddressSanitizer
 * does not see it: a returned frame's bytes may sit under a later frame's
 * redzone. */
static bool hero_rebind_fingerprint(const void *buf, uint64_t *print) {
    unsigned char copy[HERO_REBIND_PRINT_BYTES];
    mach_vm_size_t got = 0;
    if (mach_vm_read_overwrite(mach_task_self(), (mach_vm_address_t)(uintptr_t)buf, sizeof copy,
                               (mach_vm_address_t)(uintptr_t)copy, &got) != KERN_SUCCESS ||
        got != sizeof copy) {
        return false;
    }
    uint64_t h = UINT64_C(1469598103934665603);
    for (size_t i = 0; i < sizeof copy; i += 1) {
        h ^= copy[i];
        h *= UINT64_C(1099511628211);
    }
    *print = h;
    return true;
}

static void hero_rebind_write(HeroRebindRecord *r, uintptr_t was, uintptr_t buf, uintptr_t thread, uint64_t entry,
                              uintptr_t frame) {
    if (!atomic_compare_exchange_strong(&r->buf, &was, (uintptr_t)1)) return;
    atomic_store(&r->thread, thread);
    atomic_store(&r->entry, entry);
    atomic_store(&r->sp, frame);
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

/* The writable segments of the images loaded before `main`, which stay mapped
 * while the process runs; written once, before `main`. */
#define HERO_REBIND_REGIONS 1024
static _Atomic uintptr_t hero_rebind_region_lo[HERO_REBIND_REGIONS];
static _Atomic uintptr_t hero_rebind_region_hi[HERO_REBIND_REGIONS];
static _Atomic int hero_rebind_regions = 0;

static void hero_rebind_regions_of(const struct mach_header *header, intptr_t slide) {
    const struct mach_header_64 *mh = (const struct mach_header_64 *)header;
    const uint8_t *p = (const uint8_t *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i += 1) {
        const struct load_command *lc = (const struct load_command *)p;
        p += lc->cmdsize;
        if (lc->cmd != LC_SEGMENT_64) continue;
        const struct segment_command_64 *seg = (const struct segment_command_64 *)lc;
        if ((seg->initprot & VM_PROT_WRITE) == 0 || seg->vmsize == 0) continue;
        int n = atomic_load(&hero_rebind_regions);
        if (n == HERO_REBIND_REGIONS) return;
        atomic_store(&hero_rebind_region_lo[n], (uintptr_t)slide + (uintptr_t)seg->vmaddr);
        atomic_store(&hero_rebind_region_hi[n], (uintptr_t)slide + (uintptr_t)seg->vmaddr + (uintptr_t)seg->vmsize);
        atomic_store(&hero_rebind_regions, n + 1);
    }
}

/* Whether `[buf, buf + size)` lies on this thread's stack, on the alternate
 * stack it is running on, or in a writable segment of an image loaded before
 * `main`. */
static bool hero_rebind_lives(const void *buf, size_t size) {
    uintptr_t lo = (uintptr_t)buf, hi = lo + size;
    pthread_t self = pthread_self();
    uintptr_t top = (uintptr_t)pthread_get_stackaddr_np(self);
    if (lo >= top - pthread_get_stacksize_np(self) && hi <= top) return true;
    int n = atomic_load(&hero_rebind_regions);
    for (int i = 0; i < n; i += 1) {
        if (lo >= atomic_load(&hero_rebind_region_lo[i]) && hi <= atomic_load(&hero_rebind_region_hi[i])) return true;
    }
    stack_t alt;
    return sigaltstack(NULL, &alt) == 0 && (alt.ss_flags & SS_ONSTACK) != 0 && lo >= (uintptr_t)alt.ss_sp &&
           hi <= (uintptr_t)alt.ss_sp + alt.ss_size;
}

/* The pending buffer fingerprinted where it provably lives; where it does
 * not, its record is marked one no jump trusts (2). */
static void hero_rebind_settle(void) {
    if (!atomic_load_explicit(&hero_rebind_pending_keyed, memory_order_acquire)) return;
    pthread_key_t key = atomic_load_explicit(&hero_rebind_pending, memory_order_relaxed);
    void *buf = pthread_getspecific(key);
    if (buf == NULL) return;
    pthread_setspecific(key, NULL);
    HeroRebindRecord *r = hero_rebind_record(buf);
    if (r == NULL) return;
    uint64_t print = 0;
    if (!hero_rebind_lives(buf, HERO_REBIND_PRINT_BYTES) || !hero_rebind_fingerprint(buf, &print)) {
        atomic_store(&r->printed, 2);
        return;
    }
    atomic_store(&r->print, print);
    atomic_store(&r->printed, 1);
}

/* The serial of the innermost entry standing, 0 for none. */
static uint64_t hero_rebind_innermost(void) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL) return 0;
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        if (!own->entries[i].dead) return own->entries[i].serial;
    }
    return 0;
}

/* A frame just below its caller's stack pointer, on the same stack: the
 * bound route A records of where a save point's jump will land. Never
 * inlined, so the frame is its own. */
__attribute__((noinline)) static uintptr_t hero_rebind_below(void) {
    uintptr_t frame = (uintptr_t)__builtin_frame_address(0);
    __asm__ volatile("" ::: "memory");
    return frame;
}

/* A save into `buf`, `frame` below the stack pointer it saves: its record
 * taken back where it has one, else a free record near its home, else the
 * home overwritten. */
static void hero_rebind_note(const void *buf, uintptr_t frame) {
    if (buf == NULL) return;
    /* The thread runs at the save (defect 2855, `rebind_entries.c`). */
    hero_rebind_relived(hero_rebind_own(), frame);
    hero_rebind_settle();
    uint64_t entry = hero_rebind_innermost();
    uintptr_t b = (uintptr_t)buf;
    uintptr_t t = (uintptr_t)pthread_self();
    size_t home = hero_rebind_home(b, t);
    HeroRebindRecord *free_one = NULL;
    int written = 0;
    for (int i = 0; i < HERO_REBIND_PROBES && !written; i += 1) {
        HeroRebindRecord *r = hero_rebind_probe(home, i);
        uintptr_t v = atomic_load_explicit(&r->buf, memory_order_acquire);
        if (v == b && atomic_load(&r->thread) == t) {
            hero_rebind_write(r, b, b, t, entry, frame);
            written = 1;
        } else if (v == 0 && free_one == NULL) {
            free_one = r;
        }
    }
    if (!written) {
        if (free_one != NULL) {
            hero_rebind_write(free_one, 0, b, t, entry, frame);
        } else {
            HeroRebindRecord *r = hero_rebind_probe(home, 0);
            uintptr_t v = atomic_load(&r->buf);
            if (v > 1) hero_rebind_write(r, v, b, t, entry, frame);
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

/* Whether a record naming `entry` names a handler that no longer stands: the
 * save was made inside a handler the thread has left since, so a jump to it
 * lands in frames already left (shape R). */
static bool hero_rebind_left_since(const HeroSignalStack *own, uint64_t entry) {
    if (entry == 0) return false;
    for (int i = 0; i < own->depth; i += 1) {
        if (!own->entries[i].dead && own->entries[i].serial == entry) return false;
    }
    return true;
}

/* Route A's target for a jump to `buf` (rule 3, then rules 4 to 7, defect
 * 2745): true where the record is trusted, with `*t` the frame it holds and
 * `*back` whether it names a handler left since. */
static bool hero_rebind_target(HeroSignalStack *own, const void *buf, uintptr_t *t, bool *back) {
    hero_rebind_settle();
    HeroRebindRecord *r = hero_rebind_record(buf);
    uintptr_t b = (uintptr_t)buf;
    int printed = r != NULL ? atomic_load(&r->printed) : 0;
    uint64_t now = 0;
    bool trusted = r != NULL && printed != 2 &&
                   !(printed == 1 && (!hero_rebind_fingerprint(buf, &now) || atomic_load(&r->print) != now));
    uint64_t entry = trusted ? atomic_load(&r->entry) : 0;
    uintptr_t frame = trusted ? atomic_load(&r->sp) : 0;
    if (!trusted || frame == 0 || atomic_load_explicit(&r->buf, memory_order_acquire) != b) return false;
    *t = frame;
    *back = hero_rebind_left_since(own, entry);
    return true;
}

/* Route A's landing at `t`, a bound of the target the record trusts: a
 * re-entry where the record names a handler left since, or the landing lies
 * on the alternate stack from outside it. */
static void hero_rebind_landed(HeroSignalStack *own, uintptr_t t, bool back) {
    hero_rebind_reentered(own, t, back, (uintptr_t)__builtin_frame_address(0));
}

#endif
