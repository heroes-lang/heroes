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
/* `sysconf` and `_SC_PAGESIZE`, named here rather than inherited. `parts/stack.c`
 * includes this header above us in the one translation unit and that is exactly
 * why it must be repeated: stack.c's ASan arm compiles the whole POSIX half
 * away, so under `--sanitize` the include vanishes and this file stops
 * compiling. Measured 2026-09-06 while landing panel 115's floor — the ordinary
 * build was green and `heroes build --sanitize` answered `internal error: the
 * runtime did not compile`. A file that leans on another file's includes works
 * until that file grows a configuration. */
#include <unistd.h>
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

/* THE FLOOR IS THE THREAD THAT RAN `main` (panel 115, 2026-09-06). A thread this
 * runtime starts never gets less stack than the thread the program started on.
 *
 * WHAT THAT REPAIRS, measured on this Mac rather than argued: the same recursion
 * reaches **19** levels on a worker and **312** on `main` — 15.6:1, one machine,
 * one program, one runtime — because Darwin hands a created thread 536,576 bytes
 * against the process's 8,372,224. The surprise is not that platforms differ; it
 * is that one machine gives its own two threads different answers, and a program
 * that works when written works differently when moved onto a thread.
 *
 * WHY A FLOOR AND NOT A NUMBER. The sitting was convened on "the same size
 * everywhere, 8 MiB" and every seat that looked found the number indefensible:
 * it is glibc's default imported (spec-warden), it CUTS Windows eightfold from
 * the 64 MiB `selfhost/cli/flags.hero` links for a measured reason (compiler and
 * ffi seats, historian), and the Windows call that would deliver it sets the
 * COMMIT and not the reserve without `STACK_SIZE_PARAM_IS_A_RESERVATION` — a
 * defect libuv ships to this day (historian, Mozilla bug 958796). A floor needs
 * no number: it is a fact about the machine in hand, so there is nothing to
 * argue, nothing to age, and nothing to state in the spec. On glibc it is inert
 * because a created thread already gets the process's 8 MiB; on Windows it does
 * not run at all. It raises exactly one platform and lowers none.
 *
 * LLVM and Chromium reached the same answer from the same Darwin measurement,
 * which is precedent rather than invention (panel 115, the historian's Q3).
 *
 * WINDOWS IS DELIBERATELY UNTOUCHED. `CreateThread(NULL, 0, ...)` below keeps
 * the executable's own reserve, which Microsoft documents as the default "for
 * all threads and fibers", so this project's `/STACK:67108864` already applies
 * to every thread there and 67,108,864 is measured on both, main and worker. */
static size_t hero_spawn_floor = 0;

#if !defined(_WIN32)
/* The calling thread's stack size, asked of the OS. Deliberately NOT
 * `stack.c`'s `hero_stack_bounds()`: that arm is compiled out under the
 * sanitiser, where the guard yields to ASan, and a floor that disappeared under
 * `--sanitize` would be missing on exactly the leg CLAUDE.md § Commands sends
 * every `extern` program to. */
static size_t hero_spawn_stack_of_self(void) {
#if defined(__APPLE__)
    return pthread_get_stacksize_np(pthread_self());
#else
    pthread_attr_t attr;
    void *addr = NULL;
    size_t size = 0;

    if (pthread_getattr_np(pthread_self(), &attr) == 0) {
        pthread_attr_getstack(&attr, &addr, &size);
        pthread_attr_destroy(&attr);
    }
    return size;
#endif
}
#endif

/* Measured once, on the thread that runs `main`, from `hero_args_set` — the one
 * call every generated `main` makes before a program's first line, which is
 * `parts/stack.c`'s precedent and `parts/thread.c`'s. It has to be captured
 * there and not lazily: a spawned thread may itself spawn, and by then the
 * calling thread is a worker whose size is the wrong reference. */
static void hero_spawn_measure_home(void) {
#if defined(_WIN32)
    hero_spawn_floor = 0;
#else
    hero_spawn_floor = hero_spawn_stack_of_self();
#endif
}

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
    /* AND THE STACK GUARD IS CLAIMED HERE TOO, which is the whole of
     * `M-thread-stacks`' door (panel 107). Its bounds and its alternate stack
     * are the calling thread's, so a worker that recurses to the end of its
     * stack was exit 132 with an empty stderr where `main` says `panic: stack
     * exhausted in <function>` — measured on this Mac 2026-09-06, both ways.
     * Panel 107 could not place this call: on 2026-09-04 no Heroes function ran
     * on any thread but `main`, so there was no point at which to make it. */
    hero_stack_guard_enter();

    /* WHAT THE OS GAVE, NEVER WHAT IT ANSWERED (panel 115, the ffi seat, and it
     * is the trap this whole repair could have walked into). Measured on both
     * platforms in that sitting, `pthread_attr_setstacksize` refuses OPPOSITE
     * things: a size below the minimum returns 0 and clamps on Darwin but is
     * EINVAL on glibc, while a size that is not a page multiple is EINVAL on
     * Darwin and rounds on glibc — and on the arm that returns EINVAL the attr
     * KEEPS ITS DEFAULT. An implementation that trusts the return code
     * therefore restores the exact defect it was written to fix, silently, on
     * one platform only. Demonstrated end to end in the sitting: 8 MiB + 1 with
     * the return discarded gives `panic: stack exhausted in depth.down` at 20,000
     * levels, blaming the author's recursion for a thread that got 536,576 bytes.
     *
     * So the check is here, after the thread exists, and it asks the OS. One
     * page of slop, because Darwin returns 12,288 bytes MORE than asked
     * (8,388,608 -> 8,400,896, measured) and a platform is free to round. */
#if !defined(_WIN32)
    if (hero_spawn_floor > 0) {
        size_t got = hero_spawn_stack_of_self();
        size_t slop = (size_t)sysconf(_SC_PAGESIZE);

        if (got > 0 && got + slop < hero_spawn_floor) {
            hero_panic("a thread was started with less stack than this runtime "
                       "asked for — it asked for the stack the program itself "
                       "runs on, and the operating system gave less");
        }
    }
#endif
    slot->result = slot->body(slot->arg);
    /* The alternate stack goes back to the OS with the thread that took it: a
     * slot is reused (a bound on threads ALIVE, above), so a mapping kept per
     * spawn would grow without bound in a program that spawns in a loop. */
    hero_stack_guard_leave();
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
    /* The floor, asked for only when the platform's own default is BELOW it, so
     * a machine that already gives a worker what `main` has is untouched and
     * `attr` is never built there (glibc, measured: 8,388,608 both ways). */
    pthread_attr_t attr;
    pthread_attr_t *ask = NULL;
    int asked = 0;

    if (hero_spawn_floor > 0 && pthread_attr_init(&attr) == 0) {
        size_t theirs = 0;
        asked = 1;

        if (pthread_attr_getstacksize(&attr, &theirs) == 0 && theirs < hero_spawn_floor) {
            size_t page = (size_t)sysconf(_SC_PAGESIZE);
            size_t want = (hero_spawn_floor + page - 1) / page * page;

            if (pthread_attr_setstacksize(&attr, want) == 0) ask = &attr;
        }
    }
    int made = pthread_create(&hero_spawn_slots[at].handle, ask, hero_spawn_trampoline,
                              &hero_spawn_slots[at]) == 0;
    if (asked) pthread_attr_destroy(&attr);
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
