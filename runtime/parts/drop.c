/* Releasing a deep value without a deep C stack (panel 070, provisional
 * 2026-08-16; design.md §1.12).
 *
 * THE DEFECT. Releasing a `record Node { children: [Node] }` chain recursed once
 * per level — `hero_array_decref` -> the element's `drop` -> the generated
 * `Node_release` -> `hero_array_decref` — and ran out of C stack. Measured on the
 * unmodified runtime, Darwin arm64, 8 MB main thread: **exit 139 at depth 74642
 * (-O0) and at 174003 (-O2), silently**. No diagnostic, no message, no line.
 * That is the one outcome design.md §1.12 names as forbidden, reachable from
 * nine lines of Heroes.
 *
 * `-O2` IS NOT A FIX AND NEVER COULD HAVE BEEN, which the sitting had to measure
 * because the brief claimed otherwise. The recursion depth is identical at both
 * levels — 20001 at depth 20000, clang eliminates zero calls — and only the frame
 * shrinks as the generated functions inline. The call is inside a `for` loop and
 * is followed by `hero_release_block`, so it is not in tail position: no C
 * compiler is permitted to turn it into a jump.
 *
 * THE FIX NEEDS NO STORAGE, and that is what makes it affordable in a release
 * path. A block whose refcount has reached zero is doomed, so its `refcount`
 * field is dead — the link lives there. `HeroArrayHeader` and `HeroMapHeader`
 * both open with `int64_t refcount`, so both can be threaded the same way.
 * Layout is unchanged and `HERO_RUNTIME_ABI` does not move: CLAUDE.md §7 makes
 * the stamp a *declaration* stamp, and no declaration changes here. What catches
 * the behaviour change is the cache key, which hashes every file under
 * `runtime/`.
 *
 * WHAT THIS DOES NOT COVER, stated rather than left to be discovered: `==` and
 * `hash` still recurse, and still crash — measured at depth 52043, which is
 * *lower* than the drop path this repairs. They cannot be fixed the same way: a
 * generated `_eq` compares one field and then descends, so deferring the descent
 * means resuming in the middle of a function, which is a continuation. Panel 070's
 * compiler-engineer vetoed that on design.md §1.7's ceiling — it lands in the
 * emitter and the Heroes port must then reproduce it — and the alternative it
 * approved, a stack-remaining probe, took a veto from the ffi-pragmatist on
 * portability it could not verify. The hole is on `DECIDE.md` and is live.
 *
 * `_Thread_local` FROM THE START. Panel 070's ffi-pragmatist made this a
 * condition for `hero_spawn`: two threads releasing concurrently would splice
 * each other's lists and free the wrong blocks, which is the §1.12 class this
 * file exists to close. There are no threads today, so process-globals would be
 * correct today and wrong on the day a queued item was missed. C11 has the
 * keyword; using it now costs nothing and removes the deadline. */

/* TWO LISTS AND ONE FLAG, and the alternative was a defect I wrote and caught
 * before it ran. The obvious shape is one list with a tag saying which kind of
 * header each entry is — and there is nowhere to put the tag. `refcount` holds
 * the link; the next field is `len`, and `len` is exactly what the walk needs to
 * know how many elements to release. Storing a tag there destroys the block on
 * the way to freeing it.
 *
 * So: one list per header kind, each threaded through its own dead `refcount`,
 * and **one shared `running` flag**. The flag is what prevents the recursion —
 * whichever entry point is first becomes the driver and every nested release
 * defers to it, whatever kind it is — and the driver drains both lists until both
 * are empty, so an alternating `[{i64: [T]}]` chain unwinds iteratively. Two
 * flags would not have worked: an array reached from inside a map would find the
 * array flag lowered and recurse. */
static _Thread_local HeroArrayHeader *hero_drop_arrays = NULL;
static _Thread_local HeroMapHeader *hero_drop_maps = NULL;
static _Thread_local bool hero_drop_running = false;

_Static_assert(sizeof(void *) <= sizeof(HeroRefcount),
               "the pending link is stored in a doomed block's dead refcount field");

/* `memcpy` rather than a cast: a pointer parked in an `int64_t` through a cast is
 * `-Wstrict-aliasing` territory, and `FLAGS` carries `-Werror` on more than one
 * aliasing warning. Every compiler folds it to a store.
 *
 * AND IT STAYS A `memcpy` NOW THAT THE FIELD IS `_Atomic` (M-isolated-threads
 * step 3), which is worth a sentence because it looks like exactly the mistake
 * this project spends tokens to avoid: a plain byte-copy into an atomic object.
 * It is not one, and the reason is what a block on these lists IS. A block
 * reaches here only when its count went to zero, which means the thread holding
 * this list holds the last reference to it — no other thread has a pointer to
 * hand, so there is no second party for an atomic to order against. The field is
 * not a count any more at that point; it is scratch space in a block on its way
 * out, and the two lists and their flag are `_Thread_local` so the scratch is
 * this thread's.
 * An atomic store here would order this thread against itself and buy nothing.
 * The assert above is what keeps the space wide enough for the pointer. */
static void hero_drop_push_array(HeroArrayHeader *a) {
    HeroArrayHeader *next = hero_drop_arrays;
    memcpy(&a->refcount, &next, sizeof next);
    hero_drop_arrays = a;
}

static HeroArrayHeader *hero_drop_pop_array(void) {
    HeroArrayHeader *a = hero_drop_arrays;
    if (a == NULL) return NULL;
    HeroArrayHeader *next;
    memcpy(&next, &a->refcount, sizeof next);
    hero_drop_arrays = next;
    return a;
}

static void hero_drop_push_map(HeroMapHeader *m) {
    HeroMapHeader *next = hero_drop_maps;
    memcpy(&m->refcount, &next, sizeof next);
    hero_drop_maps = m;
}

static HeroMapHeader *hero_drop_pop_map(void) {
    HeroMapHeader *m = hero_drop_maps;
    if (m == NULL) return NULL;
    HeroMapHeader *next;
    memcpy(&next, &m->refcount, sizeof next);
    hero_drop_maps = next;
    return m;
}

/* The two walks, defined where each container's internals are — `array.c` and
 * `map.c` — and forward-declared here because this file is included before both.
 * One translation unit, so a forward declaration is all it takes.
 *
 * Each releases what its block HOLDS and then the block, and neither recurses:
 * anything they release that is itself a container finds `hero_drop_running`
 * raised and hands its block to a list instead. */
static void hero_array_release_contents(HeroArrayHeader *a);
static void hero_map_release_contents(HeroMapHeader *m);

/* Drain both lists until both are empty.
 *
 * **Both, and re-checked after each**, because releasing an array's elements can
 * add maps and releasing a map's entries can add arrays. A single pass over one
 * list and then the other would leave whatever the second pass produced for the
 * first — which is the alternating case this file exists for, arriving as a leak
 * rather than as a crash and therefore quieter than the defect it replaces. */
static void hero_drop_drain(void) {
    for (;;) {
        HeroArrayHeader *a = hero_drop_pop_array();
        if (a != NULL) {
            hero_array_release_contents(a);
            continue;
        }
        HeroMapHeader *m = hero_drop_pop_map();
        if (m != NULL) {
            hero_map_release_contents(m);
            continue;
        }
        return;
    }
}

static void hero_drop_drain_from_array(HeroArrayHeader *a) {
    hero_array_release_contents(a);
    hero_drop_drain();
}

static void hero_drop_drain_from_map(HeroMapHeader *m) {
    hero_map_release_contents(m);
    hero_drop_drain();
}
