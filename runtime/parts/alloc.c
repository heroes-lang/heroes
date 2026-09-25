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

/* One entry per live address, and beside it the RELEASERS its acquiring mark
 * named, `a|b` (panel 175's route A, landed at panel 176's item 1; defect 075).
 * Compared by CONTENT, token by token: each module is its own translation
 * unit, so neither a literal's address nor a header's static function is one
 * identity across them. A NULL `by` admits any consumer, which is what a
 * handle acquired before this ABI would have carried. */
typedef struct {
    const void *h;
    const char *by;
    /* References the program holds on this address (panel 176's item 4): one
     * per acquisition, one more per `retains`, one fewer per end, and the
     * entry leaves the set at zero. */
    size_t n;
    /* Ends a call has announced before running and not yet made (defect 084):
     * never above `n`, or the call would release inside C what it holds once. */
    size_t pending;
} hero_handle_entry;
static hero_handle_entry *hero_handle_set = NULL;
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
 * `hero_handle_held`, and the exit gate keeps its own call as defence in
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
                    "call that ends or transfers it ran. Three things do this: "
                    "the same handle given back TWICE, which is a double release "
                    "and may already have corrupted memory; a value acquired "
                    "WHOLE and released part by part, since one mark is one "
                    "obligation on the whole value; or a handle given back after "
                    "a call marked `transfers` handed it into another value, "
                    "which ends it from now on. The first is at %p\n",
            (unsigned long long)strays, first_stray);
    hero_abort();
}

/* ONE CALL, TWO CONSUMING POSITIONS, ONE HANDLE (defect 084): `SSL_set_bio(s,
 * b, b)`, where OpenSSL takes ONE reference. Raised before C runs, because the
 * second position would be a double release inside the library: each end the
 * call announces is PENDING on the entry until the call returns, and a call
 * that announces more ends than the address holds references is stopped here
 * rather than at the second `hero_handle_ended`, which under the first shape
 * of ABI 24 came after C had already freed twice (the landing review,
 * 2026-09-24). Panel 176 ruled it a message and a shim, not a word. */
static void hero_handle_report_twice_in_one_call(const void *h, size_t held) {
    fflush(stdout);
    fprintf(stderr, "panic: one call takes the same C handle at two consuming positions, "
                    "and the handle holds %llu reference(s) — the library takes one "
                    "reference where the binding says it takes two, as OpenSSL's "
                    "`SSL_set_bio(s, b, b)` does, so the binding needs a shim that "
                    "consumes it once; or, for a reference-counted object, one more "
                    "`retains` before the call. The handle is at %p\n",
            (unsigned long long)held, h);
    hero_abort();
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
        hero_abort();
    }
    int64_t scratch = hero_live_scratch;
    if (scratch != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld scratch buffers still live at exit "
                        "(a runtime call kept what it borrowed) — this is a "
                        "runtime bug\n",
                (long long)scratch);
        hero_abort();
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
        hero_abort();
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
        if (hero_handle_set[i].h != NULL) first_live = hero_handle_set[i].h;
    hero_handle_lock_drop(&hero_handle_lock);

    /* DEFENCE IN DEPTH SINCE defect 071: `hero_handle_held` raises this at
     * the moment of detection, so in an emitted program this branch is now
     * unreachable. It stays because an emitter that stopped telling the set
     * before the call would otherwise turn a corruption into silence, and the
     * message is the same one, written once above. */
    if (strays > 0) hero_handle_report_stray(strays, first_stray);
    if (live > 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %llu C handle(s) never given back — every life a "
                        "mark began, `acquires` or `retains`, owes one release, "
                        "and this program is missing that many. The first is at %p\n",
                (unsigned long long)live, first_live);
        hero_abort();
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
static size_t hero_handle_slot(const hero_handle_entry *table, size_t cap, const void *h) {
    size_t i = (size_t)(((uintptr_t)h * (uintptr_t)0x9E3779B97F4A7C15u) >> 32) & (cap - 1);
    while (table[i].h != NULL && table[i].h != h) i = (i + 1) & (cap - 1);
    return i;
}

/* Is the token `name[0..n)` one of the `|`-separated tokens of `set`? Whole
 * tokens only, so `close` never passes for `close_v2`. Compared in place, with
 * no copy and no buffer: a name longer than a buffer would otherwise never
 * share, and every release of it would be a false crossing (the landing
 * review, 2026-09-24, on the 128-byte copy this used to make). */
static int hero_handle_named_n(const char *set, const char *name, size_t n) {
    if (set == NULL || name == NULL) return 0;
    const char *at = set;
    for (;;) {
        const char *bar = strchr(at, '|');
        size_t len = bar == NULL ? strlen(at) : (size_t)(bar - at);
        if (len == n && memcmp(at, name, n) == 0) return 1;
        if (bar == NULL) return 0;
        at = bar + 1;
    }
}

static int hero_handle_named(const char *set, const char *name) {
    if (name == NULL) return 0;
    return hero_handle_named_n(set, name, strlen(name));
}

/* Do two sets share a token? A consumer that also acquires may pay what its
 * own releasers are owed: `freopen` takes back an `fopen` stream. */
static int hero_handle_sets_share(const char *set, const char *other) {
    if (set == NULL || other == NULL) return 0;
    const char *at = other;
    for (;;) {
        const char *bar = strchr(at, '|');
        size_t len = bar == NULL ? strlen(at) : (size_t)(bar - at);
        if (hero_handle_named_n(set, at, len)) return 1;
        if (bar == NULL) return 0;
        at = bar + 1;
    }
}

/* Grown in place, never shrunk, and the old table is freed: that is what makes
 * it ONE live allocation rather than a leak per growth. Called with the lock
 * held. */
static void hero_handle_grow(void) {
    size_t cap = hero_handle_cap == 0 ? (size_t)HERO_HANDLES_MIN : hero_handle_cap * 2;
    hero_handle_entry *fresh = (hero_handle_entry *)hero_malloc_raw(cap * sizeof(hero_handle_entry));
    for (size_t i = 0; i < cap; i++) {
        fresh[i].h = NULL;
        fresh[i].by = NULL;
    }
    for (size_t i = 0; i < hero_handle_cap; i++) {
        if (hero_handle_set[i].h != NULL)
            fresh[hero_handle_slot(fresh, cap, hero_handle_set[i].h)] = hero_handle_set[i];
    }
    free(hero_handle_set);
    hero_handle_set = fresh;
    hero_handle_cap = cap;
}

/* THE DEAD REGION (panel 177's item 1, route P; defects 077 and 088). What
 * the emitted C writes into the binding, field or element a consuming call
 * read its argument from, once `hero_handle_ended` says the life ended: an
 * address at the start of HERO_DEAD_SPAN bytes (64 KiB, `parts/stack.c`) this
 * runtime maps with NO ACCESS, so a C read or write through a dead value that
 * slipped past every check faults on the spot rather than landing in memory
 * the allocator has handed to somebody else, and the fault handlers name it
 * (`hero_handle_dead_at`). Panel 177's compiler-engineer prototyped one
 * `max_align_t` cell and its critic measured the hole (`cb_return_poison64`):
 * C wrote 64 bytes into a 16-byte static and the runtime's neighbours took the
 * rest, in silence, under `--sanitize` too. One page was the first landing's
 * size, and a field past it would have been read from whatever the kernel
 * mapped next; 64 KiB is the null window's, so both kinds of bad handle fault
 * at the same offsets.
 *
 * Mapped on the first life that ends and never unmapped. The pointer is
 * atomic so a check on another thread, or a fault handler, reads the region
 * or NULL and never a torn value; NULL means no life has ended yet, so no
 * dead value exists and `alive` cannot be handed one. Two platform
 * spellings, on this file's model: `mmap` with `PROT_NONE`, and
 * `VirtualAlloc` with `PAGE_NOACCESS`, whose 64 KiB allocation granularity
 * the span matches. **The Windows half is unrun on this Mac**
 * (`.claude/rules/platforms.md`); CI's Windows leg is the judge. The pointer
 * itself is declared in `parts/stack.c`, which the handlers that read it come
 * before. */
#if !defined(_WIN32)
#include <sys/mman.h>
#endif
static _Atomic(void *) hero_handle_dead_page = NULL;

static void *hero_handle_dead_map(void) {
#if defined(_WIN32)
    void *page = VirtualAlloc(NULL, (SIZE_T)HERO_DEAD_SPAN, MEM_RESERVE | MEM_COMMIT, PAGE_NOACCESS);
    if (page == NULL) hero_panic("cannot map the region a dead C handle points at");
    return page;
#else
    void *page = mmap(NULL, (size_t)HERO_DEAD_SPAN, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (page == MAP_FAILED) hero_panic("cannot map the region a dead C handle points at");
    return page;
#endif
}

void *hero_handle_dead(void) {
    void *page = atomic_load_explicit(&hero_handle_dead_page, memory_order_acquire);
    if (page != NULL) return page;
    hero_handle_lock_take(&hero_handle_lock);
    page = atomic_load_explicit(&hero_handle_dead_page, memory_order_relaxed);
    if (page == NULL) {
        page = hero_handle_dead_map();
        atomic_store_explicit(&hero_handle_dead_page, page, memory_order_release);
    }
    hero_handle_lock_drop(&hero_handle_lock);
    return page;
}

/* THE DEAD SET (panel 177's item 2, route T). The addresses whose life a call
 * marked `consumes` or `transfers` ended, kept until C hands the address out
 * again: an acquisition, a reference that begins a life, a `borrows`
 * hand-back, a handle C passes to a callback. What it catches is the copy
 * made BEFORE the ending call, which the poison above cannot see: the copy
 * still holds the old address, and that address is here. What it cannot
 * catch is the same copy read after C has reused the address, because the
 * reuse cleared the mark, which is the limit design.md Part 8 wart 20 states,
 * and under `--sanitize` ASan's quarantine keeps the address from coming back,
 * so the copy is caught there too (the critic's `sqlite_copy_reuse`, run).
 *
 * A WINDOW OF THE LAST 2^20 ENDS, and the bound is exact, which the shape it
 * replaced was not. An address is remembered from the end that marked it
 * until C hands it out again or 2^20 = 1,048,576 more lives have ended,
 * whichever comes first: a copy is refused through 1,048,575 later ends and
 * not at the 1,048,576-th, whatever else the program does. The memory is at
 * most 16 MiB held (the ring, 2^20 addresses of 8 bytes, and the index,
 * 2^21 slots of 4) and 20 MiB for the one instant the index doubles to its
 * last size beside its old copy. Both grow by doubling from 16 entries, so a
 * program that gives back a hundred handles pays for a hundred.
 *
 * WHAT IT REPLACED, measured by the landing's skeptic and then here
 * (2026-09-25): two generations of 2^20 addresses, dropped whole in turn.
 * Stated as *at most 2 x 16 MiB*, it allocated 40 MiB at the second
 * generation's last doubling, 16 held beside 8 and 16 in flight, and the
 * process peaked at 49.47 MiB resident with the allocator keeping the
 * smaller tables it had freed; stated as *remembered for at least 2^20 later
 * ends*, an address marked last in its generation was forgotten at the
 * 1,048,576-th. A generation's window depends on where in it the address
 * fell; a ring's does not.
 *
 * THE SHAPE. `hero_dead_ring[k & (W - 1)]` is the address the k-th mark
 * ended, so the ring's slot for a mark is its sequence number modulo the
 * window, and the mark W ends older than the newest is the one the next
 * mark overwrites. `hero_dead_index` is open addressing over the ring, one
 * slot per dead address holding the ring slot of its LATEST mark plus one
 * (0 is empty), compared through the ring, so four bytes a slot rather than
 * sixteen. A mark that leaves the window takes its index entry with it only
 * if the entry still points at it: the address may have been handed out
 * again since (the entry is gone) or given back again (the entry points at
 * the newer mark, which stays). All of it is under the live set's lock. */
#define HERO_DEAD_WINDOW ((size_t)1 << 20)
static const void **hero_dead_ring = NULL;
static size_t hero_dead_ring_cap = 0;
static uint64_t hero_dead_marks = 0;
static uint32_t *hero_dead_index = NULL;
static size_t hero_dead_index_cap = 0;
static size_t hero_dead_count = 0;

static size_t hero_dead_home(size_t cap, const void *h) {
    return (size_t)(((uintptr_t)h * (uintptr_t)0x9E3779B97F4A7C15u) >> 32) & (cap - 1);
}

/* The index slot of `h`: the one holding its entry, or the empty one where
 * it would go. The load stays at half or less, so the walk ends. */
static size_t hero_dead_find(const void *h) {
    size_t i = hero_dead_home(hero_dead_index_cap, h);
    while (hero_dead_index[i] != 0 && hero_dead_ring[hero_dead_index[i] - 1] != h)
        i = (i + 1) & (hero_dead_index_cap - 1);
    return i;
}

static void hero_dead_grow_index(void) {
    size_t cap = hero_dead_index_cap == 0 ? (size_t)HERO_HANDLES_MIN : hero_dead_index_cap * 2;
    uint32_t *fresh = (uint32_t *)hero_malloc_raw(cap * sizeof(uint32_t));
    for (size_t i = 0; i < cap; i++) fresh[i] = 0;
    for (size_t i = 0; i < hero_dead_index_cap; i++) {
        uint32_t entry = hero_dead_index[i];
        if (entry == 0) continue;
        size_t j = hero_dead_home(cap, hero_dead_ring[entry - 1]);
        while (fresh[j] != 0) j = (j + 1) & (cap - 1);
        fresh[j] = entry;
    }
    free(hero_dead_index);
    hero_dead_index = fresh;
    hero_dead_index_cap = cap;
}

/* Until the window is full the ring only fills, from slot 0, so a doubling
 * copies it as it stands. */
static void hero_dead_grow_ring(void) {
    size_t cap = hero_dead_ring_cap == 0 ? (size_t)HERO_HANDLES_MIN : hero_dead_ring_cap * 2;
    const void **fresh = (const void **)hero_malloc_raw(cap * sizeof(const void *));
    for (size_t i = 0; i < hero_dead_ring_cap; i++) fresh[i] = hero_dead_ring[i];
    free(hero_dead_ring);
    hero_dead_ring = fresh;
    hero_dead_ring_cap = cap;
}

/* Backward-shift deletion, for the live set's reason: tombstones would fill
 * a long-running program's table. */
static void hero_dead_remove_at(size_t hole) {
    size_t scan = (hole + 1) & (hero_dead_index_cap - 1);
    hero_dead_index[hole] = 0;
    while (hero_dead_index[scan] != 0) {
        size_t home = hero_dead_home(hero_dead_index_cap, hero_dead_ring[hero_dead_index[scan] - 1]);
        size_t from_hole = (scan - hole) & (hero_dead_index_cap - 1);
        size_t from_home = (scan - home) & (hero_dead_index_cap - 1);
        if (from_home >= from_hole) {
            hero_dead_index[hole] = hero_dead_index[scan];
            hero_dead_index[scan] = 0;
            hole = scan;
        }
        scan = (scan + 1) & (hero_dead_index_cap - 1);
    }
    hero_dead_count--;
}

/* Called with the lock held, at every end that closes a life. */
static void hero_dead_mark_locked(const void *h) {
    uint64_t seq = hero_dead_marks++;
    size_t slot = (size_t)(seq & (uint64_t)(HERO_DEAD_WINDOW - 1));
    if (seq >= (uint64_t)HERO_DEAD_WINDOW) {
        /* The mark 2^20 ends older leaves the window, and its address with it
         * unless the address was handed out or given back again since. */
        size_t i = hero_dead_find(hero_dead_ring[slot]);
        if (hero_dead_index[i] == (uint32_t)slot + 1) hero_dead_remove_at(i);
    } else if (slot == hero_dead_ring_cap) {
        hero_dead_grow_ring();
    }
    hero_dead_ring[slot] = h;
    if (hero_dead_index_cap == 0 || (hero_dead_count + 1) * 2 > hero_dead_index_cap) hero_dead_grow_index();
    size_t i = hero_dead_find(h);
    if (hero_dead_index[i] == 0) hero_dead_count++;
    hero_dead_index[i] = (uint32_t)slot + 1;
}

static void hero_dead_clear_locked(const void *h) {
    if (hero_dead_index_cap == 0) return;
    size_t i = hero_dead_find(h);
    if (hero_dead_index[i] != 0) hero_dead_remove_at(i);
}

static int hero_dead_holds_locked(const void *h) {
    if (hero_dead_index_cap == 0) return 0;
    return hero_dead_index[hero_dead_find(h)] != 0;
}

/* THE TWO REPORTS, and the shape of the crossing goes in the sentence:
 * `where` is what the emitter knows and the reader needs, the argument and
 * the call, the cell, or the callback whose result it was, because the line
 * a runtime abort can name is the C's, not the author's. The address goes
 * last, for the stray report's reason. */
static void hero_handle_report_poisoned(const char *where) {
    fflush(stdout);
    fprintf(stderr, "panic: a dead C handle reached %s: a call marked `consumes` or `transfers` "
                    "ended this handle's life and emptied the binding, field or element it "
                    "was read from, so the program is using a handle after giving it back. "
                    "Give a handle back once, after its last use; a handle that must outlive "
                    "one release holds a reference, which `retains` says\n",
            where);
    hero_abort();
}

static void hero_handle_report_remembered(const char *where, const void *h) {
    fflush(stdout);
    fprintf(stderr, "panic: a C handle reached %s at an address a call marked `consumes` or "
                    "`transfers` ended, and nothing has handed that address out since: a "
                    "copy made before the ending call, which C would read or free after "
                    "the program gave it back. The handle is at %p\n",
            where, h);
    hero_abort();
}

/* THE CHECK AT EVERY CROSSING INTO C: the poison first, without the lock,
 * because the page's address is the one fact a dead value carries; then the
 * remembered addresses, under the lock. A NULL is C's own failure value and
 * crosses freely, as it does everywhere else in this file. */
void hero_handle_alive(const void *h, const char *where) {
    if (h == NULL) return;
    void *dead = atomic_load_explicit(&hero_handle_dead_page, memory_order_acquire);
    if (dead != NULL && h == dead) hero_handle_report_poisoned(where);
    hero_handle_lock_take(&hero_handle_lock);
    int remembered = hero_dead_holds_locked(h);
    hero_handle_lock_drop(&hero_handle_lock);
    if (remembered) hero_handle_report_remembered(where, h);
}

/* C HANDED THE ADDRESS BACK, whatever ended there before: a `borrows` result
 * or cell, or a handle C passes to a callback. The callback's clear runs in a
 * THUNK at the address handed to C and never at the function's entry, because
 * a Heroes function called by name would otherwise clear the mark on a stale
 * copy handed to it (panel 177's critic, `cb_launder`, exit 0 reading freed
 * memory on three legs). */
void hero_handle_lent(const void *h) {
    if (h == NULL) return;
    hero_handle_lock_take(&hero_handle_lock);
    hero_dead_clear_locked(h);
    hero_handle_lock_drop(&hero_handle_lock);
}

/* `==` on two handles (spec § 13 compares the address). A dead value no
 * longer holds the address it had: two dead handles would read equal, and a
 * dead one would differ from its own earlier copy (panel 177's
 * compiler-engineer, E6), so a read of one is refused here as it is at C. */
bool hero_handle_eq(const void *a, const void *b) {
    void *dead = atomic_load_explicit(&hero_handle_dead_page, memory_order_acquire);
    if (dead != NULL && (a == dead || b == dead)) {
        fflush(stdout);
        fprintf(stderr, "panic: `==` read a dead C handle: a call marked `consumes` or `transfers` "
                        "ended its life and emptied the binding, field or element it was read "
                        "from, and a dead value holds no address to compare. Compare a handle "
                        "before giving it back, or keep a reference with `retains`\n");
        hero_abort();
    }
    return a == b;
}
/* THE CROSSING: the address IS live, and the call giving it back is not one
 * its mark named (defect 075: `popen`, then `fclose`, was check 0 and run 0 on
 * four platforms, and the child was never waited for). Raised before C runs,
 * like the stray, and it says what the two declarations say and nothing about
 * why they disagree. */
static void hero_handle_report_crossed(const void *h, const char *owed, const char *by) {
    fflush(stdout);
    fprintf(stderr, "panic: a C handle was given back to `%s`, and the mark that began its "
                    "life names `%s` — the set names the calls that may end this "
                    "handle's life, and `%s` is not one of them. The handle is at %p\n",
            by, owed, by, h);
    hero_abort();
}

void hero_handle_acquired(const void *h, const char *by) {
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
    if (hero_handle_set[i].h == NULL) {
        hero_handle_set[i].h = h;
        hero_handle_live++;
    }
    /* The NEWEST mark wins: an address C hands out again belongs to the life
     * that just began, whatever ended the last one out of sight — and it is
     * one reference, whatever the last life had left. */
    hero_handle_set[i].by = by;
    hero_handle_set[i].n = 1;
    hero_handle_set[i].pending = 0;
    /* C handed this address out again, so whatever ended there is over: a copy
     * of the old life at this address is now the new life's, and only
     * `--sanitize`'s quarantine can still tell them apart (panel 177's item 4). */
    hero_dead_clear_locked(h);
    hero_handle_lock_drop(&hero_handle_lock);
}

/* An end the call has announced and not yet made: `hero_handle_ending` and
 * `hero_handle_transferring` count it here before C runs, `hero_handle_ended`
 * makes it after, and `hero_handle_kept` withdraws it when the call said it
 * failed. More announced than held is the one-call double release, stopped
 * before C. Called with the lock held; drops it before a report. */
static void hero_handle_announce(size_t i, const void *h) {
    hero_handle_set[i].pending++;
    if (hero_handle_set[i].pending > hero_handle_set[i].n) {
        size_t held = hero_handle_set[i].n;
        hero_handle_set[i].pending--;
        hero_handle_lock_drop(&hero_handle_lock);
        hero_handle_report_twice_in_one_call(h, held);
    }
}

/* The slot of a live address, or a stray report: the lock is held on return
 * and dropped before the report. Shared by the three calls that read a life
 * the program must already hold. */
static size_t hero_handle_held(const void *h) {
    /* AN EMPTY SET HOLDS NOTHING, SO THIS IS A STRAY TOO, AND IT IS STOPPED LIKE
     * ONE (defect 086, 2026-09-24). The branch used to count and return, which
     * was right before defect 071 moved the report to the call; afterwards it
     * left a program that had acquired nothing yet — every producer `borrows` —
     * to reach C with its double release, 133 and zero bytes, while the same
     * program after one acquisition was stopped before C with the line. A
     * promise about a call must not depend on the program's history. */
    if (hero_handle_cap == 0) {
        hero_handle_strays++;
        if (hero_handle_first_stray == NULL) hero_handle_first_stray = h;
        hero_handle_lock_drop(&hero_handle_lock);
        hero_handle_report_stray(1, h);
    }
    size_t i = hero_handle_slot(hero_handle_set, hero_handle_cap, h);
    if (hero_handle_set[i].h != h) {
        hero_handle_strays++;
        if (hero_handle_first_stray == NULL) hero_handle_first_stray = h;
        hero_handle_lock_drop(&hero_handle_lock);
        hero_handle_report_stray(1, h);
    }
    return i;
}

/* THE CHECK BEFORE C RUNS (defect 071's order, kept): the address is held,
 * and the consumer is one the acquiring mark named, or may pay through its
 * own mark. Nothing is mutated here; `hero_handle_ended` is the other half,
 * after the call, so a `when` clause can make the end conditional. */
void hero_handle_ending(const void *h, const char *by, const char *pays) {
    if (h == NULL) return;
    hero_handle_lock_take(&hero_handle_lock);
    size_t i = hero_handle_held(h);
    const char *owed = hero_handle_set[i].by;
    if (owed != NULL && by != NULL && !hero_handle_named(owed, by) && !hero_handle_sets_share(owed, pays)) {
        hero_handle_lock_drop(&hero_handle_lock);
        hero_handle_report_crossed(h, owed, by);
    }
    hero_handle_announce(i, h);
    hero_handle_lock_drop(&hero_handle_lock);
}

/* THE CALL SAID IT FAILED: the end it announced is withdrawn and the life
 * stays the program's, with the count it had. A `when` clause, or a NULL
 * handle result under a transfer (panel 177's item 6). */
void hero_handle_kept(const void *h) {
    if (h == NULL) return;
    hero_handle_lock_take(&hero_handle_lock);
    size_t i = hero_handle_held(h);
    if (hero_handle_set[i].pending > 0) hero_handle_set[i].pending--;
    hero_handle_lock_drop(&hero_handle_lock);
}

/* A TRANSFER (panel 176's item 2, the historian's V1c): the life goes into
 * another value that will end it with `into`, so the handle's own set must
 * name `into` — `fclose` relabelled as `transfers pclose` on a `popen` stream
 * is refused here, and so is a real `BIO_new_fp(BIO_CLOSE)` over a `popen`
 * stream, which C would end with `fclose` and leave a child unwaited. */
static void hero_handle_report_misdirected(const void *h, const char *owed, const char *into) {
    fflush(stdout);
    fprintf(stderr, "panic: a C handle was handed into a value that will end it with `%s`, "
                    "and the mark that began its life names `%s` — the set names the "
                    "calls that may end this handle's life, and `%s` is not one of them. "
                    "The handle is at %p\n",
            into, owed, into, h);
    hero_abort();
}

void hero_handle_transferring(const void *h, const char *into) {
    if (h == NULL) return;
    hero_handle_lock_take(&hero_handle_lock);
    size_t i = hero_handle_held(h);
    const char *owed = hero_handle_set[i].by;
    if (owed != NULL && into != NULL && !hero_handle_sets_share(owed, into)) {
        hero_handle_lock_drop(&hero_handle_lock);
        hero_handle_report_misdirected(h, owed, into);
    }
    hero_handle_announce(i, h);
    hero_handle_lock_drop(&hero_handle_lock);
}

/* A REFERENCE (panel 176's item 4, kept as the critic measured it): on a live
 * address one more, and the reference must share a releaser with the life it
 * joins, which the set KEEPS; on an address the set does not hold, a first
 * life owed to the reference's own releasers. */
static void hero_handle_report_joined_wrong(const void *h, const char *owed, const char *set) {
    fflush(stdout);
    fprintf(stderr, "panic: a reference marked `retains %s` was taken to a C handle whose "
                    "life a mark owed to `%s` — the two name no releaser in common, so one "
                    "of the two releases the program now owes would end the other's life "
                    "the wrong way. The handle is at %p\n",
            set, owed, h);
    hero_abort();
}

void hero_handle_retained(const void *h, const char *releasers) {
    if (h == NULL) return;
    hero_handle_lock_take(&hero_handle_lock);
    if (hero_handle_cap == 0 || (hero_handle_live + 1) * 2 > hero_handle_cap) hero_handle_grow();
    size_t i = hero_handle_slot(hero_handle_set, hero_handle_cap, h);
    if (hero_handle_set[i].h == h) {
        const char *owed = hero_handle_set[i].by;
        if (owed != NULL && releasers != NULL && !hero_handle_sets_share(owed, releasers)) {
            hero_handle_lock_drop(&hero_handle_lock);
            hero_handle_report_joined_wrong(h, owed, releasers);
        }
        hero_handle_set[i].n++;
    } else {
        hero_handle_set[i].h = h;
        hero_handle_set[i].by = releasers;
        hero_handle_set[i].n = 1;
        hero_handle_set[i].pending = 0;
        hero_handle_live++;
        /* A life begins on an address the set did not hold: C handed it out. */
        hero_dead_clear_locked(h);
    }
    hero_handle_lock_drop(&hero_handle_lock);
}

/* THE END, after the call: one reference fewer, the announcement made good,
 * and the entry leaves the set at zero. A second end of an address the set no
 * longer holds is a stray, and the report names its causes.
 *
 * ANSWERS WHETHER THE LIFE ENDED (panel 177's item 1): 1 when the entry left
 * the set, 0 while references remain. The emitted C poisons the place the
 * argument was read from under that answer and never unconditionally, so a
 * second release through one name of a handle that `retains` gave two
 * references is not refused: the llm-ergonomist's A3, `X509_up_ref` then
 * `X509_free` twice through `cert`. The address is remembered as dead at the
 * same moment, under the same lock, so no check can run between the two. */
int hero_handle_ended(const void *h) {
    if (h == NULL) return 0;
    hero_handle_lock_take(&hero_handle_lock);
    size_t i = hero_handle_held(h);
    if (hero_handle_set[i].pending > 0) hero_handle_set[i].pending--;
    if (hero_handle_set[i].n > 1) {
        hero_handle_set[i].n--;
        hero_handle_lock_drop(&hero_handle_lock);
        return 0;
    }

    /* Backward-shift deletion, because linear probing cannot tombstone without
     * the table filling with tombstones on a long-running program. */
    size_t hole = i;
    size_t scan = (i + 1) & (hero_handle_cap - 1);
    hero_handle_set[hole].h = NULL;
    hero_handle_set[hole].by = NULL;

    while (hero_handle_set[scan].h != NULL) {
        size_t home = (size_t)(((uintptr_t)hero_handle_set[scan].h * (uintptr_t)0x9E3779B97F4A7C15u) >> 32) & (hero_handle_cap - 1);
        size_t from_hole = (scan - hole) & (hero_handle_cap - 1);
        size_t from_home = (scan - home) & (hero_handle_cap - 1);

        if (from_home >= from_hole) {
            hero_handle_set[hole] = hero_handle_set[scan];
            hero_handle_set[scan].h = NULL;
            hero_handle_set[scan].by = NULL;
            hole = scan;
        }
        scan = (scan + 1) & (hero_handle_cap - 1);
    }
    hero_handle_live--;
    hero_dead_mark_locked(h);
    hero_handle_lock_drop(&hero_handle_lock);
    return 1;
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
