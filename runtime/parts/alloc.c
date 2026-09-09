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

int64_t hero_runtime_live(void) { return hero_live_blocks; }

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
