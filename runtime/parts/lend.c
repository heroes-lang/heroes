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
 * family call to that file). A region above HERO_LEND_KEEP is unmapped when it
 * is given back, so a one-off buffer of 16 MiB does not stay mapped.
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
 * the binding, and is labelled with the call each time one lends it. */
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
} HeroLendRegion;

/* The calling thread's regions, in stack order: `hero_lend_depth` of them in
 * use, `hero_lend_cap` slots mapped. The table is a mapping too, doubled by a
 * copy, never `realloc`, for the head's reason. */
static _Thread_local HeroLendRegion *hero_lend_regions = NULL;
static _Thread_local int64_t hero_lend_cap = 0;
static _Thread_local int64_t hero_lend_depth = 0;

/* A region above this many bytes is unmapped when it is given back. */
#define HERO_LEND_KEEP ((size_t)1 << 20)

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

/* The region at the next depth, holding at least `bytes` before its guard. */
static HeroLendRegion *hero_lend_push(size_t bytes, const char *callee, const char *param, int64_t extent) {
    if (hero_lend_depth >= hero_lend_cap) hero_lend_grow_table();
    HeroLendRegion *r = &hero_lend_regions[hero_lend_depth];
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
    if (r->room > HERO_LEND_KEEP) {
        hero_lend_unmap(r->base, r->room + r->guard);
        r->base = NULL;
        r->room = 0;
        r->guard = 0;
    }
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
    if (r->room > HERO_LEND_KEEP) {
        hero_lend_unmap(r->base, r->room + r->guard);
        r->base = NULL;
        r->room = 0;
        r->guard = 0;
    }
}

/* THE HANDLER'S QUESTION, asked on the faulting thread, which is the thread
 * whose regions these are: is the address on one of their guard pages? A
 * region in use first, the innermost; then one given back and still mapped,
 * which C reached through an address it was not to keep. Reads only: no call,
 * no allocation, so it may run inside a signal handler (C11 7.14.1.1). */
__attribute__((unused)) static int hero_lend_fault_at(uintptr_t addr, HeroLendFault *out) {
    for (int64_t i = hero_lend_cap - 1; i >= 0; i--) {
        HeroLendRegion *r = &hero_lend_regions[i];
        if (r->base == NULL) continue;
        uintptr_t guard = (uintptr_t)r->base + r->room;
        if (addr >= guard && addr - guard < r->guard) {
            out->callee = r->callee;
            out->param = r->param;
            out->extent = r->extent;
            out->returned = i >= hero_lend_depth;
            out->local = r->local;
            out->owner = r->owner;
            out->name = r->name;
            return 1;
        }
    }
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
 * signal handler or a vectored exception handler may call. */
__attribute__((unused)) static void hero_lend_tell(const HeroLendFault *f, void (*say)(const char *)) {
    char digits[24];
    hero_lend_decimal(f->extent, digits);
    const char *callee = f->callee != NULL ? f->callee : "?";
    const char *param = f->param != NULL ? f->param : "?";
    if (f->returned) {
        say("panic: C reached past the buffer lent to `");
        say(param);
        say("` of `");
        say(callee);
        say("` after that call had returned\n");
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

/* Given back with the thread's alternate stack, by the thread that took them. */
static void hero_lend_thread_leave(void) {
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
}
