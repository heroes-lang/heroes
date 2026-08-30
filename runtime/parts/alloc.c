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
 * (panel 021, measured, with a 999-block leak exiting 0 in silence).
 *
 * **Scratch is counted too, since 2026-08-30, and this paragraph used to argue
 * the opposite.** It said a scratch buffer is not a Heroes block, that counting
 * it would be harmless and that *forgetting to uncount it* would report a leak
 * that is not one. The second half was the load-bearing one and it does not hold:
 * the decrement lives inside `hero_release`, exactly as the block decrement lives
 * inside `hero_release_block`, so forgetting it is not a thing a caller can do.
 * What the old shape did allow was the opposite error, and panel 098's
 * ffi-pragmatist measured it: a new runtime part that allocates scratch and keeps
 * it past the call leaks, and **both** of this project's instruments stay silent
 * — the gate because it counted only blocks, ASan because it has no leak detector
 * here. Deleting one `free` from a prototype gave exit 0, empty stderr, and a
 * quiet gate. Two counters now, reported separately, because *which* one is
 * unbalanced says whether the bug is a missing decref in the language or a
 * missing release in the runtime.
 *
 * The counted pair is still spelled differently from the scratch pair, and
 * neither can be reached by accident; what changed is that both are now weighed.
 *
 * The counter is `static` and stays so: that is what a decoy runtime linked
 * beside this one cannot reach, and it is why `runtime.c` is one translation
 * unit (its own header explains the rest).
 */

/* live heap-block balance: the leak detector that works on this platform */
static int64_t hero_live_blocks = 0;

/* live scratch balance: what a runtime call borrowed and has not given back */
static int64_t hero_live_scratch = 0;

int64_t hero_runtime_live(void) { return hero_live_blocks; }

void hero_runtime_check_leaks(void) {
    if (hero_live_blocks != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld heap blocks still live at exit "
                        "(a missing decref) — this is a compiler bug\n",
                (long long)hero_live_blocks);
        abort();
    }
    if (hero_live_scratch != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld scratch buffers still live at exit "
                        "(a runtime call kept what it borrowed) — this is a "
                        "runtime bug\n",
                (long long)hero_live_scratch);
        abort();
    }
}

/* The one `malloc`, weighed by neither counter. Both pairs below go through it,
 * so §4.20's single point stays literally true and a block is not also counted
 * as scratch on its way past. It aborts rather than returning NULL, so no caller
 * anywhere writes an out-of-memory branch. */
static void *hero_malloc_raw(size_t size) {
    void *p = malloc(size);
    if (p == NULL) hero_panic("out of memory");
    return p;
}

/* Scratch memory, borrowed by one runtime call and given back before it returns.
 * Counted since 2026-08-30 — the header says why. */
static void *hero_alloc(size_t size) {
    void *p = hero_malloc_raw(size);
    hero_live_scratch += 1;
    return p;
}

static void hero_release(void *p) {
    hero_live_scratch -= 1;
    free(p);
}

/* A block the language owns: a `str`'s bytes, an array's elements, a map's
 * table. Counted, and the count is the leak gate. */
static void *hero_alloc_block(size_t size) {
    void *p = hero_malloc_raw(size);
    hero_live_blocks += 1;
    return p;
}

static void hero_release_block(void *p) {
    hero_live_blocks -= 1;
    free(p);
}
