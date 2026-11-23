/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigguard.c — no Heroes code runs inside a signal handler (panel 246's
 * R4, enforced by panel 247's R1, R2 and R9; defect 2494).
 *
 * WHAT IT IS FOR. A Heroes function handed to C may be called by C from inside
 * a signal: given to `signal` or to the program's own `sigaction`, or called by
 * a handler written in C. There it reaches C the signal may have interrupted
 * half way: this runtime's allocator (`parts/alloc.c` brackets it, panel 247's
 * D), the buffers lent to C (`parts/lend.c`), `snprintf` in every number
 * formatted, stdio, `exit`. On Darwin the C library then kills the process by
 * SIGKILL with nothing said (a recursive `os_unfair_lock` in `snprintf`'s
 * locale, 10 runs of 10 of panel 247's `clocale` and `wraplocale`); on Linux
 * the same program goes on, its stdio corrupted in silence (the sitting's
 * ffi-pragmatist, 50 million lines where 32 stood).
 *
 * WHERE THE QUESTION IS ASKED: the first line of every THUNK, the one door from
 * C into Heroes code (`selfhost/emit/callback_thunk.hero`), so a Heroes call by
 * name or through a value never asks it, and a program that hands C no
 * function has no thunk and no probe. Three questions, the cheapest first:
 *
 *   1. THE EXACT TEST. Is the thunk's own return address the delivery's? A
 *      signal's delivery calls a handler from one place in the system, Darwin's
 *      `_sigtramp`, Linux's `__kernel_rt_sigreturn` or glibc's `__restore_rt`,
 *      the same for a plain and an `SA_SIGINFO` handler, on any stack (315,846
 *      deliveries on Darwin arm64 and 2,001,938 on Linux arm64, one address
 *      each, never a call that was not a delivery: panel 247's ffi-pragmatist).
 *      It is learned once, below, and it needs no frame record and no list of
 *      the functions that install a handler: `signal`, `sigset`, `bsd_signal`,
 *      `sigvec`, a library's or the program's own `sigaction` alike.
 *   2. ONSTACK. Where the thunk's frame lies outside its thread's own stack,
 *      is the thread running on an alternate signal stack? Only a delivery
 *      puts it there (POSIX's `sigaltstack`, `SS_ONSTACK`), so a handler of
 *      C's on an alternate stack the program made, which no walk can climb out
 *      of, is answered by the kernel exactly (panel 247's `fown`).
 *   3. THE WALK. Was one of the HERO_SIGNAL_WALK frame records above the thunk
 *      entered by the delivery: a handler of C's that called the Heroes
 *      function, directly or through `exit`'s `atexit` list, `fork`'s
 *      `pthread_atfork` list or `qsort`? Bounded by the thread's own stack,
 *      read once per thread as it is claimed, never inside a signal.
 *
 * AND A FOURTH, ASKED WHERE A ROUTE STANDS (panel 251's R1, ratified
 * 2026-11-19; defect 2522): is this thread inside a handler of C's that the
 * runtime saw begin and has not seen end? On Linux this runtime wraps every
 * handler a program's C installs and keeps the wrappers' frames
 * (`parts/siginstall.c`); on Darwin it rewrites the pointers every image
 * outside the shared cache calls `sigaction` and `signal` through, and the
 * kernel enters every handler through its trampoline (`parts/rebind.c`). A
 * handler left by a jump is told from one jumped within by the record of the
 * jump's buffer, validated (`parts/sigjump.c` on Linux, `parts/rebind.c` on
 * Darwin): a jump to a buffer with no trusted record keeps the entry, so a
 * miss is a told refusal and never Heroes code inside a signal.
 *
 * WHAT IT DOES NOT SEE, said, and `parts/alloc.c`'s guard (D) stays beneath it
 * for that (panel 247's R2 and R3): where no route stands or a route's own
 * residue lies (defect 2522's rows: a handler installed by a raw system call,
 * by a `sigaction` a library reached by `dlvsym` or by a `dlsym` made inside
 * Darwin's shared cache, or by the cache on its own account), a handler whose
 * frames keep no record (a library built
 * `-fomit-frame-pointer`), one more than HERO_SIGNAL_WALK frames below the
 * Heroes function (glibc's `qsort` of 10,000 values), and Windows,
 * where a console's Ctrl-C runs on a thread Windows makes, which
 * `parts/thread.c`'s guard refuses, and a fault's handler runs on the faulting
 * thread. A thread C made, which nothing claimed, is refused here too, in that
 * guard's words and through the stop below, since a handler of C's may be
 * running on it (below, THE THUNK FINDS IT THROUGH A THREAD KEY).
 *
 * THE ONE STOP. Every refusal of a re-entrance by a signal, this file's, the
 * allocator's and the lend tracking's, ends through `hero_signal_stop`, which
 * is async-signal-safe from its first call to its last (spec § 6, *"An abort
 * ends the program at once, saying why"*): its words assembled from fixed parts
 * and a name copied byte by byte, one `write(2)`, no `snprintf`, no stdio, no
 * `abort()`; SIGABRT's default put back, raised, `_exit(134)` if that returns.
 * It is LATCHED ONCE PER PROCESS: two threads inside a refusal at once wrote
 * their words over each other (panel 247's critic, pid 74286), so the second
 * writes nothing and waits for the end the first is making. And every refusal
 * carries the one remedy (panel 247's R9), naming no C header: a header two
 * modules include holds two of its `static` flags, and the second module's
 * handler is then never read (panel 247's spec-warden, `swp6`). */

#include <signal.h>
#if defined(_WIN32)
#include <io.h>
#include <windows.h>
#else
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#endif

/* What to write instead, the last words of every refusal below: spec § 13's
 * own sentence, byte for byte (panel 247's R5), each door named with its
 * header and its Heroes signature, as panel 247's R9 asks, and no C header,
 * the trap a `static` flag in one sets for a second module. Each refusal's
 * own first words say which way the function was reached, run as a handler
 * or called from one, so the sentence needs no clause of its own for that.
 * **A copy, held to the spec's line by the compiler's test** (defect 2533,
 * `selfhost/emit/callback_thunk.hero`): batch 36 moved the spec's sentence
 * (cd693eec1) and this one stayed behind, telling every program refused
 * that Ctrl-C and SIGTERM alone make the stop answer, where
 * `hero_net_ask_stop` makes it too. */
#define HERO_SIGNAL_REMEDY                                                                                   \
    "No function of the program is a signal handler: after `hero_net_catch_stop() -> i64` (`hero_net.h`), " \
    "Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make `hero_net_stop_asked() -> i64` answer 1 "    \
    "and a waiting call of that layer answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) "  \
    "counts any other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call"

/* 0 until the first refusal takes the stop. */
static _Atomic int hero_signal_stopping = 0;

/* Every byte of `text` to stderr: by `write(2)` through `hero_stderr_put`
 * (`parts/panic.c`), which never waits for a reader that takes nothing
 * (defect 2606); by `_write` on Windows. */
static void hero_signal_say(const char *text) {
    size_t left = 0;
    while (text[left] != '\0') left++;
#if defined(_WIN32)
    while (left > 0) {
        int put = _write(2, text, (unsigned int)left);
        if (put <= 0) return;
        text += put;
        left -= (size_t)put;
    }
#else
    hero_stderr_put(text, left);
#endif
}

/* The stop. `parts` are written one after another, then a line feed: one
 * line, since the latch admits one writer, and no buffer, so a refusal made
 * on a small alternate signal stack the program gave takes none of it.
 *
 * Every signal is held off the calling thread before the latch is taken, so a
 * second signal cannot run a second refusal on the thread already making the
 * first, which would wait on the latch for ever; SIGABRT alone is let through
 * again to end the process. */
static _Noreturn void hero_signal_stop(const char *const *parts, size_t count) {
#if !defined(_WIN32)
    sigset_t all;
    sigfillset(&all);
    pthread_sigmask(SIG_BLOCK, &all, NULL);
#endif
    if (atomic_exchange_explicit(&hero_signal_stopping, 1, memory_order_acq_rel) != 0) {
        for (;;) {
#if defined(_WIN32)
            Sleep(INFINITE);
#else
            pause();
#endif
        }
    }
    hero_runtime_spoke = 1;
    for (size_t i = 0; i < count; i++) hero_signal_say(parts[i]);
    hero_signal_say("\n");
    signal(SIGABRT, SIG_DFL);
#if !defined(_WIN32)
    sigset_t abort_only;
    sigemptyset(&abort_only);
    sigaddset(&abort_only, SIGABRT);
    pthread_sigmask(SIG_UNBLOCK, &abort_only, NULL);
#endif
    raise(SIGABRT);
    _exit(134);
}

/* WHICH ROUTE STANDS (panel 251's R1): B' with J on Linux and every POSIX
 * system but Darwin, D-rebind with J on Darwin, none on Windows. */
#if defined(__APPLE__)
#define HERO_SIG_REBIND 1
#elif !defined(_WIN32)
#define HERO_SIG_WRAPS 1
#endif

#if !defined(_WIN32)

/* How many frame records above the thunk the walk reads. Eight reaches a
 * handler calling the function directly (one), through `exit` (three on both
 * POSIX legs), `fork` (three or four) and `qsort` of a few values (two to
 * five); glibc's `qsort` of many is deeper, and D is what stands there. */
#define HERO_SIGNAL_WALK 8

/* The delivery's return address, for a plain handler and an `SA_SIGINFO` one,
 * 0 until learned; and whether it is: 0 not yet, 1 being learned, 2 learned.
 * Where B' stands, [2] and [3] are its wrapper's two calls of a program's
 * handler, plain and `SA_SIGINFO`, which call it on the system's behalf: a
 * Heroes function the program itself installed is called from there, and is
 * told as the system's call, as it is where no wrapper stands. */
static _Atomic uintptr_t hero_signal_returns[4];
static _Atomic int hero_signal_state = 0;
/* Which pair the probe running now learns: 0 the delivery's, 2 the wrapper's. */
static _Atomic int hero_signal_probing = 0;

static void hero_signal_probe_plain(int sig) {
    (void)sig;
    int at = atomic_load_explicit(&hero_signal_probing, memory_order_relaxed);
    atomic_store_explicit(&hero_signal_returns[at], (uintptr_t)__builtin_return_address(0), memory_order_relaxed);
}

static void hero_signal_probe_info(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)info;
    (void)context;
    int at = atomic_load_explicit(&hero_signal_probing, memory_order_relaxed);
    atomic_store_explicit(&hero_signal_returns[at + 1], (uintptr_t)__builtin_return_address(0), memory_order_relaxed);
}

typedef int (*HeroSignalInstall)(int, const struct sigaction *, struct sigaction *);

#if defined(HERO_SIG_WRAPS)
/* `parts/siginstall.c`, below: the C library's own `sigaction`. */
static int hero_sig_real(int sig, const struct sigaction *act, struct sigaction *old);
#endif

/* One probe: SIGURG to `probe` through `install`, and the action the program
 * had put back the same way. A thread-directed `raise` is delivered before it
 * returns. The delivery's pair is learned through the C library's own
 * `sigaction`, never through B''s definition (a runtime that learned only its
 * own wrapper's address refused nothing: panel 247's critic); on Darwin the
 * call reaches D-rebind's rewritten slot, so the delivery learned is its
 * trampoline's, which enters every handler there. */
static void hero_signal_probe(struct sigaction *probe, HeroSignalInstall install) {
    struct sigaction before;
    sigemptyset(&probe->sa_mask);
    if (install(SIGURG, probe, &before) != 0) return;
    raise(SIGURG);
    install(SIGURG, &before, NULL);
}

/* Both kinds of handler probed through `install`, into the pair `at` names. */
static void hero_signal_probe_both(HeroSignalInstall install, int at) {
    atomic_store_explicit(&hero_signal_probing, at, memory_order_relaxed);
    struct sigaction probe;
    memset(&probe, 0, sizeof probe);
    probe.sa_handler = hero_signal_probe_plain;
    hero_signal_probe(&probe, install);
    memset(&probe, 0, sizeof probe);
    probe.sa_sigaction = hero_signal_probe_info;
    probe.sa_flags = SA_SIGINFO;
    hero_signal_probe(&probe, install);
}

/* LEARNED ONCE PER PROCESS, at the first crossing of a function into C (the
 * emitted `hero_callback_of`), on the thread making it, with SIGURG let
 * through there for the two probes and its mask put back: a program that hands
 * C no function never raises it. Before that crossing no thunk's address is
 * anywhere C can call it. SIGURG is ignored by default and passed on without a
 * stop by lldb (panel 247's compiler-engineer); one sent from outside during
 * the two probes is taken by them, and a program's own handler and an ignored
 * SIGURG stay as they were (the ffi-pragmatist's `urg.c`, 100 of 100). */
void hero_signal_learn(void) {
    if (atomic_load_explicit(&hero_signal_state, memory_order_acquire) == 2) return;
    int expect = 0;
    if (!atomic_compare_exchange_strong(&hero_signal_state, &expect, 1)) {
        while (atomic_load_explicit(&hero_signal_state, memory_order_acquire) != 2) sched_yield();
        return;
    }
    sigset_t urg;
    sigset_t was;
    sigemptyset(&urg);
    sigaddset(&urg, SIGURG);
    pthread_sigmask(SIG_UNBLOCK, &urg, &was);
#if defined(HERO_SIG_WRAPS)
    hero_signal_probe_both(hero_sig_real, 0);
    hero_signal_probe_both(sigaction, 2);
#else
    hero_signal_probe_both(sigaction, 0);
#endif
    pthread_sigmask(SIG_SETMASK, &was, NULL);
    /* A probe the system refused learns nothing, and its kind is asked by the
     * other's address, the same on every leg measured; two refused leave the
     * questions below unasked rather than matching a frame whose return is 0.
     * The plain one is published last, with a release, so a thunk on any
     * thread that reads it with an acquire reads both. */
    uintptr_t plain = atomic_load_explicit(&hero_signal_returns[0], memory_order_relaxed);
    uintptr_t info = atomic_load_explicit(&hero_signal_returns[1], memory_order_relaxed);
    atomic_store_explicit(&hero_signal_returns[1], info != 0 ? info : plain, memory_order_relaxed);
    atomic_store_explicit(&hero_signal_returns[0], plain != 0 ? plain : info, memory_order_release);
    atomic_store_explicit(&hero_signal_state, 2, memory_order_release);
}

/* THE THREAD'S OWN STACK, read once as the thread is claimed (`parts/thread.c`:
 * `main`'s from `hero_args_set`, a worker's from `hero_spawn_enter` while its
 * signals are still held off), never inside a signal: glibc's
 * `pthread_getattr_np` allocates. Asked of the system rather than of
 * `parts/stack.c`'s guard, which `--sanitize` empties and the walk was then
 * blind (panel 247's compiler-engineer).
 *
 * THE THUNK FINDS IT THROUGH A THREAD KEY, NEVER A THREAD-LOCAL ACCESS, because
 * on a thread C made, which nothing claimed, the first access of a
 * thread-local is made inside the signal, and Darwin makes a thread's
 * thread-locals at their first access with `malloc` (dyld's
 * `instantiateVariable`): a thread interrupted inside the allocator was then
 * killed by SIGKILL with nothing said, 9 crash reports of plain C on this Mac,
 * 2026-11-13 (`_os_unfair_lock_recursive_abort < mfm_alloc <
 * instantiateVariable < _tlv_get_addr < handler < _sigtramp < mfm_free`). The
 * claim stores the record's address under the key; `pthread_getspecific`
 * reads a slot of the thread's own and allocates nothing, so a thread whose
 * slot is empty is one nothing claimed, told by 2492's words, and no
 * thread-local of this runtime is touched there. One read of the slot, where
 * the record was one thread-local access (four of those cost a `qsort`
 * comparator 3.7 times D's instructions, the compiler-engineer). */
#if defined(HERO_SIG_WRAPS) || defined(HERO_SIG_REBIND)
/* How many of a route's handlers one thread may be inside at once; a deeper one
 * runs its handler without an entry (`parts/siginstall.c`, `parts/rebind.c`). */
#define HERO_SIG_DEPTH 8

/* How many save points' buffers J remembers per thread (`parts/sigjump.c`);
 * a full record overwrites its oldest pair. */
#define HERO_SIG_JUMPS 32
#endif

/* One handler of C's this thread is inside, on both routes (Darwin's trampoline,
 * `parts/rebind.c`; Linux's wrapper, `parts/siginstall.c`; panel 255's R1 and
 * R3, defects 2679 and 2702). Its frame, compared and never read through;
 * its serial, one per push and never reused, which a record of route A names;
 * the alternate stack it was pushed on (none where `alt_lo == alt_hi`); how
 * many swaps made while it stood have not yet returned in their own frames
 * (rule 5); whether a `setcontext` into a coroutine's stack holds it until it
 * ends (rule 6); `unsure`, 1 where a jump this runtime could not follow passed
 * over it and 2 where the entry is itself a re-entry (a jump landed in frames
 * the thread had left); and whether it is gone, one store, so a nested signal
 * never sees an entry half moved. */
typedef struct {
    uintptr_t at;
    uint64_t serial;
    uintptr_t alt_lo;
    uintptr_t alt_hi;
    _Atomic int suspended;
    unsigned gen; /* bumped where a landing resumes the handler, so a swap's late return releases nothing */
    int held;
    int unsure;
    int dead;
} HeroSigEntry;

#if defined(HERO_SIG_REBIND)
typedef HeroSigEntry HeroRebindEntry;
#endif

typedef struct {
    uintptr_t lo;
    uintptr_t hi;
#if defined(HERO_SIG_WRAPS)
    /* B': the handlers this thread is inside, innermost last, `depth` past the
     * last one not gone; how many run with no entry because the record was
     * full (each refuses until it ends); the last serial given; whether a
     * handler of this thread ever ran on an alternate stack (only then can a
     * jump land in a handler's frames from outside). */
    HeroSigEntry entries[HERO_SIG_DEPTH];
    int depth;
    int lost;
    uint64_t serial;
    int alt_seen;
    /* J, route A only: which buffer each save point filled, the serial of the
     * innermost entry standing at the save (0 for none), the buffer's bytes
     * as the C library left them (0 until read at this thread's next save
     * point or jump or at a handler's begin), and how many bytes it holds;
     * `jump_next` is the pair a full record overwrites next. */
    uintptr_t jump_buf[HERO_SIG_JUMPS];
    uint64_t jump_entry[HERO_SIG_JUMPS];
    uint64_t jump_print[HERO_SIG_JUMPS];
    uint32_t jump_size[HERO_SIG_JUMPS];
    unsigned jump_next;
#elif defined(HERO_SIG_REBIND)
    /* Darwin's route: the handlers this thread is inside, innermost
     * last, `depth` past the last one not gone; how many run with no entry
     * because the record was full (each refuses until it ends); the last
     * serial given; whether a handler of this thread ever ran on an alternate
     * stack (only then can a jump land in a handler's frames from outside). */
    HeroSigEntry entries[HERO_SIG_DEPTH];
    int depth;
    int lost;
    uint64_t serial;
    int alt_seen;
#endif
} HeroSignalStack;

static _Thread_local HeroSignalStack hero_signal_stack;

/* The fourth question's four answers: no handler stands; one stands that
 * this runtime saw begin and no jump since passed over; one stands that a
 * jump it could not follow passed over, which may have left it; or one stands
 * that switched to another context and has not been seen to switch back or
 * to end (both routes count the switches); or one stands that a jump landed
 * back into, in frames the thread had left (both routes push such an entry). */
#define HERO_SIG_OUTSIDE 0
#define HERO_SIG_INSIDE 1
#define HERO_SIG_UNSURE 2
#define HERO_SIG_SWITCHED 3
#define HERO_SIG_REENTERED 4

#if defined(HERO_SIG_WRAPS)
/* `parts/sigentries.c`, below: is this thread inside a wrapped handler? */
static int hero_sig_inside(HeroSignalStack *own);
#elif defined(HERO_SIG_REBIND)
/* `parts/rebind.c`, below: is this thread inside a trampolined handler? */
static int hero_rebind_inside(void);
#endif

/* The key, made once by the first claim, `main`'s, before any thunk exists;
 * and whether it is: 0 not yet, 1 being made, 2 made. */
static _Atomic pthread_key_t hero_signal_key;
static _Atomic int hero_signal_keyed = 0;

/* 2492's refusal (`parts/thread.c`), async-signal-safe. */
static _Noreturn void hero_thread_refuse(const char *what);

static void hero_signal_measure(void) {
#if defined(__APPLE__)
    pthread_t self = pthread_self();
    uintptr_t top = (uintptr_t)pthread_get_stackaddr_np(self);
    hero_signal_stack.lo = top - pthread_get_stacksize_np(self);
    hero_signal_stack.hi = top;
#else
    pthread_attr_t attr;
    void *addr = NULL;
    size_t size = 0;
    if (pthread_getattr_np(pthread_self(), &attr) == 0) {
        pthread_attr_getstack(&attr, &addr, &size);
        pthread_attr_destroy(&attr);
    }
    hero_signal_stack.lo = (uintptr_t)addr;
    hero_signal_stack.hi = (uintptr_t)addr + size;
#endif
    int expect = 0;
    if (atomic_compare_exchange_strong(&hero_signal_keyed, &expect, 1)) {
        pthread_key_t key;
        if (pthread_key_create(&key, NULL) != 0) hero_panic("the system refused this runtime a thread key");
        atomic_store_explicit(&hero_signal_key, key, memory_order_relaxed);
        atomic_store_explicit(&hero_signal_keyed, 2, memory_order_release);
    } else {
        while (atomic_load_explicit(&hero_signal_keyed, memory_order_acquire) != 2) sched_yield();
    }
    if (pthread_setspecific(atomic_load_explicit(&hero_signal_key, memory_order_relaxed), &hero_signal_stack) != 0) {
        hero_panic("the system refused this runtime a thread key's slot");
    }
}

/* How a refusal was reached, which its first words say. */
#define HERO_SIGNAL_CALLED 0    /* the delivery's own frame is live beneath it */
#define HERO_SIGNAL_DELIVERED 1 /* the system called it itself */
#define HERO_SIGNAL_UNFOLLOWED 2
#define HERO_SIGNAL_STANDING 3
#define HERO_SIGNAL_SWITCHED 4
#define HERO_SIGNAL_REENTERED 5

/* EVERY REFUSAL THE FOURTH QUESTION MAKES SAYS WHAT THE RUNTIME KNOWS, never
 * more (the author's ruling B of 2026-11-20 on defect 2522's rows 1 and 2).
 * The runtime saw the handler begin and has not seen it end; it cannot tell a
 * handler still running from one C left by a jump it sees no event for, and
 * running the function would then be a bet on memory, so the refusal stands,
 * a known cost of a correct program (robustness ranks first, CLAUDE.md §
 * Precedence), and its words are true of both.
 *
 * HERO_SIGNAL_STANDING: no jump since the handler began is known to have
 * passed over it. R1's words, *C called it while handling a signal*, were
 * false where C left the handler by a jump that is no call of the C
 * library's: the inline `__builtin_setjmp` pair at `-O0` on x86-64 refused a
 * correct jump out 20 runs of 20 in the emulated Linux x86-64 image
 * (lane-signals, 2026-11-20), and a jump made inside a system library or a
 * C++ exception leaves no event either. R1's words stay for the exact test,
 * ONSTACK and the walk, which find the delivery's own frame live beneath the
 * function, so are true.
 *
 * HERO_SIGNAL_SWITCHED: the handler switched to another context (`swapcontext`
 * or `setcontext` into a context `makecontext` made) and this runtime has not
 * seen it switch back or end (panel 255's R4, defect 2702). The function may
 * be called on that other context, inside the signal's own frames, or the
 * handler may have been abandoned by a switch the runtime cannot tell from
 * one that comes back: a handler swapped away from and never resumed, then
 * the function called from the context the program returned to, is correct C
 * and is refused, for the author's ruling R4. STANDING's words, *C left the
 * handler in a way this runtime does not follow*, were false of it: the
 * runtime followed the switch.
 *
 * HERO_SIGNAL_REENTERED: C jumped into frames the thread had already left, or
 * onto the alternate stack from outside it, where only a signal's delivery puts
 * frames (a handler left by a jump and jumped back into, panel 255's R3,
 * defect 2702): the runtime decoded the target and saw where it lands, so
 * UNFOLLOWED's words, *a buffer this runtime holds no record of*, were false of
 * it. The handler is held to be running again.
 *
 * HERO_SIGNAL_UNFOLLOWED: C jumped, while the handler ran, to a buffer this
 * runtime holds no trusted record of (panel 251's R1: such a jump keeps every
 * entry), so whether the jump left the handler or landed inside it is not
 * known. R1's words were false of a correct program that copied its buffer
 * with `memcpy`, jumped out of the handler through the copy and later called
 * the function: 40 runs of 40 at `-O0` and at `-O2` on this Mac and 20 of 20
 * at `-O0` in Linux arm64, 2026-11-20 (defect 2522's row 1, step 70d). And
 * *a buffer this runtime never saw filled*, the words of that day, was false
 * of a buffer it saw filled and that was refilled since, or whose record a
 * full table overwrote: the record is of the buffer as it now stands. */
static _Noreturn void hero_signal_refuse(const char *what, int how) {
    const char *const said[] = {
        "panic: ",
        what,
        how == HERO_SIGNAL_DELIVERED
            ? " ran as a signal handler: the system called it inside a signal, where neither this "
              "runtime's memory nor the C library's is safe to use. "
        : how == HERO_SIGNAL_STANDING
            ? " was called while a signal handler of C's that this runtime saw begin had not been seen "
              "to end: either C called it from inside that handler, or C left the handler in a way this "
              "runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a "
              "system library). A handler not seen to end is held to be running, where neither this "
              "runtime's memory nor the C library's is safe to use. "
        : how == HERO_SIGNAL_SWITCHED
            ? " was called while a signal handler of C's that this runtime saw begin had switched to "
              "another context (`swapcontext`, or `setcontext` into a context `makecontext` made) and "
              "had not been seen to switch back or to end: either C called it on that other context, or "
              "C abandoned the handler by a switch this runtime cannot tell from one that comes back. A "
              "handler not seen to end is held to be running, where neither this runtime's memory nor "
              "the C library's is safe to use. "
        : how == HERO_SIGNAL_REENTERED
            ? " was called after C jumped into stack frames the thread had already left (a handler left by a "
              "jump and jumped back into, or frames only a signal's delivery "
              "puts on the alternate stack): the handler is held to be running again, where neither this "
              "runtime's memory nor the C library's is safe to use. "
        : how == HERO_SIGNAL_UNFOLLOWED
            ? " was called after C, inside a signal handler, jumped to a buffer this runtime holds no "
              "record of as it now stands (copied with `memcpy`, refilled where the runtime does not "
              "look, or saved before more save points than it keeps), so whether the jump left the "
              "handler is not known: a handler not seen to end is held to be running, where neither "
              "this runtime's memory nor the C library's is safe to use. "
            : " was called from inside a signal handler: C called it while handling a signal, where "
              "neither this runtime's memory nor the C library's is safe to use. ",
        HERO_SIGNAL_REMEDY,
    };
    hero_signal_stop(said, sizeof said / sizeof said[0]);
}

/* The thunk's first line: returns where no signal's delivery is found, and
 * otherwise does not return. `ret` and `frame` are the thunk's own
 * `__builtin_return_address(0)` and `__builtin_frame_address(0)`. Each read
 * of the walk is a word between the thunk's frame and the top of its thread's
 * stack, above the stack pointer, so it reads memory the thread has used. */
void hero_callback_entered(const char *what, void *ret, void *frame) {
    uintptr_t plain = atomic_load_explicit(&hero_signal_returns[0], memory_order_acquire);
    uintptr_t info = atomic_load_explicit(&hero_signal_returns[1], memory_order_relaxed);
    uintptr_t wrapped = atomic_load_explicit(&hero_signal_returns[2], memory_order_relaxed);
    uintptr_t wrapped_info = atomic_load_explicit(&hero_signal_returns[3], memory_order_relaxed);
    uintptr_t back = (uintptr_t)ret;
    if (plain != 0 && (back == plain || back == info)) hero_signal_refuse(what, HERO_SIGNAL_DELIVERED);
    if (wrapped != 0 && (back == wrapped || back == wrapped_info)) hero_signal_refuse(what, HERO_SIGNAL_DELIVERED);
    HeroSignalStack *own = NULL;
    if (atomic_load_explicit(&hero_signal_keyed, memory_order_acquire) == 2) {
        own = pthread_getspecific(atomic_load_explicit(&hero_signal_key, memory_order_relaxed));
        if (own == NULL) hero_thread_refuse(what);
    }
#if defined(HERO_SIG_WRAPS)
    int inside = hero_sig_inside(own);
#elif defined(HERO_SIG_REBIND)
    int inside = hero_rebind_inside();
#else
    int inside = HERO_SIG_OUTSIDE;
#endif
    if (inside != HERO_SIG_OUTSIDE) {
        hero_signal_refuse(what, inside == HERO_SIG_UNSURE       ? HERO_SIGNAL_UNFOLLOWED
                                 : inside == HERO_SIG_SWITCHED   ? HERO_SIGNAL_SWITCHED
                                 : inside == HERO_SIG_REENTERED  ? HERO_SIGNAL_REENTERED
                                                                 : HERO_SIGNAL_STANDING);
    }
    if (plain == 0 || own == NULL) return;
    uintptr_t lo = own->lo;
    uintptr_t hi = own->hi;
    uintptr_t fp = (uintptr_t)frame;
    if (fp < lo || fp >= hi) {
        stack_t now;
        if (sigaltstack(NULL, &now) == 0 && (now.ss_flags & SS_ONSTACK) != 0) hero_signal_refuse(what, HERO_SIGNAL_CALLED);
        return;
    }
    /* A record is two words, so the highest one the walk may read starts at
     * `top`; each step climbs, so the walk ends inside the stack. */
    uintptr_t top = hi - 2 * sizeof(uintptr_t);
    for (int up = 0; up < HERO_SIGNAL_WALK; up += 1) {
        uintptr_t next = ((const uintptr_t *)fp)[0];
        if (next <= fp || next > top || (next & (sizeof(uintptr_t) - 1)) != 0) return;
        uintptr_t above = ((const uintptr_t *)next)[1];
        if (above == plain || above == info) hero_signal_refuse(what, HERO_SIGNAL_CALLED);
        if (wrapped != 0 && (above == wrapped || above == wrapped_info)) hero_signal_refuse(what, HERO_SIGNAL_CALLED);
        fp = next;
    }
}

#else

/* Windows: no probe and no walk (panel 247's R10). A console's Ctrl-C runs its
 * handler on a thread Windows makes, which `parts/thread.c`'s guard refuses,
 * and a fault's on the faulting thread, which D's guard stands under; whether
 * UCRT's `raise` leaves a delivery's address to learn is the box's question,
 * unrun. */
void hero_signal_learn(void) {}

void hero_callback_entered(const char *what, void *ret, void *frame) {
    (void)what;
    (void)ret;
    (void)frame;
}

static void hero_signal_measure(void) {}

#endif
