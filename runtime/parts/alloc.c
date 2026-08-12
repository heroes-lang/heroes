/* parts/alloc.c — the single allocation point, and the live-block counter.
 *
 * design.md §4.20 has asked for this since day zero — *"the allocator (a wrapper
 * over `malloc`) — and **a single point** … Not tidiness: Part 7.13's per-thread
 * heaps need one place to change, and a second allocation site discovered later
 * is a redesign"* — and the document then said the single point *"today is
 * literally one `malloc` and one `free`, both inside the `str` primitives"*.
 *
 * That sentence was false when it was written and got worse: by M-ffi-ladder
 * there were **eleven** calls across five files (`str`, the array, the map,
 * `sort`'s scratch buffer, and the file reader). This file is the sentence made
 * true. Every `malloc` and every `free` in the runtime is here; nothing else in
 * `runtime/parts/` may call either, and the invariant is checkable with one grep.
 *
 * TWO PAIRS, NOT ONE, AND THE SPLIT IS THE LEAK GATE'S.
 *
 * `hero_live_blocks` counts what the language owns: a `str`, an array, a map. It
 * is what `hero_runtime_check_leaks()` asserts at exit, and on Darwin arm64 it is
 * the *only* leak instrument there is — AddressSanitizer's does not exist here
 * (panel 021, measured, with a 999-block leak exiting 0 in silence). A scratch
 * buffer that is allocated and freed inside one runtime call is not a Heroes
 * block: counting it would be harmless, and *forgetting* to uncount it would
 * report a leak that is not one. So the counted pair is spelled differently from
 * the uncounted one, and neither can be reached by accident.
 *
 * The counter is `static` and stays so: that is what a decoy runtime linked
 * beside this one cannot reach, and it is why `runtime.c` is one translation
 * unit (its own header explains the rest).
 */

/* live heap-block balance: the leak detector that works on this platform */
static int64_t hero_live_blocks = 0;

int64_t hero_runtime_live(void) { return hero_live_blocks; }

void hero_runtime_check_leaks(void) {
    if (hero_live_blocks != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld heap blocks still live at exit "
                        "(a missing decref) — this is a compiler bug\n",
                (long long)hero_live_blocks);
        abort();
    }
}

/* Scratch memory, owned by one runtime call and released before it returns.
 * Never counted — see the header. `hero_alloc` aborts rather than returning
 * NULL, so no caller writes an out-of-memory branch. */
static void *hero_alloc(size_t size) {
    void *p = malloc(size);
    if (p == NULL) hero_panic("out of memory");
    return p;
}

static void hero_release(void *p) { free(p); }

/* A block the language owns: a `str`'s bytes, an array's elements, a map's
 * table. Counted, and the count is the leak gate. */
static void *hero_alloc_block(size_t size) {
    void *p = hero_alloc(size);
    hero_live_blocks += 1;
    return p;
}

static void hero_release_block(void *p) {
    hero_live_blocks -= 1;
    hero_release(p);
}
