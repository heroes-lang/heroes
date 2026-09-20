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
 * true. Every allocation in the runtime is here; nothing else under `runtime/`
 * may call `malloc`, `calloc`, `realloc`, `reallocarray`, `aligned_alloc`,
 * `strdup`, `strndup` or `free`.
 *
 * AND THE GREP THIS PARAGRAPH USED TO PROMISE IS A SUITE, since 2026-09-05:
 * `tests/harness/suite_runtime.hero`, run by `heroes run tests/harness/main.hero
 * -- <compiler> runtime`. The promise was *"checkable with one grep"* and nobody
 * ever wrote the grep, so `parts/array.c` grew a static buffer with `realloc` and
 * the file went on claiming otherwise — which is the same shape as the eleven
 * calls above, one level up. The suite reads the whole allocation family and not
 * `malloc` alone, because the second site said `realloc`, and it reads code
 * rather than text: `parts/run.c` contains the English sentence *"the first
 * check is free (WNOHANG, no sleep at all)"*, which is what a grep would have
 * reported and a reader would have learned to ignore.
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
 * A third shape joined them on 2026-09-05 and it is deliberately not a pair —
 * `hero_grow_kept`, at the bottom, for the one buffer the runtime keeps for the
 * life of the process. Its own comment says why neither counter weighs it.
 *
 * The counter is `static` and stays so: that is what a decoy runtime linked
 * beside this one cannot reach, and it is why `runtime.c` is one translation
 * unit (its own header explains the rest).
 */

/* BOTH COUNTERS ARE ATOMIC AND BOTH STAY PROCESS-WIDE — panel 111 R5, ratified
 * 2026-09-05, and it is the resolution that refused the cheaper one.
 *
 * The proposal that sitting was convened on made them `_Thread_local` and added
 * a per-thread check at thread exit. M-isolated-threads step 2 built it and
 * priced it: the race does go, and the cost is not measurable. Then it ran the
 * program that matters — a worker thread that allocates a `str` and never gives
 * it back. Shared counters say `panic: 1 heap blocks still live at exit`, exit
 * 134. Thread-local counters said nothing at all and exited 0, because
 * `hero_runtime_check_leaks()` runs on the main thread and reads one counter,
 * and made thread-local that counter is the main thread's own balance. ASan has
 * no leak detector on Darwin arm64 (panel 021), so that silence is total.
 *
 * The sitting's own words for it: *race present, instrument silent*. So the
 * counters are made SAFE rather than SPLIT — one balance for the whole process,
 * which is the number the gate is asking about, kept whole by the hardware
 * instead of by luck. Two seats measured the price at 20M pairs and the sign of
 * the difference flipped between runs.
 *
 * RELAXED on the way up and down, because the counter is read once, at exit,
 * after every thread has been joined: the join is what orders it, and a relaxed
 * read-modify-write still cannot lose an increment. The gate below reads the
 * plain field, which on an `_Atomic` is a sequentially consistent load. */

/* live heap-block balance: the leak detector that works on this platform */
static _Atomic int64_t hero_live_blocks = 0;

/* live scratch balance: what a runtime call borrowed and has not given back */
static _Atomic int64_t hero_live_scratch = 0;

/* live HELD balance: buffers the PROGRAM asked for and has not released.
 * A FOURTH SHAPE (a LEASE, `s.lease()`), and it is a pair like the first two — what makes it its own
 * counter is who is at fault when it is not zero. An unbalanced block count is a
 * missing decref, which is the compiler's; an unbalanced scratch count is a
 * runtime call that kept what it borrowed. An unbalanced HELD count is a
 * `end_lease` the PROGRAM did not write, so the gate must say so: the message
 * that blames the compiler for a program's own omission is the one that gets
 * a reader to file a bug against the wrong thing (panel 124 R6). */
static _Atomic int64_t hero_live_held = 0;
/* FOURTH, and it accuses the program for the third one's reason. A C handle is
 * the one resource this runtime never allocated and can never size: no header,
 * no magic, no length — and both ends are the binding author's own words
 * (panel 148 R2).
 *
 * **THIS SAID "so a COUNT is all there can be" UNTIL 2026-09-15, AND THAT
 * SENTENCE WAS THE DEFECT.** It is true that a handle cannot be SIZED. It does
 * not follow that it cannot be IDENTIFIED: the address is the identity, and a
 * set of addresses needs no header, no magic and no length either. Panel 150
 * found four separate failures that are all one fact — the counter held a number
 * where it needed a set — and this file had already named the instrument, three
 * paragraphs down, as *"a different instrument, not a better sentence"*, while
 * pricing it at nothing.
 *
 * What a number cannot tell apart, and a set can:
 *
 *   - a NULL from an address. A producer that fails returns NULL, which is the C
 *     convention for all of them, and the counter counted it — so the correct
 *     failure path aborted (defect 039).
 *   - a null RELEASE from a real one. `slot_close(nullptr)` decremented, so a
 *     leaked handle plus one null release balanced and exited 0 in silence
 *     (defect 040). The escape from 039 and the hole in the leak check were the
 *     same construct.
 *   - a double release from an unmarked producer, which the old message had to
 *     offer as a list of causes and got rewritten three times in two days.
 *   - a composite release from an element-by-element one (defect 038).
 *
 * **It is a KEPT buffer, on `hero_eq_queue`'s model and for its reasons**: it
 * goes through `hero_malloc_raw` so §4.20's single allocation point stays
 * literally true, and it is deliberately outside `hero_live_blocks`, because a
 * buffer that is never freed would report a leak at every exit of every program
 * that binds C. THE DAY THREADS ARRIVE this is the same edit `hero_eq_queue`
 * owes, decided in this file and not at the call site.
 *
 * **The lock is not decoration.** What it replaces was `_Atomic`, so a set
 * without one would remove thread safety in silence, and a regression nobody
 * writes down is the thing this file exists to prevent. */
/* TWO PLATFORM SPELLINGS, on the model this file already uses for the kept
 * buffer's key and for its reason: Windows has no pthreads at all, measured on
 * the box 2026-09-06 and written down below. Both spellings are chosen to be
 * STATICALLY initialisable, so the lock needs no once-flag and no make — which
 * is what keeps this twenty lines rather than the kept buffer's hundred.
 * `SRWLOCK_INIT` and `PTHREAD_MUTEX_INITIALIZER` are the two that are.
 *
 * **The Windows half is UNRUN on this Mac** (`.claude/rules/platforms.md`: a
 * platform fact is run on a platform or it is an inference). CI's Windows leg is
 * the judge, and it is the leg that has caught this class before. */
#if defined(_WIN32)
#include <windows.h>
typedef SRWLOCK hero_handle_mutex;
#define HERO_HANDLE_MUTEX_INIT SRWLOCK_INIT
#define hero_handle_lock_take(m) AcquireSRWLockExclusive(m)
#define hero_handle_lock_drop(m) ReleaseSRWLockExclusive(m)
#else
#include <pthread.h>
typedef pthread_mutex_t hero_handle_mutex;
#define HERO_HANDLE_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define hero_handle_lock_take(m) pthread_mutex_lock(m)
#define hero_handle_lock_drop(m) pthread_mutex_unlock(m)
#endif

#define HERO_HANDLES_MIN 16
static hero_handle_mutex hero_handle_lock = HERO_HANDLE_MUTEX_INIT;
static const void **hero_handle_set = NULL;
static size_t hero_handle_cap = 0;
static size_t hero_handle_live = 0;
/* Released while the set did not hold it: a double release, or a part of
 * something acquired whole. Counted rather than stored, because the message
 * names the first one and the reader needs a number for the rest. */
static size_t hero_handle_strays = 0;
static const void *hero_handle_first_stray = NULL;

int64_t hero_runtime_live(void) { return hero_live_blocks; }

/* THE STRAY MESSAGE, WRITTEN ONCE AND CALLED FROM TWO PLACES — defect 071,
 * 2026-09-20. It is raised at the moment of detection, inside
 * `hero_handle_consumed`, and the exit gate keeps its own call as defence in
 * depth: an emitter that failed to tell the set before the call would leave a
 * stray standing, and that must not become silence.
 *
 * THE ADDRESS GOES LAST, and that is not a style choice. A golden asserts a
 * SUBSTRING of this message, so an address in the middle splits the one stable
 * sentence in two and no case can assert it — measured 2026-09-15, when three
 * shipped goldens went red on a message that was right. Last, the sentence a
 * case asserts is contiguous and the address a reader needs is still here. */
static void hero_handle_report_stray(size_t strays, const void *first_stray) {
    fflush(stdout);
    fprintf(stderr, "panic: %llu C handle(s) given back that were never taken — "
                    "the set of live handles did not hold that address when a "
                    "call marked `consumes` ran. Two things do this: the same "
                    "handle given back TWICE, which is a double release and "
                    "may already have corrupted memory; or a value acquired "
                    "WHOLE and released part by part, since one mark is one "
                    "obligation on the whole value. The first is at %p\n",
            (unsigned long long)strays, first_stray);
    abort();
}

/* EACH COUNTER IS READ ONCE, into a local, and the message prints that local.
 * The plain `hero_live_blocks != 0` followed by a second read inside the
 * `fprintf` was two loads of one atomic object: correct while nothing else could
 * be running, and a message that names a number the test never saw the moment
 * something can. The gate is the last thing a program does, so this costs one
 * load and removes a whole class of confusing report. */
void hero_runtime_check_leaks(void) {
    int64_t blocks = hero_live_blocks;
    if (blocks != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld heap blocks still live at exit "
                        "(a missing decref) — this is a compiler bug\n",
                (long long)blocks);
        abort();
    }
    int64_t scratch = hero_live_scratch;
    if (scratch != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld scratch buffers still live at exit "
                        "(a runtime call kept what it borrowed) — this is a "
                        "runtime bug\n",
                (long long)scratch);
        abort();
    }
    /* THIRD, AND IT ACCUSES THE PROGRAM RATHER THAN THIS COMPILER. Held bytes
     * are the one allocation a Heroes program asks for by name, so a leak of
     * them is the one leak that is not a bug in the language. */
    int64_t held = hero_live_held;
    if (held != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld lease(s) never ended — every `.lease()` owes "
                        "one `end_lease`, and this program is missing that "
                        "many\n",
                (long long)held);
        abort();
    }
    /* TWO DIRECTIONS, AND THEY ARE TWO DIFFERENT FAULTS. Positive is the
     * PROGRAM's: a handle it was given and never gave back. NEGATIVE is the
     * BINDING's: a `consumes` with no `acquires` to pair it, so a correct
     * program hands back what the count never saw arrive. The second was found
     * by running `examples/curl/main.hero`, which has carried `consumes` since
     * panel 145 and was killed at exit 134 by a message saying the opposite of
     * what had happened. One counter, two sentences.
     *
     * **AND THE NEGATIVE SENTENCE WAS REWRITTEN ON 2026-09-14, because the
     * world moved under it one step after it was written.** It named exactly
     * one cause, *a `consumes` whose producer carries no mark*, which was the
     * only one there was when `hero_live_handles` landed. `check/acquiring.hero`
     * then made the direct form of that a COMPILE error, and the sentence went
     * on naming it. Three programs were run here, all three arriving with the
     * old wording and only one of them matching it:
     *
     *   - the same handle given back TWICE, a double release, reported as a
     *     missing annotation, which sends its reader to edit a declaration
     *     while the program is corrupting memory;
     *   - an `extern` marked `borrows` whose C function in fact hands
     *     ownership over, reached directly and through an ordinary Heroes
     *     function, where the mark is a lie no rule can catch;
     *   - a handle arriving inside a record FIELD, `pair_make() -> Pair`,
     *     which `unmarked_handle_producer` does not look into, so its producer
     *     is genuinely unmarked and the compiler said nothing. That gap is
     *     filed as defect 033 rather than widened here, because widening what a
     *     diagnostic refuses is a diagnostic CLASS and goes to the panel.
     *
     * The count cannot tell the three apart, so the message states what it
     * measured and names all three rather than asserting one. Worst first.
     *
     * **And WHY it cannot is worth saying, because it names the price of the
     * alternative**: this is a counter and not a set. Telling a double release
     * from an unmarked producer needs the IDENTITY of each handle, so the
     * runtime would hold every live pointer and every call site would pay a
     * lookup — a different instrument, not a better sentence. Three cases in
     * `tests/golden/run/` keep the three-cause claim honest, one per cause, and
     * they are `abort-handle-given-back-twice`,
     * `abort-handle-borrows-that-gives-away` and
     * `abort-handle-given-back-unmarked`. If a later rule catches one of them
     * at compile time, its case goes red and this sentence is what must
     * change — which is exactly how the old one was caught.
     *
     * **AND IT HAPPENED AGAIN THE NEXT DAY, WHICH IS WHY THE MESSAGE NOW NAMES
     * TWO.** Panel 149 widened `unmarked_handle_producer` to ask what a type
     * REACHES rather than what it IS, so the third cause above — a handle
     * arriving inside a record field — became a compile error, and its case
     * moved to `tests/golden/check/fixedbugs-a-handle-reached-through-a-field`.
     * The clause naming it is struck below rather than left standing, because a
     * message that offers a cause the compiler has already closed sends its
     * reader to look for something that cannot be there.
     *
     * **Only the clause, and not the sentence.** The two remaining causes still
     * ship and still have their cases: the count cannot tell a double release
     * from a lying `borrows`, and no rule catches either, because the first
     * needs handle IDENTITY and the second is a claim about C that no Heroes
     * declaration can refute. The prediction registered with this paragraph is
     * that the next rule to close one of those two will come for `borrows`, and
     * the case that goes red will be
     * `abort-handle-borrows-that-gives-away`. */
    /* THE STRAY IS REPORTED FIRST, because it is the one that may already have
     * corrupted memory: a handle given back that the set did not hold is either
     * a second release of something already gone, or a part of something
     * acquired whole. A leak has not damaged anything yet. */
    hero_handle_lock_take(&hero_handle_lock);
    size_t strays = hero_handle_strays;
    const void *first_stray = hero_handle_first_stray;
    size_t live = hero_handle_live;
    const void *first_live = NULL;

    for (size_t i = 0; i < hero_handle_cap && first_live == NULL; i++)
        if (hero_handle_set[i] != NULL) first_live = hero_handle_set[i];
    hero_handle_lock_drop(&hero_handle_lock);

    /* DEFENCE IN DEPTH SINCE defect 071: `hero_handle_consumed` raises this at
     * the moment of detection, so in an emitted program this branch is now
     * unreachable. It stays because an emitter that stopped telling the set
     * before the call would otherwise turn a corruption into silence, and the
     * message is the same one, written once above. */
    if (strays > 0) hero_handle_report_stray(strays, first_stray);
    if (live > 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %llu C handle(s) never given back — every call "
                        "marked `acquires` owes one marked `consumes`, and this "
                        "program is missing that many. The first is at %p\n",
                (unsigned long long)live, first_live);
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
    atomic_fetch_add_explicit(&hero_live_scratch, 1, memory_order_relaxed);
    return p;
}

static void hero_release(void *p) {
    atomic_fetch_sub_explicit(&hero_live_scratch, 1, memory_order_relaxed);
    free(p);
}

/* A block the language owns: a `str`'s bytes, an array's elements, a map's
 * table. Counted, and the count is the leak gate. */
static void *hero_alloc_block(size_t size) {
    void *p = hero_malloc_raw(size);
    atomic_fetch_add_explicit(&hero_live_blocks, 1, memory_order_relaxed);
    return p;
}

static void hero_release_block(void *p) {
    atomic_fetch_sub_explicit(&hero_live_blocks, 1, memory_order_relaxed);
    free(p);
}

/* A block the PROGRAM owns: bytes it held for C and will release itself. Its own
 * pair, for the reason the counter's comment gives — who is at fault at exit. */
static void *hero_alloc_held(size_t size) {
    void *p = hero_malloc_raw(size);
    atomic_fetch_add_explicit(&hero_live_held, 1, memory_order_relaxed);
    return p;
}

static void hero_release_held(void *p) {
    atomic_fetch_sub_explicit(&hero_live_held, 1, memory_order_relaxed);
    free(p);
}

/* The handle set. Open addressing, linear probing, power-of-two capacity, so a
 * slot is found with a mask and never a division. NULL is the empty slot, which
 * is free: a NULL handle is never stored, because a producer that returned NULL
 * acquired nothing (defect 039).
 *
 * The mix is Fibonacci hashing on the pointer's own bits. An address is already
 * well distributed in its middle bits and poorly in its low ones, which are
 * alignment, so the shift is what a naive mask would throw away. */
static size_t hero_handle_slot(const void **table, size_t cap, const void *h) {
    size_t i = (size_t)(((uintptr_t)h * (uintptr_t)0x9E3779B97F4A7C15u) >> 32) & (cap - 1);
    while (table[i] != NULL && table[i] != h) i = (i + 1) & (cap - 1);
    return i;
}

/* Grown in place, never shrunk, and the old table is freed: that is what makes
 * it ONE live allocation rather than a leak per growth. Called with the lock
 * held. */
static void hero_handle_grow(void) {
    size_t cap = hero_handle_cap == 0 ? (size_t)HERO_HANDLES_MIN : hero_handle_cap * 2;
    const void **fresh = (const void **)hero_malloc_raw(cap * sizeof(const void *));
    for (size_t i = 0; i < cap; i++) fresh[i] = NULL;
    for (size_t i = 0; i < hero_handle_cap; i++) {
        if (hero_handle_set[i] != NULL)
            fresh[hero_handle_slot(fresh, cap, hero_handle_set[i])] = hero_handle_set[i];
    }
    free(hero_handle_set);
    hero_handle_set = fresh;
    hero_handle_cap = cap;
}

void hero_handle_acquired(const void *h) {
    /* A producer that failed handed back nothing. Counting it is what killed the
     * correct failure path, and the failure path is the one C programs take. */
    if (h == NULL) return;
    hero_handle_lock_take(&hero_handle_lock);
    /* Grow at half full. Linear probing degrades sharply past that, and this
     * table is read on every FFI handle call. */
    if (hero_handle_cap == 0 || (hero_handle_live + 1) * 2 > hero_handle_cap) hero_handle_grow();
    size_t i = hero_handle_slot(hero_handle_set, hero_handle_cap, h);
    /* Already live means the previous one was never given back and C has handed
     * the address out again. The set holds one entry per ADDRESS, so the earlier
     * life is lost here rather than double-counted — and it is lost either way,
     * since nothing can now tell the two apart. */
    if (hero_handle_set[i] == NULL) {
        hero_handle_set[i] = h;
        hero_handle_live++;
    }
    hero_handle_lock_drop(&hero_handle_lock);
}

void hero_handle_consumed(const void *h) {
    /* Releasing a null discharges nothing. C lets a program free NULL and this
     * lets it too; what it must not do is let that pay off a real obligation. */
    if (h == NULL) return;
    hero_handle_lock_take(&hero_handle_lock);

    if (hero_handle_cap == 0) {
        hero_handle_strays++;
        if (hero_handle_first_stray == NULL) hero_handle_first_stray = h;
        hero_handle_lock_drop(&hero_handle_lock);
        return;
    }
    size_t i = hero_handle_slot(hero_handle_set, hero_handle_cap, h);

    if (hero_handle_set[i] != h) {
        /* Given back while the set did not hold it: the second of a double
         * release, or a part of something acquired whole. The old counter could
         * only go negative and offer a list of causes.
         *
         * IT ABORTS HERE RATHER THAN AT EXIT — defect 071, 2026-09-20. The
         * report below used to wait for `hero_runtime_check_handles`, and the
         * comment beside it already gave the reason that makes waiting wrong:
         * "the stray is reported first, because it is the one that may already
         * have corrupted memory". Against a deallocator that actually frees,
         * the program never reaches exit — measured, exit 133 inside the real
         * `free` with nothing on stderr, while this message sat waiting. The
         * emitter now tells the set BEFORE the call (`emit/handle_traffic.hero`),
         * so this is the last moment at which the double release has not
         * happened yet. */
        hero_handle_strays++;
        if (hero_handle_first_stray == NULL) hero_handle_first_stray = h;
        hero_handle_lock_drop(&hero_handle_lock);
        hero_handle_report_stray(1, h);
    }
    /* Backward-shift deletion, because linear probing cannot tombstone without
     * the table filling with tombstones on a long-running program. */
    size_t hole = i;
    size_t scan = (i + 1) & (hero_handle_cap - 1);
    hero_handle_set[hole] = NULL;

    while (hero_handle_set[scan] != NULL) {
        size_t home = (size_t)(((uintptr_t)hero_handle_set[scan] * (uintptr_t)0x9E3779B97F4A7C15u) >> 32) & (hero_handle_cap - 1);
        size_t from_hole = (scan - hole) & (hero_handle_cap - 1);
        size_t from_home = (scan - home) & (hero_handle_cap - 1);

        if (from_home >= from_hole) {
            hero_handle_set[hole] = hero_handle_set[scan];
            hero_handle_set[scan] = NULL;
            hole = scan;
        }
        scan = (scan + 1) & (hero_handle_cap - 1);
    }
    hero_handle_live--;
    hero_handle_lock_drop(&hero_handle_lock);
}

/* A THIRD SHAPE, AND IT IS NOT A PAIR: memory the RUNTIME keeps for the life of
 * the process, grown in place and never given back.
 *
 * There is exactly one of these — `hero_eq_queue` in `parts/array.c`, the work
 * list that lets a deep `==` run without recursing — and it is here because it
 * grew itself with `realloc` until 2026-09-05, which made it a SECOND allocation
 * site: the one thing design.md Part 7.13 says the runtime must not have, since
 * a per-thread heap needs one place to change. The promised grep would not have
 * found it either, because the promise named `malloc` and the call said
 * `realloc`; `tests/harness/suite_runtime.hero` reads the whole family now.
 *
 * WHY NEITHER COUNTER WEIGHS IT. `hero_live_blocks` is the leak gate, and a
 * buffer that is deliberately never freed would report a leak at every exit of
 * every program that compares two deep arrays — a false alarm in the one
 * instrument this platform has (panel 021: ASan has no leak detector on Darwin
 * arm64). `hero_live_scratch` says what a runtime call borrowed and has not
 * given back, and this is not borrowed: keeping it IS the point, because the
 * alternative is a malloc and a free on every top-level comparison. So it goes
 * through `hero_malloc_raw` like both pairs above, and §4.20's single point
 * stays literally true.
 *
 * THE DAY THREADS ARRIVE this is the edit design.md promised would be one edit.
 * `array.c` says the queue must become `_Thread_local`, and *"the buffer then
 * leaks one allocation per thread"*: whether that is paid, pooled or freed at
 * thread exit is decided here, in the allocator, and not in the array.
 *
 * **THAT DAY IS M-isolated-threads STEP 4, AND THE ANSWER IS FREED AT THREAD
 * EXIT.** The three the sentence offered are not equal. *Paid* means a thread
 * that compares one deep value leaves a block behind for the life of the
 * process, and a program with one thread per connection pays it per connection.
 * *Pooled* is a shared free list, which is a second allocation site wearing a
 * different hat and the one thing this file exists to prevent. *Freed at thread
 * exit* is the model's own answer: under isolation the buffer is made and used
 * on one thread, so the thread that made it is exactly who can give it back.
 *
 * WHY THE POINTER LIVES IN THE KEY AND NOT IN A `_Thread_local`, which is the
 * whole reason this is 20 lines rather than 2. Panel 111 measured both seats
 * finding the same trap independently: on Darwin arm64 a `_Thread_local` read
 * from INSIDE a `pthread_key_t` destructor reads **0**, because thread-local
 * storage is already torn down when the destructor runs — `worker: live=3` and
 * `destructor: live=0` at the same address, in the sitting's own transcript. So
 * a destructor cannot be handed the buffer by the thread-local that names it.
 * It has to be handed the buffer by the key itself, which is the one thing still
 * alive at that point, and that is what `hero_kept_note` stores.
 *
 * **AND THE ONE SLOT IS A CLAIM, NOT AN ASSUMPTION** (CLAUDE.md §11). The key
 * holds ONE pointer because there is exactly one kept buffer in this runtime,
 * and `hero_grow_kept` has exactly one caller. That is a fact about today which
 * a second caller would quietly break — a thread would free one buffer and
 * abandon the other. So it is asserted rather than trusted: the `old` a caller
 * hands in must be what this thread last noted, and a second kept buffer trips
 * that panic on its first growth instead of leaking in silence.
 *
 * THE TWO PLATFORM SPELLINGS, and neither is a translation of the other.
 * POSIX has `pthread_key_create` with a destructor per key; Windows has no
 * pthreads at all — measured on the box 2026-09-06, `#include <pthread.h>` is
 * `fatal error: 'pthread.h' file not found` under clang targeting MSVC — and
 * answers with fibre-local storage, `FlsAlloc`, whose callback runs on thread
 * exit exactly as the destructor does. Both were run before either was written
 * in. C11's `<threads.h>` would have been one spelling for both and is not
 * available: it is present on Windows and on glibc and **absent from the macOS
 * SDK**, measured the same day, so the portable-looking answer is the one that
 * does not compile here. */
#if defined(_WIN32)
#include <windows.h>
static DWORD hero_kept_slot = FLS_OUT_OF_INDEXES;
static void WINAPI hero_kept_release(PVOID buffer) {
    free(buffer);
}
static BOOL CALLBACK hero_kept_make(PINIT_ONCE once, PVOID param, PVOID *context) {
    (void)once;
    (void)param;
    (void)context;
    hero_kept_slot = FlsAlloc(hero_kept_release);
    if (hero_kept_slot == FLS_OUT_OF_INDEXES) hero_panic("cannot make the thread's kept-buffer slot");
    return TRUE;
}
#else
#include <pthread.h>
static pthread_key_t hero_kept_key;
static void hero_kept_release(void *buffer) {
    free(buffer);
}
static void hero_kept_make(void) {
    if (pthread_key_create(&hero_kept_key, hero_kept_release) != 0) {
        hero_panic("cannot make the thread's kept-buffer key");
    }
}
#endif

/* Made once for the whole process, on whichever thread grows a kept buffer
 * first. `pthread_once` and `InitOnceExecuteOnce` are the platform spellings and
 * both are correct; this is the one place a plain `_Atomic` flag would not be,
 * because two threads racing here would make two keys and the loser's destructor
 * would never run. */
static void hero_kept_ready(void) {
#if defined(_WIN32)
    static INIT_ONCE once = INIT_ONCE_STATIC_INIT;
    InitOnceExecuteOnce(&once, hero_kept_make, NULL, NULL);
#else
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, hero_kept_make);
#endif
}

/* This thread's kept buffer, as the OS will see it at thread exit. */
static void hero_kept_note(void *old, void *fresh) {
    hero_kept_ready();
#if defined(_WIN32)
    void *held = FlsGetValue(hero_kept_slot);
#else
    void *held = pthread_getspecific(hero_kept_key);
#endif
    if (held != old) {
        hero_panic("a second buffer kept for the life of a thread — parts/alloc.c "
                   "holds one slot per thread, and it is now wrong");
    }
#if defined(_WIN32)
    if (!FlsSetValue(hero_kept_slot, fresh)) hero_panic("cannot record a kept buffer");
#else
    if (pthread_setspecific(hero_kept_key, fresh) != 0) hero_panic("cannot record a kept buffer");
#endif
}

static void *hero_grow_kept(void *old, size_t old_bytes, size_t new_bytes) {
    void *fresh = hero_malloc_raw(new_bytes);
    if (old != NULL) {
        memcpy(fresh, old, old_bytes);
        free(old);
    }
    hero_kept_note(old, fresh);
    return fresh;
}
