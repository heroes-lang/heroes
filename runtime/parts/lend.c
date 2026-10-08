/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/lend.c — the buffer C fills, lent off the stack in a guarded region
 * (design.md §1.12, §4.19; panel 196's R4, ratified 2026-10-06; defect 396).
 *
 * WHAT IT IS FOR. `SHA256_Final` writes 32 bytes through `unsigned char *md`
 * and takes no count, so no argument can say how many. A binding states it,
 * `@md: [u8] counted_by 32 lent`, and the emitter hands C a buffer of exactly
 * that extent: the array's first `min(len, extent)` elements copied in, zeros
 * after, and the array replaced by the buffer's `extent` elements after the
 * call (`selfhost/emit/lend_buffer.hero`). The extent is still the binding's
 * word, and a wrong word is what this file exists for.
 *
 * WHY OFF THE STACK, AND WHY A PAGE NOTHING MAY TOUCH. The sitting measured
 * two guards and each failing where the other held (its completeness critic):
 * a canary of a constant byte misses an overrun that writes that byte (`fgets`
 * of `0xA5`, no panic) and puts the extent on the stack (8 MiB and up, exit 139
 * with no word); one region per process gave 26 and 89 wrong digests under two
 * threads at exit 0 and could not hold an extent above a page. So the buffer
 * lives in a region of the CALLING THREAD, sized to the extent rounded up to
 * pages and grown when a larger one arrives, and ENDS where a page mapped with
 * no access begins: the first byte past the extent, read or written, faults,
 * and `parts/stack.c`'s handler finds the page here and names the call and the
 * parameter (its third witness). No fill, no compare, any overrun length.
 * Where the KERNEL writes (`read`, `pipe` on Linux), the system call fails
 * with its error instead and nothing past the extent is written: the program
 * goes on with a failed call, which the specification says.
 *
 * ONE REGION PER BUFFER, IN STACK ORDER. A call lending two buffers takes two
 * regions, and gives them back in the reverse order; a callback into Heroes
 * made while C runs (`qsort`'s comparator) that lends again takes the regions
 * above, never the one C is writing. A region is reused at its depth by the
 * next call and grown there when a larger extent arrives, which is safe
 * because nothing holds an address into a region given back: the parameter is
 * declared `lent`, C's word that it keeps nothing (panel 196's R3).
 *
 * A REGION GIVEN BACK IS WATCHED UNTIL IT IS TAKEN AGAIN (defect 451,
 * 2026-10-08). That word is C's, and a C library that breaks it was seen by
 * nothing: the region stayed readable and writable, so C reading or writing
 * through an address it kept past the call did so in silence, ASan, its free
 * fill and Guard Malloc all blind to a mapping that is not the heap. Measured
 * on this Mac at `56def9b4`, a C function keeping the address of an `@` local
 * and of a 32-byte buffer and writing through each after its function had
 * returned: exit 0 at `-O0`, `-O2` and under `--sanitize` at both; through a
 * buffer of 2,000,000 bytes, whose region was unmapped at its give-back, exit
 * 139 with no word at `-O0` and `-O2`; and through a local while the next
 * function's local held the region, that local read the 77 C wrote and not
 * the 5 its own call wrote. Three watches, by what each costs where it runs:
 *
 *   - A REGION OF A MEBIBYTE OR LESS IS SUMMED. Its lent bytes are summed when
 *     it is given back, and summed again when the next lend at its depth takes
 *     it, when its thread ends and when the program ends: a sum that moved is
 *     C writing after the lend had ended, and the panic names the last lend
 *     there. One change of one word always moves the sum (each step is a
 *     bijection of the sum so far), so no value C can write passes unseen, as
 *     a canary of a constant byte did (the head's WHY OFF THE STACK). It sees
 *     no read and it speaks late; it costs two passes over the bytes a lend
 *     copies anyway, where sealing the room costs two calls to the system a
 *     lend. Measured in instructions retired on a loop lending an `@` local
 *     and a 32-byte buffer, from 10,000 to 1,000,000 rounds: x7.8 to x18.4
 *     with every room sealed, +10 to +14% summed; a warm build of
 *     `examples/interpreter` by a compiler with every room sealed, +0.2%.
 *   - A REGION ABOVE A MEBIBYTE IS SEALED. Its pages are released when it is
 *     given back, as they were unmapped before, and its address is kept with
 *     no access, so a reach into it faults at once and the handler names it;
 *     the next lend at its depth opens it again. The same two calls to the
 *     system as the unmapping and the mapping they replace.
 *   - UNDER `--sanitize`, WHERE A RUN EXISTS TO FIND SUCH A THING, every
 *     region given back is sealed and never taken again: it goes to a
 *     quarantine of the thread's last HERO_LEND_QUARANTINE regions, the next
 *     lend at its depth maps a fresh one, and only the oldest of the quarantine
 *     is unmapped, ASan's own rule for freed heap blocks. A reach is then a
 *     fault at the C instruction that makes it, a read as well as a write, and
 *     ASan names it.
 *
 * WHAT A PLAIN BUILD CANNOT SEE is a read, and the region taken again: an
 * address C kept, written while a later lend at the same depth holds the
 * region, lands in that lend's own bytes, before any sum is taken of them.
 * The quarantine is what sees both; a sealed room everywhere would see the
 * read and still not the region taken again, at the cost above.
 *
 * THE THREAD'S, CREATED WITH ITS FIRST LEND AND RELEASED WITH ITS ALTERNATE
 * STACK. Every object here is `_Thread_local`, so two threads hashing at once
 * never share a byte; a thread maps nothing until it lends, since no size is
 * known before the first extent, and a thread this runtime started gives its
 * regions back where it gives back its signal stack (`parts/spawn.c`). The
 * thread that runs `main` keeps its regions until the process ends.
 *
 * NOTHING HERE ALLOCATES THROUGH `alloc.c`, ON PURPOSE. The table of regions
 * and the regions are mappings, `mmap` and `VirtualAlloc`, like the alternate
 * stack: they are this runtime's machinery and not a value the program owns,
 * so they stay out of the leak gate's counts, and `alloc.c`'s single point
 * keeps its meaning (`tests/harness/suite_runtime.hero` holds every `malloc`
 * family call to that file). A region above HERO_LEND_KEEP has its pages
 * released when it is given back, so a one-off buffer of 16 MiB does not stay
 * in memory; its address stays sealed, which costs address space alone.
 *
 * UNDER `--sanitize` the guard page still stops C, and AddressSanitizer, which
 * owns SIGSEGV there (`parts/stack.c`'s head), reports the fault itself. */

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

/* One region: `room` bytes the buffer is placed in, so that its `bytes` end at
 * `base + room`, then `guard` bytes mapped with no access. `callee`, `param`
 * and `extent` are what the handler names, kept after the region is given back
 * so a fault through an address C should not have kept is named too. A LOCAL
 * (panel 196's R7) holds `owner` and `name` as well, the Heroes function and
 * the binding, and is labelled with the call each time one lends it. `open`
 * says whether the room may be touched: from its take to its give, and while a
 * seal the system refused leaves it so; `sum` is its lent bytes' sum at its
 * give, where `summed` says one was taken (the head's three watches). */
typedef struct HeroLendRegion {
    char *base;
    size_t room;
    size_t guard;
    size_t bytes;
    const char *callee;
    const char *param;
    int64_t extent;
    int local;
    const char *owner;
    const char *name;
    int open;
    int summed;
    uint64_t sum;
} HeroLendRegion;

/* The calling thread's regions, in stack order: `hero_lend_depth` of them in
 * use, `hero_lend_cap` slots mapped. The table is a mapping too, doubled by a
 * copy, never `realloc`, for the head's reason. */
static _Thread_local HeroLendRegion *hero_lend_regions = NULL;
static _Thread_local int64_t hero_lend_cap = 0;
static _Thread_local int64_t hero_lend_depth = 0;

/* A region above this many bytes has its pages released and its room sealed
 * when it is given back; one of this many or fewer is summed. */
#define HERO_LEND_KEEP ((size_t)1 << 20)

/* Under the sanitizer, how many regions given back the thread keeps sealed
 * before it unmaps the oldest (the head's third watch). A local's
 * region is two pages, so 256 of them hold 2 MiB of address space on a 4 KiB
 * page and 8 MiB on a 16 KiB one, and no memory: their pages are released. */
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
#define HERO_LEND_QUARANTINE 256
static _Thread_local HeroLendRegion *hero_lend_retired = NULL;
static _Thread_local int64_t hero_lend_retired_next = 0;
#endif

static size_t hero_lend_page(void) {
#if defined(_WIN32)
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (size_t)info.dwPageSize;
#else
    long page = sysconf(_SC_PAGESIZE);
    return page > 0 ? (size_t)page : (size_t)4096;
#endif
}

/* `bytes` readable and writable, or NULL where the system refuses. */
static char *hero_lend_map(size_t bytes) {
#if defined(_WIN32)
    return (char *)VirtualAlloc(NULL, bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
    void *p = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return p == MAP_FAILED ? NULL : (char *)p;
#endif
}

static void hero_lend_unmap(char *p, size_t bytes) {
#if defined(_WIN32)
    (void)bytes;
    VirtualFree(p, 0, MEM_RELEASE);
#else
    munmap(p, bytes);
#endif
}

/* The last `guard` bytes of a mapping, no access at all. */
static int hero_lend_seal(char *p, size_t guard) {
#if defined(_WIN32)
    DWORD before = 0;
    return VirtualProtect(p, guard, PAGE_NOACCESS, &before) ? 0 : -1;
#else
    return mprotect(p, guard, PROT_NONE);
#endif
}

/* A room given back: no access at all, and with `release` its pages given to
 * the system too, the address kept (a fresh mapping of no access over it).
 * 0, or -1 where the system refused. Windows decommits every time: a page
 * committed again is zeros with the protection asked for, which is what the
 * documentation promises of a decommitted page and does not say of a page
 * whose protection was only changed; NOT RUN there (the box was off on
 * 2026-10-08), the round's leg is its first. */
static int hero_lend_close(char *p, size_t room, int release) {
#if defined(_WIN32)
    (void)release;
    return VirtualFree(p, room, MEM_DECOMMIT) ? 0 : -1;
#else
    if (release) {
        void *q = mmap(p, room, PROT_NONE, MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        return q == MAP_FAILED ? -1 : 0;
    }
    return mprotect(p, room, PROT_NONE);
#endif
}

/* A room taken again: readable and writable, its pages committed where a
 * close released them (zeros then, which the take writes over anyway). */
static int hero_lend_reopen(char *p, size_t room) {
#if defined(_WIN32)
    return VirtualAlloc(p, room, MEM_COMMIT, PAGE_READWRITE) != NULL ? 0 : -1;
#else
    return mprotect(p, room, PROT_READ | PROT_WRITE);
#endif
}

/* A message naming the call, for the refusals before C runs. */
static _Noreturn void hero_lend_refuse(const char *callee, const char *param, const char *why, int64_t value) {
    char msg[512];
    snprintf(msg, sizeof msg, "`%s` of `%s` was to be lent a buffer of %lld elements, and %s",
             param, callee, (long long)value, why);
    hero_panic(msg);
}

/* WHAT A GIVE-BACK AT THE WRONG ADDRESS SAYS. The address of a buffer or a
 * local lent to C is held in the function's frame between the take and the
 * give, and a frame can be written over: C writing past a field or an element
 * it was lent, which panel 196's R7 leaves to the binding's word, reaches
 * whatever the frame holds beside it, measured on defect 396's `md_field.hero`
 * (SHA-256's 32 bytes through one byte of a 16-byte record, its last 16 on
 * the pointer to a guarded local). So the sentence names that first and the
 * compiler second, and claims neither: it read one wrong address. */
static _Noreturn void hero_lend_misplaced(const char *what) {
    char msg[640];
    snprintf(msg, sizeof msg,
             "%s lent to C was given back at an address its region does not hold: what held the address in this "
             "function's frame was written over, by C writing past a field or an element it was lent, which "
             "is the binding's word (§ 13), or by a fault of this compiler",
             what);
    hero_panic(msg);
}

/* Room for one more region in the table, mapped and doubled by a copy. */
static void hero_lend_grow_table(void) {
    int64_t cap = hero_lend_cap < 8 ? 8 : hero_lend_cap * 2;
    size_t bytes = (size_t)cap * sizeof(HeroLendRegion);
    HeroLendRegion *fresh = (HeroLendRegion *)(void *)hero_lend_map(bytes);
    if (fresh == NULL) hero_panic("the system refused the memory for the table of buffers lent to C");
    memset(fresh, 0, bytes);
    if (hero_lend_regions != NULL) {
        memcpy(fresh, hero_lend_regions, (size_t)hero_lend_cap * sizeof(HeroLendRegion));
        hero_lend_unmap((char *)hero_lend_regions, (size_t)hero_lend_cap * sizeof(HeroLendRegion));
    }
    hero_lend_regions = fresh;
    hero_lend_cap = cap;
}

/* The sum of a region's lent bytes (the head's first watch): eight bytes at a
 * time, each step `(sum ^ word) * odd` and then the sum's high bits folded
 * into its low ones, both bijections of the sum so far, so one word changed
 * always moves the result; the bytes past the last whole word are taken one
 * at a time the same way. */
static uint64_t hero_lend_sum(const char *p, size_t n) {
    uint64_t sum = UINT64_C(0xcbf29ce484222325) ^ (uint64_t)n;
    size_t at = 0;
    for (; at + 8 <= n; at += 8) {
        uint64_t word;
        memcpy(&word, p + at, 8);
        sum = (sum ^ word) * UINT64_C(0x100000001b3);
        sum ^= sum >> 29;
    }
    for (; at < n; at++) {
        sum = (sum ^ (uint64_t)(unsigned char)p[at]) * UINT64_C(0x100000001b3);
        sum ^= sum >> 29;
    }
    return sum;
}

/* A region given back whose lent bytes no longer hold their sum: C wrote
 * through an address it kept after the lend had ended, and `when` says what
 * found it. The labels are the last lend's at that address, which is why the
 * sentence says *the last lent there* and claims no more. Never asked inside a
 * signal handler, so it may format. */
static void hero_lend_audit(HeroLendRegion *r, const char *when) {
    if (!r->summed) return;
    r->summed = 0;
    if (r->base == NULL || !r->open) return;
    if (hero_lend_sum(r->base + r->room - r->bytes, r->bytes) == r->sum) return;
    char msg[1024];
    if (r->local) {
        snprintf(msg, sizeof msg,
                 "C wrote to a local lent to it after the function that lent it had returned: the last lent "
                 "there was `%s` of `%s`%s%s%s%s%s, %s. A local lent through `@` lives until its function "
                 "returns, and C kept its address past that: what C keeps the address of must live as long "
                 "as C uses it, so it is lent from a function that runs for that long",
                 r->name != NULL ? r->name : "?", r->owner != NULL ? r->owner : "?",
                 r->callee != NULL ? ", to `" : "", r->callee != NULL && r->param != NULL ? r->param : "",
                 r->callee != NULL ? "` of `" : "", r->callee != NULL ? r->callee : "",
                 r->callee != NULL ? "`" : "", when);
    } else {
        snprintf(msg, sizeof msg,
                 "C wrote to a buffer lent to it after the call it was lent to had returned: the last lent "
                 "there was the %lld element%s lent to `%s` of `%s`, %s. The parameter is declared `lent`, "
                 "which says C keeps nothing of the address after the call, and C kept it: the declaration "
                 "is wrong about C, and a buffer lent for one call cannot serve a function that keeps it",
                 (long long)r->extent, r->extent == 1 ? "" : "s", r->param != NULL ? r->param : "?",
                 r->callee != NULL ? r->callee : "?", when);
    }
    hero_panic(msg);
}

/* Every region of the calling thread given back, audited: at the program's
 * end (`hero_runtime_check_leaks`, `hero_exit`) and its thread's
 * (`hero_lend_thread_leave`). */
static void hero_lend_audit_given(const char *when) {
    for (int64_t i = hero_lend_depth; i < hero_lend_cap; i++) hero_lend_audit(&hero_lend_regions[i], when);
}

/* The region at the next depth, holding at least `bytes` before its guard. */
static HeroLendRegion *hero_lend_push(size_t bytes, const char *callee, const char *param, int64_t extent) {
    if (hero_lend_depth >= hero_lend_cap) hero_lend_grow_table();
    HeroLendRegion *r = &hero_lend_regions[hero_lend_depth];
    hero_lend_audit(r, "found when the next lend at its depth took its memory");
    if (r->base == NULL || r->room < bytes) {
        size_t page = hero_lend_page();
        size_t room = (bytes + page - 1) / page * page;
        if (r->base != NULL) hero_lend_unmap(r->base, r->room + r->guard);
        r->base = NULL;
        r->room = 0;
        char *base = hero_lend_map(room + page);
        if (base == NULL) hero_lend_refuse(callee, param, "the system refused the memory for it", extent);
        if (hero_lend_seal(base + room, page) != 0) {
            hero_lend_unmap(base, room + page);
            hero_lend_refuse(callee, param, "the system refused to seal the page after it", extent);
        }
        r->base = base;
        r->room = room;
        r->guard = page;
        r->open = 1;
    } else if (!r->open) {
        if (hero_lend_reopen(r->base, r->room) != 0) {
            hero_lend_refuse(callee, param, "the system refused to open its memory again", extent);
        }
        r->open = 1;
    }
    r->bytes = bytes;
    r->callee = callee;
    r->param = param;
    r->extent = extent;
    r->local = 0;
    r->owner = NULL;
    r->name = NULL;
    hero_lend_depth += 1;
    return r;
}

/* The region just given back, watched (the head's three watches): summed, or
 * above a mebibyte sealed with its pages released. A seal the system refuses
 * leaves the room open, as every region was before defect 451: the program is
 * correct and only the watch is missing. A room of no bytes, a local of no
 * size, has nothing to seal, and a call with a size of zero means the whole
 * allocation to Windows' `VirtualFree`.
 *
 * Under the sanitizer the region is sealed and goes to the quarantine, and its
 * slot is emptied, so the next lend at this depth maps its own; the oldest of
 * a full quarantine is unmapped to make room. */
static void hero_lend_retire(HeroLendRegion *r) {
#if defined(HERO_LEND_QUARANTINE)
    if (hero_lend_retired == NULL) {
        size_t bytes = (size_t)HERO_LEND_QUARANTINE * sizeof(HeroLendRegion);
        hero_lend_retired = (HeroLendRegion *)(void *)hero_lend_map(bytes);
        if (hero_lend_retired != NULL) memset(hero_lend_retired, 0, bytes);
    }
    if (hero_lend_retired != NULL) {
        if (r->room > 0 && hero_lend_close(r->base, r->room, 1) == 0) r->open = 0;
        HeroLendRegion *slot = &hero_lend_retired[hero_lend_retired_next];
        if (slot->base != NULL) hero_lend_unmap(slot->base, slot->room + slot->guard);
        *slot = *r;
        hero_lend_retired_next = (hero_lend_retired_next + 1) % HERO_LEND_QUARANTINE;
        r->base = NULL;
        r->room = 0;
        r->guard = 0;
        r->open = 0;
        return;
    }
#endif
    if (r->room > HERO_LEND_KEEP) {
        if (hero_lend_close(r->base, r->room, 1) == 0) r->open = 0;
        return;
    }
    r->sum = hero_lend_sum(r->base + r->room - r->bytes, r->bytes);
    r->summed = 1;
}

int64_t hero_lend_count_u64(uint64_t n, const char *callee, const char *param) {
    if (n > (uint64_t)INT64_MAX) {
        char msg[512];
        snprintf(msg, sizeof msg,
                 "`%s` of `%s` was to be lent a buffer of %llu elements, more than this runtime can count",
                 param, callee, (unsigned long long)n);
        hero_panic(msg);
    }
    return (int64_t)n;
}

void *hero_lend_take(const HeroArrayHeader *from, int64_t extent, const char *callee, const char *param) {
    hero_array_require(from);
    if (extent < 0) hero_lend_refuse(callee, param, "an extent is a number of elements, never below zero", extent);
    size_t size = from->elem->size;
    size_t bytes = 0;
    if (__builtin_mul_overflow((size_t)extent, size, &bytes) || bytes > SIZE_MAX / 2) {
        hero_lend_refuse(callee, param, "that many bytes cannot be mapped", extent);
    }
    HeroLendRegion *r = hero_lend_push(bytes, callee, param, extent);
    char *buffer = r->base + r->room - bytes;
    int64_t kept = from->len < extent ? from->len : extent;
    size_t copied = (size_t)kept * size;
    if (copied > 0) memcpy(buffer, hero_array_data_const(from), copied);
    memset(buffer + copied, 0, bytes - copied);
    return buffer;
}

HeroArrayHeader *hero_lend_give(HeroArrayHeader *a, const void *buffer, int64_t extent) {
    hero_array_require(a);
    if (hero_lend_depth < 1) hero_panic("a buffer lent to C was given back twice — this is a compiler bug, please report it");
    HeroLendRegion *r = &hero_lend_regions[hero_lend_depth - 1];
    size_t bytes = (size_t)extent * a->elem->size;
    if (extent != r->extent || (const char *)buffer != r->base + r->room - bytes) hero_lend_misplaced("a buffer");
    HeroArrayHeader *fresh = hero_array_new(a->elem, extent);
    if (bytes > 0) memcpy(hero_array_data(fresh), buffer, bytes);
    fresh->len = extent;
    hero_array_decref(a);
    hero_lend_depth -= 1;
    hero_lend_retire(r);
    return fresh;
}

/* A LOCAL LENT TO C, IN PLACE (panel 196's R7). A binding lent whole through
 * `@` to an `extern` call lives in a region of its own for its function's life
 * instead of in the C frame: the emitter declares a pointer to it in the
 * prologue, names the binding through that pointer, labels the region before
 * each call that lends it, and gives it back at the function's one exit, so
 * regions stay in stack order with the buffers of R4 above and below them. C
 * is handed the local itself, never a copy, and one byte past it is the guard
 * page. Zeroed, which is the value a refcounted slot starts at and the one the
 * frame would not have given any other. */
void *hero_lend_local(size_t size, const char *owner, const char *name) {
    HeroLendRegion *r = hero_lend_push(size, owner, name, 1);
    char *local = r->base + r->room - size;
    memset(local, 0, size);
    r->local = 1;
    r->owner = owner;
    r->name = name;
    r->callee = NULL;
    r->param = NULL;
    return local;
}

/* Which call is lending it now: a local outlives one call, and a fault is
 * named by the call that made it. Asked also of an `@` parameter's pointer
 * inside a Heroes function that hands it on to C, which is its caller's place:
 * a guarded local's region where the caller lent one, and otherwise a field,
 * an element or an unguarded place, which no region holds and this leaves
 * alone. */
void hero_lend_local_name(void *local, const char *callee, const char *param) {
    for (int64_t i = hero_lend_depth - 1; i >= 0; i--) {
        HeroLendRegion *r = &hero_lend_regions[i];
        if (r->local && r->base + r->room - r->bytes == (char *)local) {
            r->callee = callee;
            r->param = param;
            return;
        }
    }
}

void hero_lend_local_give(void *local) {
    if (hero_lend_depth < 1) hero_panic("a local lent to C was given back twice — this is a compiler bug, please report it");
    HeroLendRegion *r = &hero_lend_regions[hero_lend_depth - 1];
    if (!r->local || r->base + r->room - r->bytes != (char *)local) hero_lend_misplaced("a local");
    hero_lend_depth -= 1;
    hero_lend_retire(r);
}

/* Whether `addr` is in `r`: on its guard page, or in its room while the room
 * is sealed. The answer, with the region's labels, into `out`. */
static int hero_lend_fault_in(const HeroLendRegion *r, uintptr_t addr, int returned, HeroLendFault *out) {
    if (r->base == NULL) return 0;
    uintptr_t room = (uintptr_t)r->base;
    uintptr_t guard = room + r->room;
    int on_guard = addr >= guard && addr - guard < r->guard;
    int inside = !r->open && addr >= room && addr < guard;
    if (!on_guard && !inside) return 0;
    out->callee = r->callee;
    out->param = r->param;
    out->extent = r->extent;
    out->returned = returned || inside;
    out->inside = inside;
    out->local = r->local;
    out->owner = r->owner;
    out->name = r->name;
    return 1;
}

/* THE HANDLER'S QUESTION, asked on the faulting thread, which is the thread
 * whose regions these are: is the address on one of their guard pages, or in
 * a room given back and sealed? A region in use first, the innermost; then one
 * given back, which C reached through an address it was not to keep; then,
 * under the sanitizer, the quarantine. Reads only: no call, no allocation, so
 * it may run inside a signal handler (C11 7.14.1.1). */
__attribute__((unused)) static int hero_lend_fault_at(uintptr_t addr, HeroLendFault *out) {
    for (int64_t i = hero_lend_cap - 1; i >= 0; i--) {
        if (hero_lend_fault_in(&hero_lend_regions[i], addr, i >= hero_lend_depth, out)) return 1;
    }
#if defined(HERO_LEND_QUARANTINE)
    if (hero_lend_retired != NULL) {
        for (int64_t i = 0; i < HERO_LEND_QUARANTINE; i++) {
            if (hero_lend_fault_in(&hero_lend_retired[i], addr, 1, out)) return 1;
        }
    }
#endif
    return 0;
}

/* A count in decimal with no call at all, for the handler's message. */
static void hero_lend_decimal(int64_t v, char buf[24]) {
    char rev[24];
    int n = 0;
    uint64_t u = v < 0 ? (uint64_t)0 - (uint64_t)v : (uint64_t)v;
    do {
        rev[n++] = (char)('0' + (int)(u % 10));
        u /= 10;
    } while (u > 0 && n < 22);
    int at = 0;
    if (v < 0) buf[at++] = '-';
    while (n > 0) buf[at++] = rev[--n];
    buf[at] = '\0';
}

/* The sentence, written through the platform's own writer, which is all a
 * signal handler or a vectored exception handler may call.
 *
 * AFTER THE LEND HAS ENDED, the labels are those of the LAST lend at that
 * address, which is the one C kept only where no later lend took the region
 * (always under the sanitizer's quarantine, not otherwise), so the sentence
 * says *the last lent there* and claims no more. A local and a buffer are told
 * apart, since a local's parameter need not be declared `lent` and the words
 * for a buffer say it is (defect 451; until then a local reached past after its
 * function had returned was told it was a buffer). */
__attribute__((unused)) static void hero_lend_tell(const HeroLendFault *f, void (*say)(const char *)) {
    char digits[24];
    hero_lend_decimal(f->extent, digits);
    const char *callee = f->callee != NULL ? f->callee : "?";
    const char *param = f->param != NULL ? f->param : "?";
    if (f->returned && f->local) {
        say(f->inside ? "panic: C reached a local lent to it" : "panic: C reached past a local lent to it");
        say(" after the function that lent it had returned: the last lent there was `");
        say(f->name != NULL ? f->name : "?");
        say("` of `");
        say(f->owner != NULL ? f->owner : "?");
        if (f->callee != NULL) {
            say("`, to `");
            say(param);
            say("` of `");
            say(callee);
        }
        say("`\n");
        say("  A local lent through `@` lives until its function returns, and C kept its address past\n");
        say("  that: what C keeps the address of must live as long as C uses it, so it is lent from a\n");
        say("  function that runs for that long.\n");
        return;
    }
    if (f->returned) {
        say(f->inside ? "panic: C reached a buffer lent to it" : "panic: C reached past a buffer lent to it");
        say(" after the call it was lent to had returned: the last lent there was the ");
        say(digits);
        say(f->extent == 1 ? " element lent to `" : " elements lent to `");
        say(param);
        say("` of `");
        say(callee);
        say("`\n");
        say("  The parameter is declared `lent`, which says C keeps nothing of the address after the\n");
        say("  call, and C kept it: the declaration is wrong about C, and a buffer lent for one call\n");
        say("  cannot serve a function that keeps it.\n");
        return;
    }
    if (f->local) {
        say("panic: C reached past the local `");
        say(f->name != NULL ? f->name : "?");
        say("` of `");
        say(f->owner != NULL ? f->owner : "?");
        say("`, the one element lent to `");
        say(param);
        say("` of `");
        say(callee);
        say("`\n");
        say("  An `@` parameter promises C one element, and the local ends at a page nothing may touch,\n");
        say("  so C stopped there and changed nothing beside it. Where C writes several, the binding\n");
        say("  lends an array with the extent C writes: `@");
        say(param);
        say(": [T] counted_by <n> lent`.\n");
        return;
    }
    say("panic: C reached past the ");
    say(digits);
    say(f->extent == 1 ? " element lent to `" : " elements lent to `");
    say(param);
    say("` of `");
    say(callee);
    say("`, the extent its declaration states\n");
    say("  The buffer ends at a page nothing may touch, so C stopped there and changed nothing beside\n");
    say("  it. C reaches as far as the header's function does, not as far as the declaration says:\n");
    say("  state the extent it writes, or name the sibling that tells it how many.\n");
}

/* Given back with the thread's alternate stack, by the thread that took them,
 * the quarantine's with them. */
static void hero_lend_thread_leave(void) {
    hero_lend_audit_given("found when its thread ended");
    for (int64_t i = 0; i < hero_lend_cap; i++) {
        HeroLendRegion *r = &hero_lend_regions[i];
        if (r->base != NULL) hero_lend_unmap(r->base, r->room + r->guard);
    }
    if (hero_lend_regions != NULL) {
        hero_lend_unmap((char *)hero_lend_regions, (size_t)hero_lend_cap * sizeof(HeroLendRegion));
    }
    hero_lend_regions = NULL;
    hero_lend_cap = 0;
    hero_lend_depth = 0;
#if defined(HERO_LEND_QUARANTINE)
    if (hero_lend_retired != NULL) {
        for (int64_t i = 0; i < HERO_LEND_QUARANTINE; i++) {
            HeroLendRegion *r = &hero_lend_retired[i];
            if (r->base != NULL) hero_lend_unmap(r->base, r->room + r->guard);
        }
        hero_lend_unmap((char *)hero_lend_retired, (size_t)HERO_LEND_QUARANTINE * sizeof(HeroLendRegion));
    }
    hero_lend_retired = NULL;
    hero_lend_retired_next = 0;
#endif
}
