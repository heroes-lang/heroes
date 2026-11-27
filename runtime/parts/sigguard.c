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
    "and a waiting call of that layer, or a mailbox's `receive` that finds it empty, answer stopped; "      \
    "`hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any other signal, and "                      \
    "`hero_signal_take(sig: i32) -> i64` gives the count since its last call"

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

/* A JUMP OUT OF A HANDLER THAT INTERRUPTED THIS RUNTIME'S ALLOCATOR (panel
 * 258's R1, provisional, author ratification pending; defect 2755). A handler
 * of C's that `siglongjmp`s out of the code it interrupted (a watchdog's
 * `SIGALRM`) abandons that code at whatever instruction the signal found, and
 * where that was inside this runtime's allocator, or its record of the buffers
 * lent to C, the bracket the code had opened was never closed: the landing
 * code's next allocation found it open and stopped the program saying *a
 * Heroes function given to C as a signal handler*, of a program that has none
 * (`alarm`, 8 processes of 8 at exit 134, or killed by the C library's own
 * allocator with nothing said). POSIX leaves everything after such a jump
 * undefined, and the C library's allocator the code was inside holds its own
 * lock (the sitting's ffi-pragmatist: 10 processes of 10 die with no Heroes
 * code at all, on Darwin and Linux). So the jump hooks (`parts/sigjump_save.c`
 * on Linux, `parts/rebind_jump.c` on Darwin) ask here, after a jump has left
 * every handler the thread stood in, and the program stops before the jump,
 * saying what happened, from inside the handler: async-signal-safe, as every
 * refusal of this file. A jump the hooks never see leaves the bracket open,
 * and its next use says the words below with the re-entrance they name
 * (`parts/alloc.c`'s `hero_alloc_reentered`). */
#if !defined(_WIN32)
static int hero_alloc_interrupted(void); /* `parts/alloc.c` */
static int hero_lend_interrupted(void);  /* `parts/lend.c` */

static void hero_signal_jump_checked(void) {
    int alloc = hero_alloc_interrupted();
    if (!alloc && !hero_lend_interrupted()) return;
    static const char *const said[] = {
        "panic: a signal handler jumped out of the code it interrupted while that code was inside this runtime's ",
        NULL,
        ", which nothing will ever finish: what runs after such a jump is undefined, the allocator's lock and "
        "lists may be left half written, so the program stops here, before the jump. A handler that ends work by "
        "jumping (`siglongjmp`, `longjmp`) may leave only code it is safe to abandon, and this runtime's "
        "allocation is not: count the signal instead, with `hero_signal_watch(sig: i32) -> i64` and "
        "`hero_signal_take(sig: i32) -> i64` (`hero_os.h`), and let the function return",
    };
    const char *parts[3] = {said[0], alloc ? "allocator" : "record of the buffers lent to C", said[2]};
    hero_signal_stop(parts, 3);
}
#endif

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

/* THE LEARNED ADDRESSES ARE KEPT MASKED (defect d-lane-sig-family-3). The walk
 * below compares words of the thread's stack with the delivery's return
 * address, and the runtime's own copies of that address, the locals of the
 * learning and of every thunk's question spilled in frames that are gone,
 * lay in such words: a fibre a program's own assembly started with its frame
 * pointer at its stack's top led the walk onto two of them, and a correct
 * program with no signal handler was refused in a handler's words
 * (`fm_top`, panel 262's compiler-engineer, under `--sanitize` on this Mac).
 * So every learned address is stored, and every word compared with it read,
 * XORed with this mask, which no code address equals (its high bits are no
 * user address's on either architecture): a spilled copy holds the masked
 * value, which unmasks to none. The probe's own delivery is made on an
 * alternate stack of its own, cleared after, so its frame is not left on the
 * thread's stack either. */
#define HERO_SIGNAL_MASK ((uintptr_t)0xa5c3000000000000ull)

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
    atomic_store_explicit(&hero_signal_returns[at], (uintptr_t)__builtin_return_address(0) ^ HERO_SIGNAL_MASK,
                          memory_order_relaxed);
}

static void hero_signal_probe_info(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)info;
    (void)context;
    int at = atomic_load_explicit(&hero_signal_probing, memory_order_relaxed);
    atomic_store_explicit(&hero_signal_returns[at + 1], (uintptr_t)__builtin_return_address(0) ^ HERO_SIGNAL_MASK,
                          memory_order_relaxed);
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
static char hero_signal_probe_stack[(size_t)64 << 10] __attribute__((aligned(16)));

static void hero_signal_probe(struct sigaction *probe, HeroSignalInstall install) {
    struct sigaction before;
    sigemptyset(&probe->sa_mask);
    stack_t own = {0}, was = {0};
    own.ss_sp = hero_signal_probe_stack;
    own.ss_size = sizeof hero_signal_probe_stack;
    bool alt = sigaltstack(&own, &was) == 0;
    if (alt) probe->sa_flags |= SA_ONSTACK;
    if (install(SIGURG, probe, &before) == 0) {
        raise(SIGURG);
        install(SIGURG, &before, NULL);
    }
    if (alt) {
        sigaltstack((was.ss_flags & SS_DISABLE) != 0 ? &(stack_t){.ss_flags = SS_DISABLE} : &was, NULL);
        memset(hero_signal_probe_stack, 0, sizeof hero_signal_probe_stack);
    }
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

/* How many spans of frames of handlers left by a jump one thread keeps
 * (defect 2855); a full record merges two spans of one stack rather than
 * forget one (defect d-lane-sig-family-4). */
#define HERO_SIG_LEFT 16
#endif

/* One handler of C's this thread is inside, on both routes (Darwin's trampoline,
 * `parts/rebind.c`; Linux's wrapper, `parts/siginstall.c`; panel 255's R1 and
 * R3, defects 2679 and 2702). Its frame, compared and never read through;
 * its serial, one per push and never reused, which a record of route A names;
 * the alternate stack it was pushed on (none where `alt_lo == alt_hi`); how
 * many swaps made while it stood have not yet returned in their own frames
 * (rule 5); whether a `setcontext` into a coroutine's stack holds it until it
 * ends (rule 6); `unsure`, 1 where a jump this runtime could not follow passed
 * over it, 2 where the entry is itself a re-entry (a jump landed in frames
 * the thread had left), 3 where a jump onto a coroutine's stack passed over
 * it and did not leave it (defect 2705) and 4 the same onto a stack route A
 * holds a coroutine's with no switch into it seen (defect 2745), 5 where a jump
 * made inside its frames on a coroutine's stack landed off that stack while a
 * save point made inside them could still switch back (defect 2872); whether
 * such a save point was made (`saved`); and whether it is gone, one store, so
 * a nested signal never sees an entry half moved. */
typedef struct {
    uintptr_t at;
    uint64_t serial;
    uintptr_t alt_lo;
    uintptr_t alt_hi;
    _Atomic int suspended;
    unsigned gen; /* bumped where a landing resumes the handler, so a swap's late return releases nothing */
    int held;
    int unsure;
    int saved;
    int dead;
    /* Where the signal interrupted a switch of contexts this thread was
     * making, the contexts that switch saves into and loads (defect 2871);
     * `cut_to` 0 elsewhere. */
    uintptr_t cut_from;
    uintptr_t cut_to;
} HeroSigEntry;

/* A switch of contexts a wrapper of `swapcontext` or `setcontext` has begun
 * (defect 2871): the wrapper's frame, 0 for none; the stack it lies on; the
 * context it saves into (0 for `setcontext`) and the one it loads. */
typedef struct {
    uintptr_t frame;
    uintptr_t lo;
    uintptr_t hi;
    uintptr_t from;
    uintptr_t to;
} HeroSigSwitch;

#if defined(HERO_SIG_REBIND)
typedef HeroSigEntry HeroRebindEntry;
#endif

typedef struct {
    uintptr_t lo;
    uintptr_t hi;
#if defined(HERO_SIG_WRAPS) || defined(HERO_SIG_REBIND)
    /* The switch of contexts this thread has begun and not yet been seen to
     * end (defect 2871). */
    HeroSigSwitch switching;
#endif
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
    /* The frames of the handlers this thread left by a jump (defect 2855):
     * each a span [left_lo, left_hi) of the stack [left_slo, left_shi) it lay
     * on, shrunk as the thread is seen running there again; a landing inside
     * one is that handler re-entered (rule 6). */
    uintptr_t left_lo[HERO_SIG_LEFT];
    uintptr_t left_hi[HERO_SIG_LEFT];
    uintptr_t left_slo[HERO_SIG_LEFT];
    uintptr_t left_shi[HERO_SIG_LEFT];
    int left_n;
    /* Set once a span could be kept by no merge within its own stack: every
     * one of the record's spans on a stack of its own (defect
     * d-lane-sig-family-4); from then on a landing below the frame making the
     * jump, on that frame's stack, is held a re-entry too, as before 2855. */
    int left_lost;
    /* J, route A only: which buffer each save point filled, the serial of the
     * innermost entry standing at the save (0 for none), the buffer's bytes
     * as the C library left them (0 until read at this thread's next save
     * point or jump or at a handler's begin), how many bytes it holds, and a
     * frame just below the stack pointer the save wrote, on the stack it
     * wrote it on (defect 2745); `jump_next` is the pair a full record
     * overwrites next. */
    uintptr_t jump_buf[HERO_SIG_JUMPS];
    uint64_t jump_entry[HERO_SIG_JUMPS];
    uint64_t jump_print[HERO_SIG_JUMPS];
    uint32_t jump_size[HERO_SIG_JUMPS];
    uintptr_t jump_sp[HERO_SIG_JUMPS];
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
    /* The frames of the handlers this thread left by a jump (defect 2855):
     * each a span [left_lo, left_hi) of the stack [left_slo, left_shi) it lay
     * on, shrunk as the thread is seen running there again; a landing inside
     * one is that handler re-entered (rule 6). */
    uintptr_t left_lo[HERO_SIG_LEFT];
    uintptr_t left_hi[HERO_SIG_LEFT];
    uintptr_t left_slo[HERO_SIG_LEFT];
    uintptr_t left_shi[HERO_SIG_LEFT];
    int left_n;
    /* Set once a span could be kept by no merge within its own stack: every
     * one of the record's spans on a stack of its own (defect
     * d-lane-sig-family-4); from then on a landing below the frame making the
     * jump, on that frame's stack, is held a re-entry too, as before 2855. */
    int left_lost;
#endif
} HeroSignalStack;

static _Thread_local HeroSignalStack hero_signal_stack;

/* The fourth question's four answers: no handler stands; one stands that
 * this runtime saw begin and no jump since passed over; one stands that a
 * jump it could not follow passed over, which may have left it; or one stands
 * that switched to another context and has not been seen to switch back or
 * to end (both routes count the switches); or one stands that a jump landed
 * back into, in frames the thread had left (both routes push such an entry);
 * or one stands that a jump onto a coroutine's stack passed over, a stack a
 * switch entered (ONSTACK) or one route A holds with no switch seen (HELD); or
 * one a jump off the coroutine's stack it runs on left with a save point inside
 * its frames (PARKED). */
#define HERO_SIG_OUTSIDE 0
#define HERO_SIG_INSIDE 1
#define HERO_SIG_UNSURE 2
#define HERO_SIG_SWITCHED 3
#define HERO_SIG_REENTERED 4
#define HERO_SIG_ONSTACK 5
#define HERO_SIG_HELD 6
#define HERO_SIG_PARKED 7

#if defined(HERO_SIG_WRAPS) || defined(HERO_SIG_REBIND)
/* A HANDLER THAT SWITCHES CONTEXTS INSIDE A SWITCH IT INTERRUPTED (defect
 * 2871, 2026-11-27; panel 262's ffi-pragmatist, `gtc`). A switch loads the
 * target's signal mask before its registers, Darwin's `setcontext` and
 * glibc's `swapcontext` alike, so a signal pending while blocked is delivered
 * inside the switch, on the stack being left, as soon as the target's mask
 * lets it through. A green-thread scheduler whose `SIGALRM` handler
 * `swapcontext`s to a thread never run before, whose context `getcontext`
 * made with the signal let through, meets that whenever its next tick is
 * already due: the handler runs again there with the scheduler's current
 * thread already the new one, saves into that thread's context, the one the
 * interrupted switch has yet to load, and the switch, resumed later, loads
 * the frames of a handler that has returned. Measured: `gtc` with the tick
 * forced due there (a wait of 7 ms in its handler) died at 134 with nothing
 * said, 3 runs of 3 on this Mac, which lldb read in a fourth as
 * `__stack_chk_fail` at the return of the runtime's `swapcontext` on a green
 * thread's stack, the very end of the one failure of 640 runs under
 * `--sanitize` there (its crash report); and as C alone, with
 * no Heroes runtime, by Bus error or Segmentation fault, 6 of 6 at `-O0` and
 * `-O2`; in Linux arm64, 139 plain, 3 of 3, and as C alone 2,222 ticks
 * found on another thread's stack than the scheduler's current one when its
 * output was cut at 20,000 bytes, 3 of 3. Neither
 * function is async-signal-safe, and POSIX leaves a call of one by a handler
 * that interrupted one undefined.
 *
 * So each wrapper of `swapcontext` and `setcontext` notes the switch it
 * begins, its own frame, the stack that frame lies on and the two contexts,
 * until the switch is seen to end (`parts/rebind_jump.c`,
 * `parts/sigjump_save.c`); a delivery whose interrupted frame records reach
 * that frame within HERO_SIG_SWITCH_WALK records interrupted the switch
 * (each record read lies on that stack below the frame), and its entry keeps
 * the two contexts; and a `swapcontext` or `setcontext` its handler then
 * calls naming either of them stops the program before it switches, saying
 * why. A handler's switch naming other contexts runs, as it ran before: no
 * context the interrupted switch has yet to finish is touched, and both
 * libraries finish it when the handler returns
 * (`run/fixedbugs-2871-a-handler-swapping-other-contexts-inside-a-swapcontext-runs`,
 * which ran before this repair too). The end is seen where the switch returns
 * into a wrapper's frame, which puts back the switch it was made inside of,
 * or where a jump lands at or above the noted frame on its stack, or where
 * `makecontext` names that stack again; a switch to a context no wrapper
 * saved (a coroutine `makecontext` made, one `getcontext` saved) leaves the
 * note behind, and only a delivery whose own frame records reach that very
 * frame again reads it.
 *
 * What it does not reach, said: a jump out of such a handler, which panel
 * 258's refusal of the allocator's bracket sees and nothing here; a scheduler
 * whose first tick is due before its first switch begins (`gtc` arms its
 * timer and then switches, its current thread already the first), whose
 * handler interrupts the program's own code and saves the thread that is
 * running into another's context, a race of the program's own that C alone
 * dies of too (forced: Bus error on this Mac, SIGTRAP as C alone, 3 of 3); and
 * a handler installed where no route stands. */
#define HERO_SIG_SWITCH_WALK 8

/* A wrapper's own note of the switch it is made inside of, empty, in the
 * wrapper's frame. The wrappers carry no redzones (defect 2853: a switch may
 * never return, and redzones left behind would poison later frames), so
 * AddressSanitizer clears nothing of their frames on entry, and the shadow
 * there may still mark the redzones of instrumented frames a switch abandoned
 * (a handler that swapped away to a context its runner took above it leaves
 * every frame below that context poisoned). So the note's bytes are cleared in
 * the shadow before anything writes them: until then the C library's `memset`
 * of the note, which ASan checks, read a redzone of `gr_raise_deep`'s abandoned
 * frame as an overflow in `setcontext`, a correct program stopped at exit 1
 * (`run/fixedbugs-2702-a-handler-swapped-away-from-and-resumed-after-it-was-
 * left-form-2-on-the-own-stack-is-refused` at `--sanitize -O0` in Linux arm64,
 * lane sig-family-3, step 199p, once that case's padding was kept whole). */
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define HERO_SIG_ASAN 1
void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
#endif
#endif
static void hero_signal_switch_none(HeroSigSwitch *note) {
#if defined(HERO_SIG_ASAN)
    __asan_unpoison_memory_region(note, sizeof *note);
#endif
    note->frame = 0;
    note->lo = 0;
    note->hi = 0;
    note->from = 0;
    note->to = 0;
}

/* The switch `next` begins, the wrapper of `call` asking: refused where the
 * handler the thread runs now interrupted a switch and `next` names either
 * context of it, which the C library has yet to finish saving or loading;
 * else noted, the switch it was made inside of kept in `prev`. A switch
 * naming other contexts is no harm the runtime sees, and runs. */
static void hero_signal_switch_begin(HeroSignalStack *own, const char *call, const HeroSigSwitch *next,
                                     HeroSigSwitch *prev) {
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        const HeroSigEntry *e = &own->entries[i];
        if (e->dead) continue;
        if (e->cut_to == 0) break;
        bool named = (next->from != 0 && (next->from == e->cut_from || next->from == e->cut_to)) ||
                     next->to == e->cut_from || next->to == e->cut_to;
        if (!named) break;
        const char *const said[] = {
            "panic: a signal handler called `",
            call,
            "` naming a context that the code the signal interrupted was itself switching from or to, inside a "
            "`swapcontext` or `setcontext` of this thread that had not yet loaded the context it switches to: "
            "neither is async-signal-safe, and that switch, resumed, would save or load a context this one "
            "changes and resume frames that have returned, so the program stops here, before the switch. A "
            "handler that switches contexts may interrupt only code that is not switching: keep its signal "
            "blocked around every switch made outside a handler, and in the `uc_sigmask` of every context given "
            "to `makecontext` until that context runs",
        };
        hero_signal_stop(said, sizeof said / sizeof said[0]);
    }
    *prev = own->switching;
    own->switching.frame = 0;
    atomic_signal_fence(memory_order_seq_cst);
    own->switching.lo = next->lo;
    own->switching.hi = next->hi;
    own->switching.from = next->from;
    own->switching.to = next->to;
    atomic_signal_fence(memory_order_seq_cst);
    own->switching.frame = next->hi > next->lo && next->frame >= next->lo && next->frame < next->hi ? next->frame : 0;
}

/* The switch noted has ended, its wrapper's frame `frame` running again: the
 * switch it was made inside of, where that one's frame lies above it on its
 * stack (a handler's switch inside the switch it interrupted), is the one in
 * progress again; else none is. */
static void hero_signal_switch_back(HeroSignalStack *own, const HeroSigSwitch *prev, uintptr_t frame) {
    own->switching.frame = 0;
    if (prev->frame == 0 || frame < prev->lo || frame >= prev->frame) return;
    atomic_signal_fence(memory_order_seq_cst);
    own->switching.lo = prev->lo;
    own->switching.hi = prev->hi;
    own->switching.from = prev->from;
    own->switching.to = prev->to;
    atomic_signal_fence(memory_order_seq_cst);
    own->switching.frame = prev->frame;
}

/* A jump landing at `t`: at or above the noted frame on its stack, it leaves
 * the switch there for good. */
static void hero_signal_switch_left(HeroSignalStack *own, uintptr_t t) {
    if (own->switching.frame != 0 && t >= own->switching.frame && t < own->switching.hi) own->switching.frame = 0;
}

/* `makecontext` names `[lo, hi)` a coroutine's stack: a switch noted on it is
 * gone with it. */
static void hero_signal_switch_remade(HeroSignalStack *own, uintptr_t lo, uintptr_t hi) {
    if (own->switching.frame != 0 && lo < own->switching.hi && own->switching.lo < hi) own->switching.frame = 0;
}

/* Whether the code a delivery interrupted, its context `context`, is inside
 * the switch noted: its frame pointer is the noted frame, or links up to it
 * within HERO_SIG_SWITCH_WALK records, each read on the noted stack below the
 * noted frame and above the one before. The words read are other frames',
 * whose shadow AddressSanitizer may hold poisoned for their own locals, so
 * the walk is not instrumented: instrumented, it was told as an
 * `unknown-crash` inside the runtime under `--sanitize` at `-O2`, where a
 * handler resumes frames it left (`run/fixedbugs-2702-a-handler-swapped-away-
 * from-and-resumed-after-it-was-left-form-0-on-the-own-stack-is-refused` and
 * its form 2, this repair's first run of them). */
__attribute__((no_sanitize("address"))) static bool hero_signal_in_switch(const HeroSignalStack *own,
                                                                          void *context) {
    uintptr_t f = own->switching.frame;
    if (f == 0 || context == NULL) return false;
    uintptr_t pc = 0, fp = 0, sp = 0, lr = 0;
    hero_stack_regs(context, &pc, &fp, &sp, &lr);
    for (int i = 0; i < HERO_SIG_SWITCH_WALK; i += 1) {
        if (fp == f) return true;
        if (fp < own->switching.lo || fp > f || fp + 16 > own->switching.hi || (fp & 7) != 0) return false;
        uintptr_t next = *(const uintptr_t *)fp;
        if (next <= fp) return false;
        fp = next;
    }
    return false;
}

/* A delivery's entry, `mine` its serial, given the contexts of the switch
 * noted where the signal interrupted it (`cut`). */
static void hero_signal_mark_cut(HeroSignalStack *own, uint64_t mine, bool cut) {
    if (!cut || mine == 0) return;
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        if (own->entries[i].serial == mine) {
            own->entries[i].cut_from = own->switching.from;
            own->entries[i].cut_to = own->switching.to;
            return;
        }
    }
}
#endif

#if defined(HERO_SIG_WRAPS)
/* `parts/sigentries.c`, below: is this thread inside a wrapped handler? And
 * the top of the coroutine range a frame lies on (defect 2852). */
static int hero_sig_inside(HeroSignalStack *own);
static bool hero_sig_coroutine_top(uintptr_t frame, uintptr_t *top);
#elif defined(HERO_SIG_REBIND)
/* `parts/rebind.c`, below: is this thread inside a trampolined handler? */
static int hero_rebind_inside(void);
static bool hero_rebind_coroutine_top(uintptr_t frame, uintptr_t *top);
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
#define HERO_SIGNAL_ONSTACK 6
#define HERO_SIGNAL_HELD 7
#define HERO_SIGNAL_PARKED 8

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
 * it. The handler is held to be running again, and the function to be called
 * from inside it: spec § 13's sentence on a callback called after a jump says a
 * refusal says *the handler was not seen to end, or that C called it from
 * inside one* (panel 257's R1), and these words, which said neither, now keep
 * the second. `tests/harness/spec_refusals.hero` holds every arm of this
 * message but DELIVERED's to one of the two.
 *
 * HERO_SIGNAL_ONSTACK: C jumped (`longjmp`, `siglongjmp`, `_longjmp`) out of
 * the handler onto a stack a context switch had entered and none saw end, a
 * coroutine's that `makecontext` was given (panel 257's `as_0`, defect 2705):
 * the runtime followed the jump and judged it, and a jump onto another stack
 * proves no handler left where a save point was made inside the handler's
 * frames, since it cannot tell it from a jump onto frames the handler itself
 * switched to, through that save. With none made, nothing can switch back
 * into the handler's frames, and the jump leaves it (the mirror of defect
 * 2872's `ck`, lane sig-family-3): `as_0`, refused in these words until then,
 * runs. STANDING's words, *C left the handler in a way
 * this runtime does not follow (an inline `__builtin_longjmp`, a C++
 * exception, a jump inside a system library)*, named three causes the program
 * has none of.
 *
 * HERO_SIGNAL_HELD: the same jump onto a stack `makecontext` was given, where
 * this system's contexts fall to route A (defect 2745): the runtime cannot
 * read where a context switch lands, so it holds every such stack to be a
 * coroutine's for as long as it stands, and saw no switch into this one.
 * ONSTACK's words, *a stack a context switch had entered*, were false of a
 * stack no switch was seen to enter (`fixedbugs-2706`'s program whose stack
 * `makecontext` was given and never switched to, built with
 * `HERO_JMP_FORCE_UNCALIBRATED` 2 on this Mac, lane-d2745).
 *
 * HERO_SIGNAL_PARKED: C jumped out of a handler running on a coroutine's stack
 * (`makecontext`'s) to another stack, from inside the handler's frames, after
 * a save point (`setjmp`, `getcontext`, `swapcontext` and their kin) was made
 * inside those frames: the frames stay on the coroutine's stack and that save
 * can switch back into them, so whether the jump left the handler is not
 * known (defect 2872). With no such save the jump leaves the handler, its
 * frames kept as a span a landing back into reads as a re-entry (rule 6), and
 * the function runs: panel 262's critic's `ck`, a handler on a coroutine's
 * heap stack leaving by `siglongjmp` or `setcontext` to the runner's save,
 * was refused 3 runs of 3 in STANDING's words, *C left the handler in a way
 * this runtime does not follow (an inline `__builtin_longjmp`, a C++
 * exception, a jump inside a system library)*, three causes it had none of.
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
 * full table overwrote: the record is of the buffer as it now stands. And
 * since defect 2867 a record is read wherever the system lets the runtime
 * copy its bytes, so a buffer it could not read is the fourth cause the words
 * name: until then a buffer on the heap was never read, and a correct program
 * leaving its handler by a `setcontext` to its runner's heap context was
 * refused in words naming three causes it had none of. */
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
              "puts on the alternate stack): the handler is held to be running again, and the function to be "
              "called from inside it, where neither this runtime's memory nor the C library's is safe to use. "
        : how == HERO_SIGNAL_ONSTACK
            ? " was called after C, inside a signal handler, jumped (`longjmp`, `siglongjmp` or `_longjmp`) "
              "onto a stack a context switch had entered and this runtime has not seen end (a coroutine's, "
              "`makecontext`'s), where it cannot tell that jump from one onto frames the handler itself "
              "switched to, so whether the jump left the handler is not known: a handler not seen to end is "
              "held to be running, where neither this runtime's memory nor the C library's is safe to use. "
        : how == HERO_SIGNAL_HELD
            ? " was called after C, inside a signal handler, jumped (`longjmp`, `siglongjmp` or `_longjmp`) "
              "onto a stack `makecontext` was given, which this runtime holds may still be a coroutine's, "
              "since on this system it cannot read where a context switch lands or see a coroutine end, so "
              "whether the jump left the handler is not known: a handler not seen to end is held to be "
              "running, where neither this runtime's memory nor the C library's is safe to use. "
        : how == HERO_SIGNAL_PARKED
            ? " was called after C, inside a signal handler running on a coroutine's stack (`makecontext`'s), "
              "jumped off that stack, leaving the handler's frames on it with a save point made inside them "
              "(`setjmp`, `getcontext`, `swapcontext`) that can switch back into them, so the handler was not "
              "seen to end: a handler not seen to end is held to be running, where neither this runtime's "
              "memory nor the C library's is safe to use. "
        : how == HERO_SIGNAL_UNFOLLOWED
            ? " was called after C, inside a signal handler, jumped to a buffer this runtime holds no "
              "record of as it now stands (copied with `memcpy`, refilled where the runtime does not "
              "look, saved before more save points than it keeps, or in memory the system would not let "
              "it read), so whether the jump left the handler is not known: a handler not seen to end is "
              "held to be running, where neither this runtime's memory nor the C library's is safe to use. "
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
static void hero_callback_asked(const char *what, void *ret, void *frame);

/* The thunk's first line: the question, then the count (`parts/callback.c`):
 * the count is begun only for a call the question let through, which the
 * thunk's own last line then ends. Nothing is judged given back by the
 * thunk's address (panel 259's R1, `parts/frame.c`). */
void hero_callback_entered(const char *what, void *ret, void *frame) {
    hero_callback_asked(what, ret, frame);
    hero_callback_in();
}

static void hero_callback_asked(const char *what, void *ret, void *frame) {
    uintptr_t plain = atomic_load_explicit(&hero_signal_returns[0], memory_order_acquire);
    uintptr_t info = atomic_load_explicit(&hero_signal_returns[1], memory_order_relaxed);
    uintptr_t wrapped = atomic_load_explicit(&hero_signal_returns[2], memory_order_relaxed);
    uintptr_t wrapped_info = atomic_load_explicit(&hero_signal_returns[3], memory_order_relaxed);
    uintptr_t back = (uintptr_t)ret ^ HERO_SIGNAL_MASK;
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
                                 : inside == HERO_SIG_ONSTACK    ? HERO_SIGNAL_ONSTACK
                                 : inside == HERO_SIG_HELD       ? HERO_SIGNAL_HELD
                                 : inside == HERO_SIG_PARKED     ? HERO_SIGNAL_PARKED
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
     * `top`; each step climbs, so the walk ends inside the stack. And where
     * the thunk runs on a coroutine's stack `makecontext` handed out, carved
     * from the thread's own, the walk ends at that range's top (defect 2852):
     * the coroutine's first record is `makecontext`'s, none on glibc and the
     * range's very top on Darwin, so the walk climbed past it into words of
     * the thread's stack a call before had left, where one naming the
     * delivery refused a correct program in a handler's words it does not
     * have (`mj_carved` under `--sanitize` on this Mac). A frame of the
     * coroutine's lies inside its range; a delivery that called the function
     * there put its frame there too. */
    uintptr_t top = hi - 2 * sizeof(uintptr_t);
    uintptr_t coroutine = 0;
#if defined(HERO_SIG_WRAPS)
    bool on_coroutine = hero_sig_coroutine_top(fp, &coroutine);
#else
    bool on_coroutine = hero_rebind_coroutine_top(fp, &coroutine);
#endif
    if (on_coroutine && coroutine - 2 * sizeof(uintptr_t) < top) top = coroutine - 2 * sizeof(uintptr_t);
    /* And it looks for the kernel's own delivery alone (defect
     * d-lane-sig-family-3): a delivery a route makes, Linux's wrapper or
     * Darwin's trampoline, stands as an entry while its handler runs, and the
     * question above has already refused every call made while one stands, so
     * a word naming it here is a copy left in a frame that is gone. On Darwin
     * the route's trampoline is the only delivery learned, so no walk is
     * made. */
#if defined(HERO_SIG_REBIND)
    return;
#endif
    for (int up = 0; up < HERO_SIGNAL_WALK; up += 1) {
        uintptr_t next = ((const uintptr_t *)fp)[0];
        if (next <= fp || next > top || (next & (sizeof(uintptr_t) - 1)) != 0) return;
        uintptr_t above = ((const uintptr_t *)next)[1] ^ HERO_SIGNAL_MASK;
        if (above == plain || above == info) hero_signal_refuse(what, HERO_SIGNAL_CALLED);
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
    hero_callback_in();
}

static void hero_signal_measure(void) {}

#endif
