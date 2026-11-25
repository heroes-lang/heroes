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
 * WHAT CROSSES IS ONE `int64_t` EACH WAY. The body is `int64_t (*)(int64_t)`,
 * and spec section 13 says so since panel 246's R3 (2026-11-13). A Heroes
 * program binds it through `hero_os.h`: `selfhost/check/ffi.hero`'s
 * `crosses_the_boundary` judges the callback's own parameters and result
 * (since defect 014), so a `[T]` or a `{K: V}` in that signature is
 * `error[ffi_type]` on the author's line, and any other type, a `str` or a
 * handle, passes the checker and is refused by clang at the build against
 * `hero_os.h`'s prototype (`ffi_callback_type`), as panel 246's seats
 * measured. So no refcounted Heroes value reaches a second thread: Part 7.13's
 * isolation is obtained from the prototype and the type rule together. Until
 * 2026-11-13 this paragraph said the checker alone enforced it, which held for
 * arrays and maps alone (lane b34-signals found it beside `hero_os.h`'s twin).
 * Panel 113 measured what the absence of the type rule cost: a container
 * crossed inside a callback signature at exit 0 with an empty stderr. Beside
 * it, one `str` each way, its bytes copied at each crossing, through the
 * doors of `parts/spawn_str.c` (decision 2498; feature 2521), which start
 * and join through this file and are held to their own prototype the same
 * way.
 *
 * AND THE THREAD IS THIS PROGRAM'S, WHICH IS WHY THE GUARD LETS IT THROUGH.
 * `parts/thread.c` refuses a Heroes function on any thread the program did not
 * start (panel 111 R9). A thread started here IS one the program started, so
 * `hero_spawn_enter` claims it before the body runs. A thread a C library made
 * for its own purposes still stops by name, and that refusal is not weakened by
 * one line of this file.
 *
 * FROM ITS FIRST INSTRUCTION TO ITS LAST, NOT ONLY FROM ITS CLAIM (defect
 * 2496). A signal sent to the process runs its handler on any thread that does
 * not block it, and a thread exists for the kernel before it has run a line of
 * `hero_spawn_enter`: a Heroes function given to C's `signal` ran on a worker
 * not yet claimed and was refused as a thread the program did not start, exit
 * 134, in 8 runs of 50 on this Mac and 3 of 50 in Linux arm64 (the issue's
 * `whichthread2`, SIGINT blocked on `main`, 2026-11-13). So no such signal
 * reaches a thread of this file while its runtime state is being built or
 * given back: the starter holds them off itself across the start, the new
 * thread inherits that, and it takes its starter's own mask back only once its
 * claim and its stack guard stand (`hero_spawn_hold`, below).
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
/* `EINVAL`, for a size no page rounding can hold (panel 184 R5), and
 * `getrlimit`, for a main thread whose stack has no limit; named here for the
 * reason above: `os.c` and `run.c` include them, after this file. */
#include <errno.h>
#include <sys/resource.h>
/* `pthread_sigmask` and `sigset_t`, for the start's held signals (defect
 * 2496); named here for the reason above, though `panic.c` reads it first. */
#include <signal.h>
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

    /* AN UNLIMITED STACK HAS NO SIZE TO MATCH (2026-10-03, lane depth, beside
     * panel 184's R5). Under `ulimit -s unlimited` glibc answers the main
     * thread's size as all the room down to the mapping below it,
     * 93,823,035,207,680 bytes in the Linux arm64 container, and no thread can
     * be given that: `examples/threads` panicked at its first spawn, *the
     * operating system refused to start a thread*, with this file as it stood
     * at `5bcf0368`; and the compiler's own thread, which takes the larger of
     * its 256 MiB and the floor, was asked for that much too, and granted it
     * in a container whose `vm.overcommit_memory` is 1. The main thread then
     * grows until memory ends, so there is no floor to keep: a thread asked
     * for nothing gets what glibc gives a thread under an unlimited limit (2
     * MiB there), and one asked for a size gets it. Measured the same day
     * after the change: the example and the compiler both run under the
     * unlimited limit. */
    struct rlimit room;
    if (getrlimit(RLIMIT_STACK, &room) == 0 && room.rlim_cur == RLIM_INFINITY) {
        hero_spawn_floor = 0;
    }
#endif
}

typedef int64_t (*HeroThreadBody)(int64_t);

/* WHICH JOIN A THREAD TAKES (feature 2521). A thread of `hero_thread_spawn`
 * answers an `i64`; one of `hero_thread_spawn_str` answers the address of a
 * block holding a `str`'s bytes (`parts/spawn_str.c`), and each is joined by
 * its own door, so that address never reaches a program as a number, nor a
 * number a program chose is read as an address. */
#define HERO_SPAWN_I64 0
#define HERO_SPAWN_STR 1
/* A thread of the language's own `spawn` (spec § 11; feature 2647): its value
 * crosses in a box the program's C fills and reads (`parts/mailbox.c`), and
 * the program's handle names the slot and the start's generation together. */
#define HERO_SPAWN_LANG 2

typedef struct {
#if defined(_WIN32)
    HANDLE handle;
#else
    pthread_t handle;
#endif
    HeroThreadBody body;
    int64_t arg;
    int64_t result;
    /* HERO_SPAWN_I64 or HERO_SPAWN_STR, the door that started it. */
    int kind;
    /* The stack this thread was asked for, the floor below or more where its
     * starter named a size (`hero_thread_spawn_sized`, panel 184 R5); 0 asks
     * for nothing and is what Windows records for an unsized thread. */
    size_t want;
    /* 0 free · 1 running · 2 finished and not yet joined. A joined slot goes
     * back to 0, which is what makes the bound a concurrency bound. The end
     * of `main` reads it before anything else (`hero_spawn_check_joined`). */
    int state;
    /* The function the program handed the door, and the address the door was
     * called from: what the end of `main` names a thread nobody joined by
     * (panel 251's R2). Neither is ever called through or read as data. */
    uintptr_t runs;
    uintptr_t from;
    /* How many starts this slot has seen, under the table's lock: a thread of
     * the language is handed out as this number and the slot together, so a
     * copy kept past its wait names a start that is over, never the slot's next
     * thread (`hero_thread_lang_handle`, `parts/mailbox.c`). */
    uint64_t generation;
    /* Where the language's `spawn` stood, for the end of `main` to name. */
    const HeroPlace *place;
    /* 1 once the function the thread was started with has returned, so
     * `result` holds its answer; 0 from the start (panel 258's R1, defect
     * 2754). A thread C ended before that, by `pthread_exit` or a
     * cancellation in a function it called, never set it, and its wait says
     * so instead of reading an answer nobody put. Written by the thread, read
     * by its join after the system's join, which orders the two. */
    int answered;
    /* 1 where the thread was seen ending without returning (POSIX's cleanup
     * handler, below), so the end of `main` tells it as ended rather than
     * running. */
    int ended;
    /* The thread that holds a language thread's value, by the runtime's own
     * number (`parts/alloc.c`, `hero_this_thread`): its starter, or the last to
     * receive it, HERO_HELD_IN_TRANSIT while a mailbox carries it, 0 once its
     * wait claimed it. Only the holder waits for it (panel 254's R3). */
    uint64_t holder;
#if !defined(_WIN32)
    /* The signal mask of the thread that started this one, as it was before
     * the start held the signals off (defect 2496): what POSIX says a new
     * thread inherits, which the new thread takes once it is the program's
     * own. Written before the thread exists, read by it once, and the slot is
     * not reused before the thread is joined. */
    sigset_t mask;
    /* 1 once the new thread has its claim, its stack guard and that mask, and
     * the start returns only then (defect 2496, `hero_spawn_await`), under the
     * slot's own lock and condition, made at each start and given back at its
     * join. */
    int ready;
    pthread_mutex_t ready_lock;
    pthread_cond_t ready_cond;
#endif
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

#if !defined(_WIN32)
/* EVERY SIGNAL BUT A FAULT'S, HELD OFF THE CALLING THREAD (defect 2496); its
 * mask before, in `before` where the caller wants it back. `SIG_BLOCK`, never
 * `SIG_SETMASK`: a signal the thread already held stays held.
 *
 * The six a fault raises stay open because a fault's signal goes to the thread
 * that faulted and never arrives early from elsewhere, and because holding one
 * turns a fault into a hang on Darwin: a null read with SIGSEGV held hung until
 * SIGKILL on this Mac and died at 139 in Linux arm64 without its handler
 * (`blocked_fault.c`, 2026-11-13). libmalloc ends a heap it finds corrupt with
 * a trap, SIGTRAP, so a free in the thread's end that met a corruption would
 * have hung there rather than say so. `run.c`'s sentinel thread, which runs no
 * Heroes code and holds every signal for its whole life, is the precedent.
 *
 * `pthread_sigmask` fails only for a `how` it does not know, and the mask it
 * would leave unset is the one restored later, so a failure stops here. */
static void hero_spawn_hold(sigset_t *before) {
    sigset_t held;
    sigfillset(&held);
    sigdelset(&held, SIGSEGV);
    sigdelset(&held, SIGBUS);
    sigdelset(&held, SIGILL);
    sigdelset(&held, SIGTRAP);
    sigdelset(&held, SIGFPE);
    sigdelset(&held, SIGSYS);
    if (pthread_sigmask(SIG_BLOCK, &held, before) != 0) {
        hero_panic("internal error: the signals a thread's start holds off could not be held");
    }
}

/* AND THE START RETURNS ONLY ONCE THE NEW THREAD CAN TAKE A SIGNAL (defect
 * 2496), because Darwin does not keep a signal for the process: one sent while
 * every thread holds it off is parked on one thread, `main`, and handed to no
 * other thread that lets it through later, where Linux hands it to the first
 * that does (`routing.c`, 5 runs of 5 each way, 2026-11-13). With the start's
 * hold alone, the issue's program, whose `main` holds SIGINT for good and
 * sends it right after the start, was refused 0 times in 60 and ran its
 * handler in only 21 of them on this Mac: the other 39 signals waited on
 * `main` and were never handled. So the program's next line runs on a thread
 * that can take one, and only a signal sent from outside during the start
 * itself can find every thread holding it.
 *
 * Slept on, under a lock and a condition of the slot's own: the new thread
 * takes that lock with signals let through, and a handler run on it then that
 * started a thread would take another slot's, never this one. What it costs,
 * over 10,000 starts and joins on this Mac (`instructions retired`): about
 * 24,000 instructions a start, 14%, for the whole of defect 2496's repair,
 * where a wait polled with `sched_yield` had cost about 80,000 on its own. */
static void hero_spawn_await(HeroSpawnSlot *slot) {
    pthread_mutex_lock(&slot->ready_lock);
    while (!slot->ready) pthread_cond_wait(&slot->ready_cond, &slot->ready_lock);
    pthread_mutex_unlock(&slot->ready_lock);
}

/* The slot's lock and condition, made just before its thread is created: 0,
 * or the number a refusal hands back as a start the system refused. */
static int hero_spawn_ready_made(HeroSpawnSlot *slot) {
    slot->ready = 0;
    int made = pthread_mutex_init(&slot->ready_lock, NULL);

    if (made != 0) return made;
    made = pthread_cond_init(&slot->ready_cond, NULL);
    if (made != 0) pthread_mutex_destroy(&slot->ready_lock);
    return made;
}

/* And given back once no thread can touch them: a refused create, or a join. */
static void hero_spawn_ready_given_back(HeroSpawnSlot *slot) {
    pthread_cond_destroy(&slot->ready_cond);
    pthread_mutex_destroy(&slot->ready_lock);
}

/* AND A SIGNAL ALREADY ON A THREAD THAT ENDS IS HANDLED, NEVER DROPPED WITH IT
 * (defect 2496). Darwin moves a signal sent to the process onto one thread
 * that lets it through, so one moved onto this thread just before its end held
 * them would die with the thread, where Linux keeps it for the process. With
 * a worker that ends as `main` sends, 300 runs of 200 signals on this Mac lost
 * 17 of 60,000 with the end's hold alone, and 1 and then 0 of 60,000 in two
 * readings with what follows. So once the
 * end holds them, every signal pending here that this thread's own mask lets
 * through is let through once more, while its claim, its room and its stack
 * guard all still stand, and the hold is taken again, until none is pending.
 * A program's own held signals stay held, being outside that mask; the rounds
 * are bounded, since a storm that outlasts them only coalesces into signals
 * the kernel already counts as one. */
static void hero_spawn_drain(const HeroSpawnSlot *slot) {
    for (int round = 0; round < 64; round += 1) {
        sigset_t pending;
        int any = 0;

        if (sigpending(&pending) != 0) return;
        for (int sig = 1; sig < NSIG && !any; sig += 1) {
            any = sigismember(&pending, sig) == 1 && sigismember(&slot->mask, sig) == 0;
        }
        if (!any) return;
        pthread_sigmask(SIG_SETMASK, &slot->mask, NULL);
        hero_spawn_hold(NULL);
    }
}
#endif

/* The body runs here, and `hero_thread_claim` is the first thing that happens
 * on it: this thread is one the program started, so `parts/thread.c`'s guard
 * must let a Heroes function run on it. Claiming before the body and not inside
 * it is deliberate — the body is the author's Heroes function and it must not
 * have to know that any of this exists. */
/* `parts/lend.c`, later in this translation unit: the thread's lend regions;
 * `parts/frame.c`: its large values' room (defect 2278). */
static void hero_lend_thread_leave(void);
static void hero_frame_thread_leave(void);
static void hero_spawn_leave(HeroSpawnSlot *slot);

#if !defined(_WIN32)
/* A THREAD C ENDS BEFORE ITS FUNCTION RETURNS (panel 258's R1, provisional,
 * author ratification pending; defect 2754): `pthread_exit` or a
 * cancellation in a C function the thread's function called. No answer was
 * put, and the end below `slot->body` never ran, so the slot stayed running,
 * the thread's lend regions, room and alternate stack were never given back,
 * and `wait` read the answer nobody put: `panic: a null pointer was read
 * through ... called from h_0take_...`, 3 runs of 3 of `pexit`. POSIX runs a
 * thread's cleanup handlers on both ways out, and both systems were measured
 * to (Darwin: a C probe, 2 of 2, and `pexit`'s shapes; glibc, which runs them
 * in its forced unwind: the same shapes in the Linux arm64 image, 2026-11-24,
 * the end of `main` telling the thread as ended, 3 runs of 3): this one marks
 * the slot ended and gives back
 * what the end below gives back, and `wait` then says the thread ended
 * without its answer. On Windows a thread C ends by `ExitThread` runs no
 * handler of this runtime's; its wait says the same from `answered`. */
static void hero_spawn_ended_unanswered(void *p) {
    HeroSpawnSlot *slot = (HeroSpawnSlot *)p;
    slot->ended = 1;
    hero_spawn_leave(slot);
}
#endif

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
#if !defined(_WIN32)
    /* ONLY NOW MAY A SIGNAL REACH THIS THREAD (defect 2496): it was started
     * with every signal but a fault's held off (`hero_spawn_start`), and its
     * claim and its alternate stack both stand, so a handler run here runs on
     * a thread the program owns, on a stack whose guard can name an overflow.
     * The mask it takes is its starter's from before the start, the one POSIX
     * would have given it, so a program's own masking is kept as it was. */
    pthread_sigmask(SIG_SETMASK, &slot->mask, NULL);
    pthread_mutex_lock(&slot->ready_lock);
    slot->ready = 1;
    pthread_cond_signal(&slot->ready_cond);
    pthread_mutex_unlock(&slot->ready_lock);
#endif

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
     * (8,388,608 -> 8,400,896, measured) and a platform is free to round.
     *
     * It reads what THIS thread asked for, the floor or a size its starter
     * named (panel 184 R5), and the sentence says which, because a thread
     * that asked for 256 MiB and was handed the default would otherwise be
     * reported as asking for the stack the program runs on. */
#if !defined(_WIN32)
    if (slot->want > 0) {
        size_t got = hero_spawn_stack_of_self();
        size_t slop = (size_t)sysconf(_SC_PAGESIZE);

        if (got > 0 && got + slop < slot->want) {
            char msg[256];
            snprintf(msg, sizeof msg,
                     "a thread was started with less stack than this runtime asked "
                     "for — it asked for %zu bytes and the operating system gave %zu",
                     slot->want, got);
            hero_panic(msg);
        }
    }
#endif
#if !defined(_WIN32)
    pthread_cleanup_push(hero_spawn_ended_unanswered, slot);
#endif
    slot->result = slot->body(slot->arg);
#if !defined(_WIN32)
    pthread_cleanup_pop(0);
#endif
    slot->answered = 1;
    hero_spawn_leave(slot);
}

/* WHAT A THREAD'S END GIVES BACK, whether its function returned or C ended it
 * (panel 258's R1). */
static void hero_spawn_leave(HeroSpawnSlot *slot) {
    /* What callbacks of this thread never finished (a C jump over their frames,
     * defect 2704) joins the sum the leak gate reads (`parts/callback.c`). */
    hero_callback_thread_ends();
    /* A print span the body left open is given back before anything else
     * ends, so stdout's lock never outlives its holder (`parts/panic.c`). */
    hero_print_thread_leave();
#if !defined(_WIN32)
    /* AND THE SIGNALS ARE HELD OFF AGAIN BEFORE ANY OF IT IS GIVEN BACK, the
     * other end of the start's window and the same cause (defect 2496): a
     * handler run between the three calls below would find its thread's large
     * values' room half freed, its lend regions unmapped or its alternate
     * stack gone, and one run after this function returns, while the C
     * library ends the thread, reads the claim as a thread-local Darwin has
     * already given back and made anew at zero. Measured with a worker that
     * ends as `main` sends its signal: with no hold here 2 runs of 50 were
     * refused as a thread the program did not start on this Mac, and 0 of 50
     * with it. The thread ends holding them, and a signal sent to the process
     * meanwhile goes to one that does not. */
    hero_spawn_hold(NULL);
#endif
    /* FINISHED, AND NOT YET JOINED (panel 251's R2): the program's function
     * has returned, so the end of `main` tells this thread as finished rather
     * than running. Under the table's lock, as every write of `state` is, and
     * on POSIX with the signals held, since a handler that started a thread
     * would wait for the lock it interrupted. */
    HERO_SPAWN_TAKE();
    slot->state = 2;
    HERO_SPAWN_DROP();
#if !defined(_WIN32)
    hero_spawn_drain(slot);
#endif
    /* The alternate stack goes back to the OS with the thread that took it: a
     * slot is reused (a bound on threads ALIVE, above), so a mapping kept per
     * spawn would grow without bound in a program that spawns in a loop. And
     * the regions its buffers were lent from (panel 196's R4), for the same
     * reason and under the sanitizer too, where the guard's two doors are
     * empty and these mappings are still the thread's. */
    hero_lend_thread_leave();
    hero_frame_thread_leave();
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

/* Every door below starts here, `parts/spawn_str.c`'s too, `kind` naming
 * which join the thread takes. `bytes` 0 asks for the floor alone, today's
 * thread; more asks for at least that much (panel 184 R5). A thread the
 * operating system will not start is -1 with its own number in `*why`, and
 * the caller decides what to say: the unsized door panics as it always has,
 * the sized one hands the refusal back. `runs` and `from` are the program's
 * function and the door's caller, kept for the end of `main` to name. */
static int64_t hero_spawn_start(HeroThreadBody body, int64_t arg, size_t bytes, int kind, int64_t *why, uintptr_t runs,
                                 uintptr_t from) {
    if (body == NULL) hero_panic("a thread was started with no function to run");
#if !defined(_WIN32)
    /* Held before the table's lock and given back after it (defect 2496): the
     * new thread inherits this thread's mask as `pthread_create` finds it, so
     * it starts with them held, and no handler can run on this thread while it
     * holds the table either, where one that started a thread would wait for
     * the lock it interrupted. */
    sigset_t before;
    hero_spawn_hold(&before);
#endif
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
        /* The language's `spawn` answers this as it answers the system's own
         * refusal, a failure the program can read (feature 2647); the doors of
         * `hero_os.h` keep their abort. */
        if (kind == HERO_SPAWN_LANG && why != NULL) {
#if !defined(_WIN32)
            pthread_sigmask(SIG_SETMASK, &before, NULL);
#endif
            *why = HERO_SPAWN_SLOTS;
            return -1;
        }
        hero_panic("too many threads running at once — this runtime holds 256, "
                   "and a thread's slot comes back when it is joined");
    }
    hero_spawn_slots[at].body = body;
    hero_spawn_slots[at].arg = arg;
    hero_spawn_slots[at].result = 0;
    hero_spawn_slots[at].kind = kind;
    hero_spawn_slots[at].state = 1;
    hero_spawn_slots[at].runs = runs;
    hero_spawn_slots[at].from = from;
    hero_spawn_slots[at].generation += 1;
    hero_spawn_slots[at].place = NULL;
    hero_spawn_slots[at].holder = 0;
    hero_spawn_slots[at].answered = 0;
    hero_spawn_slots[at].ended = 0;
    hero_spawn_slots[at].want = bytes > hero_spawn_floor ? bytes : hero_spawn_floor;
    int refused = 0;
#if defined(_WIN32)
    /* A named size is a RESERVATION: without the flag `CreateThread` takes it
     * as the commit and keeps the executable's reserve, the defect panel
     * 115's historian found in libuv. Unsized, the executable's own reserve
     * (`/STACK:67108864`, `selfhost/cli/flags.hero`) is every thread's, as
     * the head of this file says. Unrun on Windows on the day it was written
     * (2026-10-03): the box was offline.
     *
     * NOTHING IS HELD OFF HERE, and defect 2496 has no window on this arm: a
     * signal never lands on a thread Windows did not choose for it. The C
     * library runs a `raise` handler on the thread that raised it, and Ctrl-C
     * and Ctrl-Break on a thread the system creates for them (Microsoft's
     * `signal` and HandlerRoutine pages, read by panel 246's ffi-pragmatist),
     * so no handler can run on a thread started here before its claim. */
    SIZE_T reserve = (SIZE_T)hero_spawn_slots[at].want;
    hero_spawn_slots[at].handle =
        CreateThread(NULL, reserve, hero_spawn_trampoline, &hero_spawn_slots[at],
                     reserve > 0 ? STACK_SIZE_PARAM_IS_A_RESERVATION : 0, NULL);
    if (hero_spawn_slots[at].handle == NULL) refused = (int)GetLastError();
#else
    /* The size, asked for only when the platform's own default is BELOW it, so
     * a machine that already gives a worker what `main` has is untouched and
     * `attr` is never built there (glibc, measured: 8,388,608 both ways).
     *
     * A size `pthread_attr_setstacksize` refuses is a REFUSAL here, and it was
     * not until panel 184: the attr kept its default, the thread started on
     * it, and only the check in `hero_spawn_enter` caught the difference, on
     * the new thread. Asked of the attr and refused before any thread exists,
     * the caller hears it from the call that asked. */
    pthread_attr_t attr;
    pthread_attr_t *ask = NULL;
    int asked = 0;
    size_t want = hero_spawn_slots[at].want;
    hero_spawn_slots[at].mask = before;

    if (want > 0 && pthread_attr_init(&attr) == 0) {
        size_t theirs = 0;
        asked = 1;

        if (pthread_attr_getstacksize(&attr, &theirs) == 0 && theirs < want) {
            size_t page = (size_t)sysconf(_SC_PAGESIZE);

            if (want > (size_t)-1 - page) {
                refused = EINVAL;
            } else {
                int set = pthread_attr_setstacksize(&attr, (want + page - 1) / page * page);

                if (set == 0) ask = &attr;
                else refused = set;
            }
        }
    }

    if (refused == 0) refused = hero_spawn_ready_made(&hero_spawn_slots[at]);
    if (refused == 0) {
        refused = pthread_create(&hero_spawn_slots[at].handle, ask, hero_spawn_trampoline,
                                 &hero_spawn_slots[at]);
        if (refused != 0) hero_spawn_ready_given_back(&hero_spawn_slots[at]);
    }
    if (asked) pthread_attr_destroy(&attr);
#endif

    if (refused != 0) {
        hero_spawn_slots[at].state = 0;
        HERO_SPAWN_DROP();
#if !defined(_WIN32)
        pthread_sigmask(SIG_SETMASK, &before, NULL);
#endif
        if (why != NULL) *why = refused;
        return -1;
    }
    HERO_SPAWN_DROP();
#if !defined(_WIN32)
    pthread_sigmask(SIG_SETMASK, &before, NULL);
    hero_spawn_await(&hero_spawn_slots[at]);
#endif
    return at;
}

int64_t hero_thread_spawn(HeroThreadBody body, int64_t arg) {
    int64_t why = 0;
    int64_t at = hero_spawn_start(body, arg, 0, HERO_SPAWN_I64, &why, (uintptr_t)body,
                                  (uintptr_t)__builtin_return_address(0));

    if (at < 0) hero_panic("the operating system refused to start a thread");
    return at;
}

/* THE STACK THE CALLER CHOOSES (panel 184 R5, ratified 2026-10-01). The
 * compiler runs every command on one of these, so how deep a source may nest
 * is the compiler's own number on every machine and never the shell's `ulimit
 * -s`: measured before it landed, the compiler could not check its own source
 * at `ulimit -s 512` and a sum of 300 terms stopped `check` at 134.
 *
 * The thread is never smaller than the floor above, and the size is reserved
 * address space rather than memory: a page is committed when it is first
 * touched, which is what each platform documents for a thread's stack. A
 * refusal is ANSWERED rather than panicked,
 * because the caller is the one that can say what it needed the thread for:
 * -1, `*why` the operating system's own number. */
int64_t hero_thread_spawn_sized(HeroThreadBody body, int64_t arg, int64_t bytes, int64_t *why) {
    if (bytes < 0) hero_panic("a thread was asked for a negative number of bytes of stack");
    return hero_spawn_start(body, arg, (size_t)bytes, HERO_SPAWN_I64, why, (uintptr_t)body,
                            (uintptr_t)__builtin_return_address(0));
}

/* Waiting is where the two undefined behaviours of C become named panics, and
 * a third that this runtime would otherwise commit itself: a thread joined by
 * the door of the other kind (`kind`, the door asking). The checks read the
 * slot under the lock and then wait OUTSIDE it, because joining while holding
 * the table would stop every other thread from starting one. */
static int64_t hero_spawn_join(int64_t handle, int kind, const HeroPlace *where) {
    if (handle < 0 || handle >= HERO_SPAWN_SLOTS) {
        hero_panic("a thread handle that this program never received was joined");
    }
    HERO_SPAWN_TAKE();
    int state = hero_spawn_slots[handle].state;
    int started = hero_spawn_slots[handle].kind;
    HERO_SPAWN_DROP();

    if (state == 0) {
        hero_panic("a thread was joined twice, or joined before it was started — "
                   "a handle is spent once the thread it names has been waited for");
    }
    if (started != kind) {
        hero_panic(started == HERO_SPAWN_STR
                       ? "a thread `hero_thread_spawn_str` started was joined by `hero_thread_join`, which would "
                         "hand its answer over as a number: join it with `hero_thread_join_str`, which gives its "
                         "`str` back"
                       : "a thread `hero_thread_spawn` started was joined by `hero_thread_join_str`, which would "
                         "read its `i64` answer as a `str`: join it with `hero_thread_join`");
    }
#if defined(_WIN32)
    WaitForSingleObject(hero_spawn_slots[handle].handle, INFINITE);
    CloseHandle(hero_spawn_slots[handle].handle);
#else
    pthread_join(hero_spawn_slots[handle].handle, NULL);
    hero_spawn_ready_given_back(&hero_spawn_slots[handle]);
#endif
    HERO_SPAWN_TAKE();
    int64_t out = hero_spawn_slots[handle].result;
    int answered = hero_spawn_slots[handle].answered;
    hero_spawn_slots[handle].state = 0;
    hero_spawn_slots[handle].body = NULL;
    hero_spawn_slots[handle].kind = HERO_SPAWN_I64;
    HERO_SPAWN_DROP();
    /* A THREAD C ENDED HAS NO ANSWER, and `wait` says so rather than hand on
     * the one nobody put (panel 258's R1, defect 2754): the specification
     * gives `wait` the thread's answer and says nothing of a thread that has
     * none, and this runtime's reading of that silence is to stop, naming it. */
    if (!answered) {
        hero_panic_where("a thread ended without its answer: C ended it (`pthread_exit`, a cancellation, "
                         "`ExitThread`) inside a function its own function called, before that function "
                         "returned, so waiting for it has no answer to give",
                         where);
    }
    return out;
}

int64_t hero_thread_join(int64_t handle) { return hero_spawn_join(handle, HERO_SPAWN_I64, NULL); }

/* How many a program may have running at once, so a Heroes program can ask
 * rather than learn it from a panic. A value, not a form — `hero_word_bits`'s
 * precedent (DESIGN-LOG 2026-08-26): where the machine knows something, ask C
 * for it through an ordinary declaration instead of writing a guess into the
 * language. */
int64_t hero_thread_limit(void) { return HERO_SPAWN_SLOTS; }

/* A THREAD NOBODY JOINS IS THE PROGRAM'S MISTAKE, TOLD WHEN `main` RETURNS
 * (panel 251's R2, ratified 2026-11-19; defect 2552; spec section 13, *a
 * thread nobody joins ... aborts when `main` returns, saying how many*).
 *
 * The end of `main` asks this table before it reads any balance
 * (`hero_runtime_check_leaks`, `parts/alloc.c`), because what a thread still
 * running holds is no missing decref: a worker holding one `str` when `main`
 * returned was told *"2 heap blocks still live at exit ... this is a compiler
 * bug"*, 20 runs of 20, and one holding no block, or a lease it was about to
 * end, was told nothing or a lease's words (the sitting's eleven programs).
 * A slot running (1) or finished and not yet joined (2) is told, both: a
 * finished thread's answer waits in its slot for a join that never came. So
 * the gate's premise, that every thread was joined before it reads the
 * counters (`parts/alloc.c`'s RELAXED paragraph), holds whenever a balance is
 * read at all.
 *
 * WHAT IS NOT COUNTED, said: the runtime's own threads (the poller, a
 * launch's watcher), which take no slot; and a program that ends by
 * `exit(code:)`, which ends every thread with the process and skips the gate
 * (`hero_os.h`'s `hero_exit`).
 *
 * WHAT IT SAYS: how many, how many of them still run, and for the first
 * `HERO_SPAWN_NAMED` by slot, the handle the program was given, whether it
 * runs, the function it was handed and the function that started it, each
 * named by `parts/stack.c`'s reader where the platform has one (POSIX); on
 * Windows the handle and the state alone. A thread a library starts for the
 * program is named by the library's own function, the door's caller. Then
 * `hero_abort`, as every report of the gate ends: C's `exit` would flush
 * stdout and wait on the lock a worker blocked in a `print` holds, which is
 * also why stdout is flushed here only where its lock can be had
 * (`hero_report_begin`, `parts/panic.c`). */
#define HERO_SPAWN_NAMED 8

#if !defined(_WIN32)
/* The function whose bytes hold `at`, as a stack walk tells one; nothing where
 * no symbol holds it. */
static void hero_spawn_say_function(const char *before, uintptr_t at) {
    const char *sym = at != 0 ? hero_stack_symbol(at) : NULL;
    if (sym == NULL) return;
    hero_stack_say(before);
    if (hero_stack_is_heroes(sym)) {
        hero_stack_say_heroes_name(sym);
    } else {
        hero_stack_say(sym);
    }
}
#endif

static void hero_spawn_check_joined(void) {
    int64_t handle[HERO_SPAWN_NAMED];
    int state[HERO_SPAWN_NAMED];
    uintptr_t runs[HERO_SPAWN_NAMED];
    uintptr_t from[HERO_SPAWN_NAMED];
    const HeroPlace *place[HERO_SPAWN_NAMED];
    int64_t left = 0;
    int64_t running = 0;

    HERO_SPAWN_TAKE();
    for (int64_t i = 0; i < HERO_SPAWN_SLOTS; i += 1) {
        const HeroSpawnSlot *slot = &hero_spawn_slots[i];

        if (slot->state == 0) continue;
        /* ENDED BY C WITHOUT ITS ANSWER (panel 258's R1, defect 2754): POSIX's
         * cleanup handler marked it; on Windows a thread the system says has
         * ended while its slot still reads running never reached the end its
         * function's return leads to, so `ExitThread` ended it. */
#if defined(_WIN32)
        int ended = slot->ended || (slot->state == 1 && WaitForSingleObject(slot->handle, 0) == WAIT_OBJECT_0);
#else
        int ended = slot->ended;
#endif
        if (left < HERO_SPAWN_NAMED) {
            handle[left] = i;
            state[left] = ended ? 3 : slot->state;
            runs[left] = slot->runs;
            from[left] = slot->from;
            place[left] = slot->place;
        }
        left += 1;
        if (slot->state == 1 && !ended) running += 1;
    }
    HERO_SPAWN_DROP();
    if (left == 0) return;

    hero_report_begin();
    hero_report_printf(
            "panic: %lld thread(s) never waited for when `main` returned, %lld of them still running — every thread a "
            "program starts is waited for before `main` returns, and this program is missing that many\n",
            (long long)left, (long long)running);
    for (int64_t k = 0; k < left && k < HERO_SPAWN_NAMED; k += 1) {
        hero_report_printf("  thread %lld, %s", (long long)handle[k],
                           state[k] == 1   ? "still running"
                           : state[k] == 3 ? "ended by C without its answer"
                                           : "finished");
        /* The language's `spawn` names its own line, which every platform
         * has; a door's thread is named by the stack walk below. */
        if (place[k] != NULL) {
            hero_report_printf(", started at %s:%lld:%lld, in %s\n", place[k]->path, (long long)place[k]->line,
                               (long long)place[k]->column, place[k]->name);
            continue;
        }
#if !defined(_WIN32)
        hero_spawn_say_function(" ", runs[k]);
        /* A return address names the instruction after the call; one byte
         * back is still the caller's, as the stack walk asks it. */
        hero_spawn_say_function(", started in ", from[k] != 0 ? from[k] - 1 : 0);
#else
        (void)runs[k];
        (void)from[k];
#endif
        hero_stderr_put("\n", 1);
    }
    if (left > HERO_SPAWN_NAMED) hero_report_printf("  and %lld more\n", (long long)(left - HERO_SPAWN_NAMED));
    hero_abort();
}
