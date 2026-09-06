/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/spawn.c — starting a thread, and waiting for it (design.md Part 7.13).
 *
 * WHY THE RUNTIME OWNS THIS AND NOT THE PROGRAM. There is no header that spells
 * threads on all three of this project's platforms. Measured 2026-09-06:
 * `pthread.h` does not exist under clang targeting `x86_64-pc-windows-msvc`,
 * and C11's `<threads.h>` does not exist in the macOS SDK while it does exist
 * on Windows and on glibc. The intersection is empty, and gnulib's own platform
 * list says the same about `<threads.h>` for thirteen platforms including
 * macOS. So a program that named a header directly could not be one program.
 *
 * The arm therefore lives here, in C, which is where every other platform
 * difference in this project already lives — `dir.c`, `fs.c`, `run.c`,
 * `stack.c`, `os.c`, `f64.c`, `alloc.c` — and which `selfhost/cli/io.hero:22`
 * states as doctrine in the repository's own words: *"The route out is not a
 * platform arm — this language has no `#if`, by design — but ... the platform
 * question lives in the runtime's C, the one file in this project that is
 * allowed to know what machine it is on."* Panel 114 refused to put a form in
 * the language for this, on three vetoes and none of them the token budget.
 *
 * WHY THE HANDLE IS AN `int64_t` AND NOT A THREAD. `pthread_t` is an opaque
 * pointer on Darwin, an `unsigned long` on glibc and a `HANDLE` on Windows, so
 * a binding that named it would need three spellings and would be a different
 * program on each machine. An index into this file's table is the same spelling
 * everywhere, it is a number Heroes already has, and it turns two mistakes that
 * are undefined behaviour in C — joining twice, joining something that was
 * never started — into a named panic.
 *
 * WHAT MAY CROSS, AND IT IS ENFORCED BY THE CHECKER RATHER THAN BY THIS
 * COMMENT. The body is `int64_t (*)(int64_t)`. A Heroes program binds it
 * through `hero_os.h`, so `selfhost/check/ffi.hero`'s `crosses_the_boundary`
 * judges the callback's own parameters and result — which it did not do until
 * defect 014 was repaired the same day — and a `[T]` or a `{K: V}` inside that
 * signature is `error[ffi_type]` on the author's line. So no refcounted Heroes
 * value reaches a second thread, which is Part 7.13's isolation obtained from
 * the type rule rather than promised in prose. Panel 113 measured what the
 * absence of that rule cost: a container crossed inside a callback signature at
 * exit 0 with an empty stderr.
 *
 * AND THE THREAD IS THIS PROGRAM'S, WHICH IS WHY THE GUARD LETS IT THROUGH.
 * `parts/thread.c` refuses a Heroes function on any thread the program did not
 * start (panel 111 R9). A thread started here IS one the program started, so
 * `hero_thread_enter` claims it before the body runs. A thread a C library made
 * for its own purposes still stops by name, and that refusal is not weakened by
 * one line of this file.
 */

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

/* THE BOUND IS ON THREADS ALIVE AT ONCE, NOT ON THREADS EVER STARTED, and that
 * distinction is the whole reason a slot is reused. A bound on starts would
 * make `for i in range(...)` spawn-and-join exhaust the table in a loop that is
 * doing nothing wrong; a bound on concurrency is the machine's own limit
 * written down. design.md Part 7.13 asks for "OS threads — few, real,
 * OS-scheduled", so 256 is an order of magnitude above what the model is for,
 * on `run.c`'s precedent for a bound of this shape.
 *
 * Overflow is a panic and never a truncation, for `run.c`'s reason: a program
 * that gets fewer threads than it asked for computes a wrong answer quietly. */
#define HERO_SPAWN_SLOTS 256

typedef int64_t (*HeroThreadBody)(int64_t);

typedef struct {
#if defined(_WIN32)
    HANDLE handle;
#else
    pthread_t handle;
#endif
    HeroThreadBody body;
    int64_t arg;
    int64_t result;
    /* 0 free · 1 running · 2 finished and not yet joined. A joined slot goes
     * back to 0, which is what makes the bound a concurrency bound. */
    int state;
} HeroSpawnSlot;

static HeroSpawnSlot hero_spawn_slots[HERO_SPAWN_SLOTS];

/* THE TABLE IS SHARED AND IT IS THE ONE PLACE IN THIS RUNTIME THAT TAKES A
 * LOCK. Every other shared object here is either `_Thread_local`, `_Atomic`, or
 * written down in `tests/harness/suite_runtime.hero`'s allow-list with the
 * reason it may stay shared. This one cannot be any of those: a spawned thread
 * may itself spawn, so two threads can be choosing a slot at the same moment,
 * and choosing a slot is a read-then-write over several fields rather than one
 * counter. That is the shape an atomic cannot make single — the same shape
 * `cow.c`'s `if (refcount == 1)` has, which panel 113 left open for exactly
 * this reason. Here the answer is a mutex, because here we own both sides. */
#if defined(_WIN32)
static SRWLOCK hero_spawn_lock = SRWLOCK_INIT;
#define HERO_SPAWN_TAKE() AcquireSRWLockExclusive(&hero_spawn_lock)
#define HERO_SPAWN_DROP() ReleaseSRWLockExclusive(&hero_spawn_lock)
#else
static pthread_mutex_t hero_spawn_lock = PTHREAD_MUTEX_INITIALIZER;
#define HERO_SPAWN_TAKE() pthread_mutex_lock(&hero_spawn_lock)
#define HERO_SPAWN_DROP() pthread_mutex_unlock(&hero_spawn_lock)
#endif

/* The body runs here, and `hero_thread_claim` is the first thing that happens
 * on it: this thread is one the program started, so `parts/thread.c`'s guard
 * must let a Heroes function run on it. Claiming before the body and not inside
 * it is deliberate — the body is the author's Heroes function and it must not
 * have to know that any of this exists. */
static void hero_spawn_enter(HeroSpawnSlot *slot) {
    hero_thread_claim();
    slot->result = slot->body(slot->arg);
}

#if defined(_WIN32)
static DWORD WINAPI hero_spawn_trampoline(LPVOID p) {
    hero_spawn_enter((HeroSpawnSlot *)p);
    return 0;
}
#else
static void *hero_spawn_trampoline(void *p) {
    hero_spawn_enter((HeroSpawnSlot *)p);
    return NULL;
}
#endif

int64_t hero_thread_spawn(HeroThreadBody body, int64_t arg) {
    if (body == NULL) hero_panic("a thread was started with no function to run");
    HERO_SPAWN_TAKE();
    int64_t at = -1;

    for (int64_t i = 0; i < HERO_SPAWN_SLOTS; i += 1) {
        if (hero_spawn_slots[i].state == 0) {
            at = i;
            break;
        }
    }

    if (at < 0) {
        HERO_SPAWN_DROP();
        hero_panic("too many threads running at once — this runtime holds 256, "
                   "and a thread's slot comes back when it is joined");
    }
    hero_spawn_slots[at].body = body;
    hero_spawn_slots[at].arg = arg;
    hero_spawn_slots[at].result = 0;
    hero_spawn_slots[at].state = 1;
#if defined(_WIN32)
    hero_spawn_slots[at].handle =
        CreateThread(NULL, 0, hero_spawn_trampoline, &hero_spawn_slots[at], 0, NULL);
    int made = hero_spawn_slots[at].handle != NULL;
#else
    int made = pthread_create(&hero_spawn_slots[at].handle, NULL, hero_spawn_trampoline,
                              &hero_spawn_slots[at]) == 0;
#endif

    if (!made) {
        hero_spawn_slots[at].state = 0;
        HERO_SPAWN_DROP();
        hero_panic("the operating system refused to start a thread");
    }
    HERO_SPAWN_DROP();
    return at;
}

/* Waiting is where the two undefined behaviours of C become named panics. The
 * checks read the slot under the lock and then wait OUTSIDE it, because joining
 * while holding the table would stop every other thread from starting one. */
int64_t hero_thread_join(int64_t handle) {
    if (handle < 0 || handle >= HERO_SPAWN_SLOTS) {
        hero_panic("a thread handle that this program never received was joined");
    }
    HERO_SPAWN_TAKE();
    int state = hero_spawn_slots[handle].state;
    HERO_SPAWN_DROP();

    if (state == 0) {
        hero_panic("a thread was joined twice, or joined before it was started — "
                   "a handle is spent once the thread it names has been waited for");
    }
#if defined(_WIN32)
    WaitForSingleObject(hero_spawn_slots[handle].handle, INFINITE);
    CloseHandle(hero_spawn_slots[handle].handle);
#else
    pthread_join(hero_spawn_slots[handle].handle, NULL);
#endif
    HERO_SPAWN_TAKE();
    int64_t out = hero_spawn_slots[handle].result;
    hero_spawn_slots[handle].state = 0;
    hero_spawn_slots[handle].body = NULL;
    HERO_SPAWN_DROP();
    return out;
}

/* How many a program may have running at once, so a Heroes program can ask
 * rather than learn it from a panic. A value, not a form — `hero_word_bits`'s
 * precedent (DESIGN-LOG 2026-08-26): where the machine knows something, ask C
 * for it through an ordinary declaration instead of writing a guess into the
 * language. */
int64_t hero_thread_limit(void) { return HERO_SPAWN_SLOTS; }
