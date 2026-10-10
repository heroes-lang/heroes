/* parts/run.c — running a program by argument list, instead of by writing a
 * sentence for a shell to parse.
 *
 * WHAT THIS REPLACES. The compiler used to build a command LINE as a string and
 * hand it to `system()`, quoting every filename on the way with a hand-written
 * POSIX quoter. Panel 098 measured what that costs: the same argument
 * `a b 'c' $HOME` arrives as 14 bytes through an argument list and as 20 —
 * `a b c /Users/joseph`, three words, quotes eaten, the variable expanded —
 * through a shell with the quoter removed. The quoter was a parser defence
 * maintained by hand across 189 call sites. An argument list deletes the parser.
 *
 * WHY A BUILDER AND NOT ONE CALL. `hero_run(program, argv: [str])` cannot be
 * declared: `[str]` is `error[ffi_type]`, and packing the arguments into one
 * NUL-separated string is `error[unknown_escape]`. There is no single Heroes
 * value that can carry an argument list, so the list is pushed one word at a
 * time and this file holds it between the calls (panel 098 conditions 1 and 2).
 * The buffers are `hero_alloc`, which `parts/alloc.c` has counted since
 * 2026-08-30 — so a word that is never released is a panic at exit rather than
 * a leak nothing reports.
 *
 * "NEVER STARTED" IS ITS OWN ANSWER, AND THAT IS THE POINT. `system()` answers
 * 127 both for a program that does not exist and for one that ran and exited
 * 127, and a program killed by a signal decodes to `WEXITSTATUS = 0`, which
 * reads as success. That collision is why `heroes build` on a machine with no
 * clang used to say *"internal error: the runtime did not compile"* — the
 * compiler blaming itself for the machine. Here the child reports a failed
 * `execvp` through a close-on-exec pipe, so the parent knows the difference
 * without guessing, and a signal comes back as 128 + the signal.
 */

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#if defined(__linux__)
#include <sys/prctl.h>
#endif
#if defined(__APPLE__)
#include <libproc.h>
#include <pthread.h>
#endif
#endif

/* The argument list being built. Bounded rather than grown: the longest line
 * this compiler writes is a clang invocation, and overflow is a panic, not a
 * truncation. It said 256 words was "an order of magnitude above it" until
 * 2026-09-25, when the link line of the compiler itself, one object per
 * module, reached 233 words and the 234th module could not be added (found by
 * the landing review of M-agreed-retention step 11). A line is one word per
 * module plus the flags, and the kernel's own ceiling is megabytes, so the
 * bound is set where no program this compiler builds will meet it. */
#define HERO_RUN_MAX_ARGS 4096

static _Thread_local char *hero_run_words[HERO_RUN_MAX_ARGS + 1];
static _Thread_local int64_t hero_run_count = 0;

/* Every word every thread's list holds, so the exit check can tell a list the
 * program never gave back from a runtime call that kept what it borrowed
 * (defect 573): the words are scratch, and the scratch count alone said *this
 * is a runtime bug* of a program that left its last list standing. */
static _Atomic int64_t hero_run_words_held = 0;

static int64_t hero_run_words_held_at_exit(void) {
    return atomic_load(&hero_run_words_held);
}

/* Drop every word. Safe to call twice, and called before every build. */
void hero_run_reset(void) {
    for (int64_t i = 0; i < hero_run_count; i += 1) {
        hero_release(hero_run_words[i]);
        hero_run_words[i] = NULL;
        atomic_fetch_sub(&hero_run_words_held, 1);
    }
    hero_run_count = 0;
}

/* One word, copied. The copy is this file's, because the `str` it came from may
 * be gone before `hero_run_go` is called.
 *
 * A NUL inside the word is a panic rather than a truncation: a `str`'s `len` is
 * authoritative (spec § 3 Types) and `execvp` reads to the first NUL, so the two
 * disagree exactly when an argument would silently become a shorter one. */
void hero_run_arg(HeroStr word) {
    if (hero_run_count >= HERO_RUN_MAX_ARGS) {
        hero_panic("a command line reached 4096 arguments");
    }
    size_t length = (size_t)hero_str_len(word);
    if (strlen(word.ptr) != length) {
        hero_panic("an argument contains a NUL byte");
    }
    char *copy = hero_alloc(length + 1);
    memcpy(copy, word.ptr, length);
    copy[length] = '\0';
    hero_run_words[hero_run_count] = copy;
    hero_run_count += 1;
    atomic_fetch_add(&hero_run_words_held, 1);
}

/* Where a stream goes when the caller wants it thrown away. The word is spelled
 * here and nowhere else: `selfhost/` never writes `/dev/null`, which is a
 * platform word panel 097 keeps out of the compiler (condition 7). A caller
 * that wants to discard asks `hero_run_discard_path()` for the local spelling
 * and passes it back as an ordinary path.
 *
 * **An empty path means INHERIT, and that is the safer default of the two.**
 * `heroes run` hands the program the terminal it was called from: its output is
 * the point of the command, and a stream that vanishes by default would be a
 * silence nobody asked for. Discarding is the exception and says so at the call
 * site. */
#if defined(_WIN32)
#define HERO_RUN_DISCARD "NUL"
#else
#define HERO_RUN_DISCARD "/dev/null"
#endif

/* What an executable is called on this machine: `.exe` on Windows, nothing
 * elsewhere.
 *
 * **This is not cosmetic and it is not the linker's habit.** clang honours `-o`
 * exactly, so a binary named `prog` is a file named `prog` on every platform.
 * But `CreateProcess` with a NULL application name — which is what makes a bare
 * `clang` resolve against PATH — runs the documented search, and that search
 * appends the executable extensions rather than trying the bare name. So a
 * binary with no extension is built successfully and then cannot be started:
 * `error: cannot execute build/405f1a3d19a003e7/00first`, measured on the CI's
 * Windows leg.
 *
 * The driver asks this and names its output accordingly, which is
 * issues/2026-08/14/2026-08-14-0005-is-not-a-fact-about-the-program-so-the-driver.md's rule — a fact about the machine, measured on the machine,
 * rather than a word the compiler has to know. */
HeroStr hero_run_exe_suffix(void) {
#if defined(_WIN32)
    return hero_str_from_bytes(".exe", 4);
#else
    return hero_str_from_bytes("", 0);
#endif
}

int64_t hero_os_pid(void) {
#if defined(_WIN32)
    return (int64_t)GetCurrentProcessId();
#else
    return (int64_t)getpid();
#endif
}

HeroStr hero_run_discard_path(void) {
    return hero_str_from_bytes(HERO_RUN_DISCARD, (int64_t)strlen(HERO_RUN_DISCARD));
}

static int hero_run_inherits(const char *path) {
    return path == NULL || path[0] == '\0';
}

#if defined(_WIN32)
/* THE QUOTING DOES NOT DISAPPEAR ON WINDOWS, IT MOVES HERE.
 *
 * `CreateProcess` takes one string and the child's C runtime parses it back
 * into words, so an argument list has to be re-flattened. The rules are the
 * CRT's, and they are not the shell's: a word is wrapped in double quotes when
 * it holds a space, a tab or a quote; a `"` inside becomes `\"`; and every
 * backslash immediately before a quote — the closing one included — is doubled,
 * which is why `C:\dir\` at the end of a quoted word needs `C:\dir\\`.
 *
 * This function replaces one that had a test (`cli_shell.hero`'s `sq`) AND HAS
 * NONE OF ITS OWN. This comment claimed a golden that was never written — `git
 * log --diff-filter=A` finds zero commits adding it — and it said so from the
 * day the function landed until 2026-09-06, when the citation check learned to
 * read the compiler's own comments and reported it. The round trip is owed and
 * is in `docs/work/SCHEDULED.md (retired 2026-09-12)`.
 * **NOT VERIFIED on a real Windows CRT** — the round trip is checked against
 * this project's own parser on the platforms it can run on, and the first green
 * Windows tag run is what closes that gap. */
static void hero_run_win_quote(const char *word, char *out, size_t *at) {
    size_t needs_quotes = 0;
    for (const char *p = word; *p != '\0'; p += 1) {
        if (*p == ' ' || *p == '\t' || *p == '"') needs_quotes = 1;
    }
    if (word[0] == '\0') needs_quotes = 1;

    if (!needs_quotes) {
        for (const char *p = word; *p != '\0'; p += 1) out[(*at)++] = *p;
        return;
    }

    out[(*at)++] = '"';
    for (const char *p = word; *p != '\0'; ) {
        size_t slashes = 0;
        while (*p == '\\') { slashes += 1; p += 1; }
        if (*p == '\0') {
            /* Trailing backslashes precede the closing quote: double them, or
             * the CRT reads the quote as escaped and the word never ends. */
            for (size_t i = 0; i < slashes * 2; i += 1) out[(*at)++] = '\\';
            break;
        }
        if (*p == '"') {
            for (size_t i = 0; i < slashes * 2 + 1; i += 1) out[(*at)++] = '\\';
            out[(*at)++] = '"';
        } else {
            for (size_t i = 0; i < slashes; i += 1) out[(*at)++] = '\\';
            out[(*at)++] = *p;
        }
        p += 1;
    }
    out[(*at)++] = '"';
}

/* Every pushed word, flattened into the one string CreateProcess wants. Owned,
 * and the caller decrefs it. Worst case per word is 2n+3 bytes — every byte a
 * backslash before a quote, plus the wrapping quotes and the separator. */
static HeroStr hero_run_win_command_line(void) {
    size_t room = 1;
    for (int64_t i = 0; i < hero_run_count; i += 1) {
        room += strlen(hero_run_words[i]) * 2 + 3;
    }
    char *line = hero_alloc(room);
    size_t at = 0;
    for (int64_t i = 0; i < hero_run_count; i += 1) {
        if (i > 0) line[at++] = ' ';
        hero_run_win_quote(hero_run_words[i], line, &at);
    }
    line[at] = '\0';
    HeroStr built = hero_str_from_bytes(line, (int64_t)at);
    hero_release(line);
    return built;
}
#endif

#if !defined(_WIN32)
/* **WHEN THIS PROCESS IS ENDED, THE CHILDREN IT WAITS ON END FIRST** (defect
 * 425, 2026-10-07).
 *
 * Every child gets a process group of its own (below), which is what lets the
 * terminal's Ctrl-C reach the program in front and the sweep reach what it
 * left behind. Its price was that a signal ending THIS process reached no
 * child: `timeout 3 heroes run forever.hero` exited 124 and the program ran on
 * with parent 1, and so did every program under a `heroes` the harness's
 * watchdog ended. On 2026-10-06 one of them wrote 146 GB in 44 minutes and
 * filled the disk every lane works on.
 *
 * So while a child is waited on, SIGTERM, SIGINT, SIGHUP and SIGQUIT are
 * caught where this process had left them at their default, and the handler
 * does what a shell does for its jobs: the same signal to every child's group,
 * `HERO_RUN_GRACE_MS` for them to end, SIGKILL for any group still standing,
 * the terminal handed back where a child held it, and then this process dies
 * of the signal itself, its default restored, so whoever waits on it reads
 * the status that signal means. A signal this process ignores stays ignored,
 * and one a C library already handles stays its own: neither is caught.
 *
 * **Everything the handler touches is async-signal-safe**: `kill`, `waitid`,
 * `select` as a sleep, `sigaction`, `tcsetpgrp`, and lock-free atomics
 * (C11 7.14.1.1p5). The children it may signal are a table of pids rather
 * than a list, so it allocates nothing and takes no lock.
 *
 * **Two races are closed by construction rather than by timing.**
 *
 * - A launch in flight: between its look at `hero_run_ending` and its pid in
 *   the table, a thread counts itself in `hero_run_launching` with the four
 *   signals blocked, so the handler never runs on it there; the handler,
 *   having claimed `hero_run_ending` first, waits for that count to fall
 *   before it reads the table. Either the launch saw the ending and started
 *   nothing, or its child is in the table when the handler reads it.
 * - A pid reused: a waiter leaves the table before it reaps, and once the
 *   process is ending it does not reap at all (`hero_run_unless_ending`), so a
 *   pid the handler read is still this process's unreaped child, its group's
 *   number pinned, whatever it signals.
 *
 * A child is given its dispositions back before exec, and the mask the
 * launch blocked, so a program never starts with a signal held.
 *
 * SIGKILL is not among them, and no process can catch it. On Linux the child
 * asks before exec to be sent SIGKILL when the thread that started it ends
 * (defect 437, `hero_run_child`), so a `heroes` killed with it takes its child
 * along; the Windows arm's job object is closed with the last handle a killed
 * process held, which is the same end by another road (its design, not run
 * against a killed caller here). Darwin has no such request; there a
 * process apart, the runner's SENTINEL, holds the read end of a pipe whose
 * write ends are the runner's alone, and ends every child still named down it
 * when that pipe reads its end, whatever ended the runner (defect 465, panel
 * 200's R2, below `hero_run_let_go`). */

/* How long a child forwarded the signal ending this process has to end before
 * SIGKILL. */
#define HERO_RUN_GRACE_MS 2000

/* How long a child the watchdog asked to end with SIGTERM has before SIGKILL:
 * longer than `HERO_RUN_GRACE_MS`, so a `heroes` the watchdog ends has ended
 * its own child before it is killed. */
#define HERO_RUN_WATCHDOG_GRACE_MS 5000

/* Children waited on at once: one a thread, and `parts/spawn.c` starts 256. */
#define HERO_RUN_LIVE 1024

/* The pid of each child a `hero_run_go` is waiting on, 0 for a free slot. */
static _Atomic int hero_run_live[HERO_RUN_LIVE];

/* Launches between their look at `hero_run_ending` and their pid's slot. */
static _Atomic int hero_run_launching = 0;

/* The signal ending this process, 0 while none is. */
static _Atomic int hero_run_ending = 0;

/* The group to hand the terminal back to while a child holds it, 0 when none
 * does. */
static _Atomic int hero_run_tty_back = 0;

/* Bits 0 to 3, which of the four this file caught; `HERO_RUN_CAUGHT` once they
 * are installed, `HERO_RUN_CATCHING` while one thread installs them. */
static _Atomic int hero_run_guard = 0;
#define HERO_RUN_CATCHING 256
#define HERO_RUN_CAUGHT 512

/* The four signals that end a process and are its to answer, by index. */
static int hero_run_ending_signal(int at) {
    switch (at) {
        case 0: return SIGTERM;
        case 1: return SIGINT;
        case 2: return SIGHUP;
        default: return SIGQUIT;
    }
}

static void hero_run_ending_set(sigset_t *set) {
    sigemptyset(set);
    for (int at = 0; at < 4; at += 1) sigaddset(set, hero_run_ending_signal(at));
}

/* A sleep a handler may take. */
static void hero_run_nap(int ms) {
    struct timeval wait;
    wait.tv_sec = ms / 1000;
    wait.tv_usec = (ms % 1000) * 1000;
    (void)select(0, NULL, NULL, NULL, &wait);
}

static void hero_run_tty_give(pid_t group);

/* Whether the child `pid` is still running: an exit not yet reaped answers no,
 * and leaves it to be reaped. */
static int hero_run_standing(int pid) {
    siginfo_t seen;
    memset(&seen, 0, sizeof seen);
    if (waitid(P_PID, (id_t)pid, &seen, WEXITED | WNOHANG | WNOWAIT) != 0) return 0;
    return seen.si_pid == 0;
}

static void hero_run_ended(int sig) {
    int none = 0;
    if (!atomic_compare_exchange_strong(&hero_run_ending, &none, sig)) return;

    for (int i = 0; i < 2000 && atomic_load(&hero_run_launching) > 0; i += 1) hero_run_nap(1);

    /* SIGCONT after it, as a shell's `kill` does for a stopped job: a child
     * stopped at the terminal (defect 436) acts on the signal now rather than
     * at the SIGKILL. */
    for (int i = 0; i < HERO_RUN_LIVE; i += 1) {
        int pid = atomic_load(&hero_run_live[i]);
        if (pid > 0) {
            kill(-(pid_t)pid, sig);
            kill(-(pid_t)pid, SIGCONT);
        }
    }
    for (int waited = 0; waited < HERO_RUN_GRACE_MS; waited += 10) {
        int standing = 0;
        for (int i = 0; i < HERO_RUN_LIVE; i += 1) {
            int pid = atomic_load(&hero_run_live[i]);
            if (pid > 0 && hero_run_standing(pid)) standing = 1;
        }
        if (!standing) break;
        hero_run_nap(10);
    }
    for (int i = 0; i < HERO_RUN_LIVE; i += 1) {
        int pid = atomic_load(&hero_run_live[i]);
        if (pid > 0) kill(-(pid_t)pid, SIGKILL);
    }

    int back = atomic_load(&hero_run_tty_back);
    if (back > 0) hero_run_tty_give((pid_t)back);

    struct sigaction plain;
    memset(&plain, 0, sizeof plain);
    plain.sa_handler = SIG_DFL;
    sigemptyset(&plain.sa_mask);
    sigaction(sig, &plain, NULL);
    kill(getpid(), sig);
}

/* Catch the four where this process left them at their default, once for the
 * process: a disposition is the process's, not a thread's. */
static void hero_run_catch_endings(void) {
    int state = atomic_load(&hero_run_guard);
    if (state & HERO_RUN_CAUGHT) return;
    int clear = 0;
    if (!atomic_compare_exchange_strong(&hero_run_guard, &clear, HERO_RUN_CATCHING)) {
        while (!(atomic_load(&hero_run_guard) & HERO_RUN_CAUGHT)) hero_run_nap(1);
        return;
    }
    sigset_t all;
    hero_run_ending_set(&all);
    int caught = 0;
    for (int at = 0; at < 4; at += 1) {
        int sig = hero_run_ending_signal(at);
        struct sigaction before;
        if (sigaction(sig, NULL, &before) != 0) continue;
        if ((before.sa_flags & SA_SIGINFO) || before.sa_handler != SIG_DFL) continue;
        struct sigaction ours;
        memset(&ours, 0, sizeof ours);
        ours.sa_handler = hero_run_ended;
        ours.sa_mask = all;
        if (sigaction(sig, &ours, NULL) == 0) caught |= 1 << at;
    }
    atomic_store(&hero_run_guard, HERO_RUN_CAUGHT | caught);
}

/* In the child, before exec: the four this file caught back at their default,
 * and the mask as it was before the launch blocked them. */
static void hero_run_child_signals(const sigset_t *before) {
    int caught = atomic_load(&hero_run_guard);
    for (int at = 0; at < 4; at += 1) {
        if (!(caught & (1 << at))) continue;
        struct sigaction plain;
        memset(&plain, 0, sizeof plain);
        plain.sa_handler = SIG_DFL;
        sigemptyset(&plain.sa_mask);
        sigaction(hero_run_ending_signal(at), &plain, NULL);
    }
    sigprocmask(SIG_SETMASK, before, NULL);
}

/* The child's slot in the table, or -1 where every slot is taken. */
static int hero_run_hold(pid_t child) {
    for (int i = 0; i < HERO_RUN_LIVE; i += 1) {
        int free_slot = 0;
        if (atomic_compare_exchange_strong(&hero_run_live[i], &free_slot, (int)child)) return i;
    }
    return -1;
}

#if defined(__APPLE__)
static void hero_run_sentinel_say(int32_t op, int32_t pid);
#define HERO_SENTINEL_HOLD 1
#define HERO_SENTINEL_FREE 2
#endif

/* Out of the table, before it is reaped: unnamed to the sentinel first, so the
 * pid it held is never one a later process can be given. */
static void hero_run_let_go(int slot) {
    if (slot < 0) return;
#if defined(__APPLE__)
    int pid = atomic_load(&hero_run_live[slot]);
    if (pid > 0) hero_run_sentinel_say(HERO_SENTINEL_FREE, (int32_t)pid);
#endif
    atomic_store(&hero_run_live[slot], 0);
}

#if defined(__APPLE__)
/* **THE SENTINEL, DARWIN'S ANSWER TO PR_SET_PDEATHSIG** (defect 465; panel 200's
 * R2, ratified 2026-10-09, the ffi-pragmatist's route A with every item the
 * sitting's critic owed it). Before it, 0 of 5 programs ended when the
 * `heroes` that ran them was killed with SIGKILL: each was adopted by launchd
 * and ran on.
 *
 * A process forked from the runner at its first launch holds the read end of a
 * pipe; the write end is the runner's, close-on-exec, so a child holds a copy
 * only between its fork and its exec. Each child names itself down the pipe
 * before it asks after its parent, the runner names it again once its pid is
 * in the table and unnames it before it is reaped, and when the pipe reads its
 * end the runner is gone, by whatever signal, and the sentinel ends every child
 * still named, its group first, and exits. A name given twice is held once.
 *
 * **WHAT THE CRITIC OWED IT**, each built here:
 * - the write end is `F_SETNOSIGPIPE` and `O_NONBLOCK`, so a sentinel killed,
 *   stopped or behind never ends its runner (SIGPIPE killed it at 141 in the
 *   sitting) nor holds it or a child before exec: a word that cannot be
 *   written is a sentinel gone, and the runner RESTARTS one, under a lock,
 *   ending the old one first so that its pipe's end ends nothing, and names
 *   every child of its table to the new one;
 * - the runner asks before each launch whether its sentinel still stands, so
 *   a sentinel killed between two launches is replaced before the next child;
 * - a child reaped past the table's 1,024 is unnamed before it is reaped, as
 *   every other is (`hero_run_let_go`);
 * - Darwin alone, `defined(__APPLE__)`: `libproc.h` is Darwin's.
 *
 * **WHAT THE SENTINEL CALLS** is what a fork of a process with threads may: it
 * allocates nothing and touches no lock, and calls `setpgid`, `sigaction`,
 * `sigprocmask`, `close`, `read`, `kill`, `_exit` and `proc_pidinfo`, which on
 * this Mac (arm64, 2026-10-10) is a wrapper of syscall 336 and nothing else,
 * `__proc_info` read by lldb's disassembly. It closes every descriptor but its
 * own read end from the kernel's list, a call at a time until none is left,
 * because a write end of a launch's report pipe held by another thread would
 * keep that launch waiting for an end it never reads, and `ulimit -n` read
 * 1,048,576 in the shell the sitting measured.
 *
 * **THE PREMISES, written as such**: a process this runtime does not start,
 * forked by a program through C without an exec, holds a copy of the write end
 * and keeps the sentinel waiting while it lives; and a held child's pid reused
 * between launchd's reap and the sentinel's kill, after the runner has gone,
 * would be ended as well, a window of the time the sentinel takes to read one
 * end of file, which no check of a pid alone closes. */
typedef struct { int32_t op; int32_t pid; } HeroSentinelWord;

/* The write end, -1 while there is none: read by a child after its fork, so
 * written only under the lock and before or after a close, never between. */
static _Atomic int hero_run_sentinel_fd = -1;
/* The sentinel's pid, and whether a word to it has failed since it was
 * started: both the lock's own state, read and written under it alone. */
static pid_t hero_run_sentinel_pid = 0;
static int hero_run_sentinel_failed = 0;
static pthread_mutex_t hero_run_sentinel_lock = PTHREAD_MUTEX_INITIALIZER;

/* One word down `fd`, whole or not at all: eight bytes, under PIPE_BUF, so no
 * other thread's word is ever interleaved with it. 0, or -1 where the
 * sentinel cannot take it, EPIPE or EAGAIN, never SIGPIPE. */
static int hero_run_sentinel_put(int fd, int32_t op, int32_t pid) {
    HeroSentinelWord w;
    w.op = op;
    w.pid = pid;
    for (;;) {
        ssize_t n = write(fd, &w, sizeof w);
        if (n == (ssize_t)sizeof w) return 0;
        if (n < 0 && errno == EINTR) continue;
        return -1;
    }
}

/* The sentinel's own table is on its stack, the forking thread's, which it
 * alone uses from its fork on: nothing of it is shared with the runner. */
static void hero_run_sentinel_loop(int in) {
    int held[HERO_RUN_LIVE];
    memset(held, 0, sizeof held);
    for (;;) {
        HeroSentinelWord w;
        size_t have = 0;
        while (have < sizeof w) {
            ssize_t got = read(in, (char *)&w + have, sizeof w - have);
            if (got > 0) { have += (size_t)got; continue; }
            if (got < 0 && errno == EINTR) continue;
            break;
        }
        if (have < sizeof w) break;
        int free_at = -1;
        int held_at = -1;
        for (int i = 0; i < HERO_RUN_LIVE; i += 1) {
            if (held[i] == w.pid && w.pid > 0) { held_at = i; break; }
            if (held[i] == 0 && free_at < 0) free_at = i;
        }
        if (w.op == HERO_SENTINEL_HOLD && held_at < 0 && free_at >= 0) held[free_at] = w.pid;
        if (w.op == HERO_SENTINEL_FREE && held_at >= 0) held[held_at] = 0;
    }
    for (int i = 0; i < HERO_RUN_LIVE; i += 1) {
        int pid = held[i];
        if (pid > 0) {
            kill(-(pid_t)pid, SIGKILL);
            kill((pid_t)pid, SIGKILL);
        }
    }
    _exit(0);
}

/* In the forked sentinel: its own group, the ending signals ignored but
 * SIGTERM, an empty mask, and every descriptor closed but `keep`. */
static void hero_run_sentinel_alone(int keep) {
    setpgid(0, 0);
    struct sigaction plain;
    memset(&plain, 0, sizeof plain);
    sigemptyset(&plain.sa_mask);
    plain.sa_handler = SIG_DFL;
    sigaction(SIGTERM, &plain, NULL);
    plain.sa_handler = SIG_IGN;
    sigaction(SIGINT, &plain, NULL);
    sigaction(SIGQUIT, &plain, NULL);
    sigaction(SIGHUP, &plain, NULL);
    sigaction(SIGPIPE, &plain, NULL);
    sigset_t none;
    sigemptyset(&none);
    sigprocmask(SIG_SETMASK, &none, NULL);
    struct proc_fdinfo listed[256];
    for (;;) {
        int bytes = proc_pidinfo(getpid(), PROC_PIDLISTFDS, 0, listed, (int)sizeof listed);
        int n = bytes > 0 ? bytes / (int)sizeof(struct proc_fdinfo) : 0;
        int closed = 0;
        for (int i = 0; i < n; i += 1) {
            int fd = listed[i].proc_fd;
            if (fd != keep) {
                close(fd);
                closed += 1;
            }
        }
        if (closed == 0) break;
    }
}

/* Under the lock: a sentinel started, the old one ended first and reaped, and
 * every child of the table named to the new one. A runner whose pipe or fork
 * fails runs on unguarded, as the Linux arm does on a refused request. */
static void hero_run_sentinel_restart_locked(void) {
    if (hero_run_sentinel_pid > 0) {
        kill(hero_run_sentinel_pid, SIGKILL);
        while (waitpid(hero_run_sentinel_pid, NULL, 0) < 0 && errno == EINTR) { }
        hero_run_sentinel_pid = 0;
    }
    int old = atomic_load(&hero_run_sentinel_fd);
    atomic_store(&hero_run_sentinel_fd, -1);
    if (old >= 0) close(old);
    int ends[2];
    if (pipe(ends) != 0) return;
    if (fcntl(ends[0], F_SETFD, FD_CLOEXEC) != 0 || fcntl(ends[1], F_SETFD, FD_CLOEXEC) != 0
        || fcntl(ends[1], F_SETNOSIGPIPE, 1) != 0 || fcntl(ends[1], F_SETFL, O_NONBLOCK) != 0) {
        close(ends[0]);
        close(ends[1]);
        return;
    }
    pid_t s = fork();
    if (s < 0) {
        close(ends[0]);
        close(ends[1]);
        return;
    }
    if (s == 0) {
        hero_run_sentinel_alone(ends[0]);
        hero_run_sentinel_loop(ends[0]);
    }
    close(ends[0]);
    hero_run_sentinel_pid = s;
    atomic_store(&hero_run_sentinel_fd, ends[1]);
    for (int i = 0; i < HERO_RUN_LIVE; i += 1) {
        int pid = atomic_load(&hero_run_live[i]);
        if (pid > 0 && hero_run_sentinel_put(ends[1], HERO_SENTINEL_HOLD, (int32_t)pid) != 0) {
            hero_run_sentinel_failed = 1;
            return;
        }
    }
    hero_run_sentinel_failed = 0;
}

/* Under the lock: whether the sentinel stands. Its own child, so a sentinel
 * that ended is reaped here, and one reaped by another call is gone too. */
static int hero_run_sentinel_stands_locked(void) {
    if (hero_run_sentinel_pid <= 0 || atomic_load(&hero_run_sentinel_fd) < 0 || hero_run_sentinel_failed) return 0;
    pid_t seen = waitpid(hero_run_sentinel_pid, NULL, WNOHANG);
    if (seen == 0) return 1;
    hero_run_sentinel_pid = 0;
    return 0;
}

/* Before a launch: a sentinel standing, started or restarted. */
static void hero_run_sentinel_ready(void) {
    pthread_mutex_lock(&hero_run_sentinel_lock);
    if (!hero_run_sentinel_stands_locked()) hero_run_sentinel_restart_locked();
    pthread_mutex_unlock(&hero_run_sentinel_lock);
}

/* The runner's word, after a launch and before a reap: a word the sentinel
 * cannot take restarts it, which names the table, and the word is said again. */
static void hero_run_sentinel_say(int32_t op, int32_t pid) {
    pthread_mutex_lock(&hero_run_sentinel_lock);
    int fd = atomic_load(&hero_run_sentinel_fd);
    if (fd < 0 || hero_run_sentinel_put(fd, op, pid) != 0) {
        hero_run_sentinel_restart_locked();
        fd = atomic_load(&hero_run_sentinel_fd);
        if (fd >= 0 && hero_run_sentinel_put(fd, op, pid) != 0) hero_run_sentinel_failed = 1;
    }
    pthread_mutex_unlock(&hero_run_sentinel_lock);
}

/* The child's own word, after its fork and before it asks after its parent:
 * its copy of the write end, no lock, and a word it cannot write left to the
 * runner, which names it once its pid is in the table. */
static void hero_run_sentinel_child_hold(void) {
    int fd = atomic_load(&hero_run_sentinel_fd);
    if (fd >= 0) (void)hero_run_sentinel_put(fd, HERO_SENTINEL_HOLD, (int32_t)getpid());
}
#endif

/* A waiter about to reap: while the process is ending it waits for the handler
 * to end it instead, so no pid the handler may still hold is freed. */
static void hero_run_unless_ending(void) {
    if (atomic_load(&hero_run_ending) == 0) return;
    for (;;) pause();
}

/* The child half of the POSIX arm: redirect, exec, and if the exec fails say so
 * down the pipe before dying. Never returns. */
/* **THE CHILD ALWAYS GETS A GROUP, AND WHERE THERE IS A TERMINAL IT GETS THAT
 * TOO.**
 *
 * A group is what makes the whole tree killable, and it is the POSIX answer to
 * the job object on the Windows arm. It has one cost and it is not theoretical:
 * a program in a BACKGROUND process group that READS the controlling terminal
 * is stopped with SIGTTIN and never exits, so the caller waits out the whole
 * watchdog. Measured at panel 174 on Darwin and on Linux, same program, same
 * answer — `Stopped (tty input)`, signal 21.
 *
 * **The first repair made the group CONDITIONAL — no group where stdin was the
 * terminal — and the author refused it** (2026-09-22, in their words: *avoid
 * these compromises, make things more robust; if you have to handle it
 * differently, handle it differently and make it more robust*). They were
 * right: skipping the group leaves `heroes run` on a program that reads the
 * keyboard with exactly the orphan this file is about. It is a case left out
 * rather than solved.
 *
 * What solves it is what every shell has done for forty years: make the child's
 * group the FOREGROUND one while it runs, and take the terminal back
 * afterwards. Then no case is left out —
 *
 *   stdin is a file            no SIGTTIN is possible; group, nothing else
 *   stdin inherited, not a tty no SIGTTIN is possible; group, nothing else
 *   stdin IS the terminal
 *     and we hold it           group, and the child is handed the terminal
 *     and we do not            group; the child would have been background
 *                              today as well, so nothing changes for it
 *
 * That last row is what keeps this a fact about the VALUE rather than a premise
 * about the world (`.claude/rules/module-shape.md`): the question is whether
 * THIS process holds the terminal, never whether a terminal exists somewhere.
 *
 * -1 means there is nothing to hand over; otherwise it is the group that holds
 * the terminal now and must get it back. */
static pid_t hero_run_tty_holder(void) {
    if (!isatty(0)) return -1;

    pid_t mine = getpgrp();
    pid_t holder = tcgetpgrp(0);

    /* Only a process that HOLDS the terminal may hand it on. Taking it while
     * background would steal it from whoever has it. */
    if (mine == -1 || holder == -1 || holder != mine) return -1;
    return holder;
}

/* Hand the terminal to `group` — or take it back, when `group` is the holder
 * saved before the child started.
 *
 * **SIGTTOU IS IGNORED ACROSS THE CALL, AND THE SECOND CALL IS WHY.** By the
 * time the terminal is taken back, this process is itself in a background group
 * — the child's group holds it — and `tcsetpgrp` from a background process
 * raises SIGTTOU, whose default action would STOP this very process. A guard
 * against one program hanging must not hang the program running it. The
 * disposition is restored, so nothing a later program does inherits our
 * choice. */
static void hero_run_tty_give(pid_t group) {
    struct sigaction ignore;
    struct sigaction saved;
    memset(&ignore, 0, sizeof ignore);
    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);

    if (sigaction(SIGTTOU, &ignore, &saved) != 0) return;
    (void)tcsetpgrp(0, group);
    (void)sigaction(SIGTTOU, &saved, NULL);
}

/* **THE TERMINAL COMES BACK ON EVERY EXIT, AND NOT BECAUSE ANYBODY REMEMBERED
 * TO WRITE IT FIVE TIMES.**
 *
 * The POSIX arm below leaves through five returns — two `waitpid` failures, the
 * watchdog's 124, a child that never exec'd, and the ordinary answer — and a
 * `heroes run` that took any of them must not leave the author's shell without
 * its terminal. Five call sites is five chances to forget one, and the one
 * forgotten is the one nobody runs.
 *
 * So it is a destructor instead. `__attribute__((cleanup))` runs when the
 * variable leaves scope, early returns included; it is a GNU extension and
 * `-std=gnu11` is what this project names rather than inherits
 * (`.claude/rules/generated-c.md`). The Windows arm never reaches here. */
static void hero_run_tty_restore(pid_t *holder) {
    if (*holder == -1) return;
    atomic_store(&hero_run_tty_back, 0);
    hero_run_tty_give(*holder);
}

/* **A CHILD STOPPED AT THE TERMINAL STOPS THIS PROCESS WITH IT** (defect 436,
 * 2026-10-07), the job control a shell user expects. A child handed the
 * terminal is the program in front, so Ctrl-Z stops it and not this process,
 * which went on waiting for an exit that could not come while the shell waited
 * for this process: the terminal answered nobody. So where this process held
 * the terminal and handed it on (`holder`), a stop is waited for as well as an
 * exit, and on one the terminal is taken back where the child holds it and
 * this process stops itself with SIGTSTP, so the shell sees its job stopped
 * and has its prompt. `fg` continues this process in front: the terminal goes
 * to the child again and the child is continued with SIGCONT. `bg` continues
 * it in the background, holding no terminal, so the child is continued and
 * left in the background, where a read of the terminal stops it again and
 * this process with it, until a `fg`. With no terminal handed on, a stop is
 * not waited for, as before.
 *
 * Where SIGTSTP is ignored there is no job control to answer, and SIGSTOP
 * stops this process all the same; where this process is a group no shell
 * holds, an orphaned one, the kernel discards SIGTSTP, the call returns at
 * once, and the child is continued, which is what a session without job
 * control does with Ctrl-Z. */
static void hero_run_stopped(pid_t child, pid_t holder) {
    if (tcgetpgrp(0) == child) hero_run_tty_give(holder);

    struct sigaction now;
    int stop = SIGTSTP;
    if (sigaction(SIGTSTP, NULL, &now) == 0 && !(now.sa_flags & SA_SIGINFO) && now.sa_handler == SIG_IGN) {
        stop = SIGSTOP;
    }
    /* `raise` and not `kill(getpid(), …)`: the signal must stop THIS thread
     * before the call returns. Sent to the process, it was taken by another
     * of `heroes`'s threads on Linux arm64, and this one went on past the
     * stop, gave the terminal back to the child and continued it: `ps` read
     * `heroes` stopped and the program running in the background. */
    raise(stop);

    /* Continued. In front again, the terminal is the child's. */
    if (tcgetpgrp(0) == getpgrp()) hero_run_tty_give(child);
    kill(-child, SIGCONT);
}

/* Wait for the child, and sweep its group before its pid can be reused.
 *
 * **WNOWAIT IS THE WHOLE POINT.** `waitid` reports the exit without reaping, so
 * the child stays a zombie and its pid — which is also its GROUP id — is pinned
 * by the kernel. Reap first and `kill(-pgid)` afterwards and this runtime is
 * signalling a group number somebody else may already own, which is a
 * corruption class rather than a leak (design.md §1.12).
 *
 * `si_pid` is zeroed first because POSIX leaves it unspecified when WNOHANG
 * finds nothing; zero is then the documented way to tell "not yet" from "here
 * it is". */
static pid_t hero_run_reap(pid_t child, int slot, int *wait_status, int nohang,
                           pid_t holder) {
    siginfo_t seen;
    memset(&seen, 0, sizeof seen);
    int flags = WEXITED | WNOWAIT | (nohang ? WNOHANG : 0) | (holder != -1 ? WSTOPPED : 0);
    if (waitid(P_PID, (id_t)child, &seen, flags) != 0) return -1;
    if (seen.si_pid == 0) return 0;

    /* A stop is no exit (defect 436). WNOWAIT leaves its report standing, so
     * it is taken here, without waiting, and answered where the child held the
     * terminal; one already gone, the child continued by somebody else, is no
     * stop to answer. **Darwin reports a stop to WEXITED alone**, measured on
     * macOS 26 with WNOWAIT: `si_code` CLD_STOPPED, again at every call until
     * WSTOPPED takes it. Read as an exit, the sweep's SIGKILL ended the
     * stopped program and `heroes run` answered 137, at Ctrl-Z and at a
     * `kill -STOP` with no terminal alike; so it is taken with no terminal
     * too, and the wait goes on, as it does on Linux. */
    if (seen.si_code == CLD_STOPPED || seen.si_code == CLD_TRAPPED) {
        siginfo_t taken;
        memset(&taken, 0, sizeof taken);
        if (waitid(P_PID, (id_t)child, &taken, WSTOPPED | WNOHANG) == 0 && taken.si_pid != 0 && holder != -1) {
            hero_run_stopped(child, holder);
        }
        return 0;
    }

    /* Anything the child left behind dies here, on the ORDINARY exit as well as
     * on the timeout: the recorded holder was left by a compiler driver whose
     * parent had finished. */
    kill(-child, SIGKILL);

    /* Out of the table before it is reaped, and not reaped at all once this
     * process is ending (defect 425). */
    hero_run_let_go(slot);
    hero_run_unless_ending();
    return waitpid(child, wait_status, 0);
}

static void hero_run_child(const char *program, int report, int tty,
                           const char *in_path, const char *out_path,
                           const char *err_path, int64_t write_limit,
                           const sigset_t *before, pid_t parent) {
#if defined(__linux__)
    /* **AND IT DIES WITH THIS PROCESS, SIGKILL INCLUDED** (defect 437,
     * 2026-10-07). A signal this process can catch reaches the child through
     * `hero_run_ended`; SIGKILL cannot be caught, and a `heroes` killed with
     * it left its program running with parent 1 (measured on Linux arm64: a
     * `sleep 30` run through `hero_run_go` outlived its runner's `kill -9`).
     * Linux sends a child the signal it asks for when the THREAD that forked
     * it ends, and that thread waits here in `hero_run_go` until the child is
     * reaped, so its end is this process's. Asked before exec, which keeps the
     * request (a set-user-ID program's exec clears it); and the parent asked
     * for again after it, because a parent that died first left nobody whose
     * end could send it, so the child ends itself the same way. A refusal
     * leaves the child unguarded rather than not run, as the Windows arm
     * treats its job. Every child this runtime starts asks the same, so a
     * chain of them, a `heroes` running a `heroes` running a program, ends
     * link by link; what a program starts by other means is its own. */
    (void)prctl(PR_SET_PDEATHSIG, SIGKILL);
    if (getppid() != parent) raise(SIGKILL);
#elif defined(__APPLE__)
    /* The same end on Darwin through the sentinel (defect 465): the child
     * names itself in its own group BEFORE it asks after its parent, holding
     * its copy of the write end until exec, so a runner gone already is seen
     * here and one that goes later leaves the sentinel holding this name. */
    setpgid(0, 0);
    hero_run_sentinel_child_hold();
    if (getppid() != parent) raise(SIGKILL);
#else
    (void)parent;
#endif

    /* Between fork and exec is the only place this can go. The parent cannot
     * know the pid soon enough to win the race against the child's own first
     * spawn, so both sides set it and whichever runs first wins — the
     * documented way to close it. */
    setpgid(0, 0);

    /* The dispositions this process caught given back, and the mask the
     * launch held, before anything here can block (defect 425). */
    hero_run_child_signals(before);

    /* The ceiling `hero_run_limit_writes` asked for, on this child alone: the
     * soft limit only, so the hard one the parent had is left as it was, and
     * SIGXFSZ ignored, which exec keeps, so a write past it answers EFBIG
     * rather than killing the child. */
    if (write_limit > 0) {
        struct rlimit most;
        struct sigaction ignore;
        memset(&ignore, 0, sizeof ignore);
        ignore.sa_handler = SIG_IGN;
        sigemptyset(&ignore.sa_mask);
        int held = getrlimit(RLIMIT_FSIZE, &most) == 0;
        if (held) {
            rlim_t asked = (rlim_t)write_limit;
            most.rlim_cur = most.rlim_max != RLIM_INFINITY && most.rlim_max < asked ? most.rlim_max : asked;
            held = sigaction(SIGXFSZ, &ignore, NULL) == 0 && setrlimit(RLIMIT_FSIZE, &most) == 0;
        }
        if (!held) {
            int failure = errno;
            ssize_t ignored = write(report, &failure, sizeof failure);
            (void)ignored;
            _exit(126);
        }
    }

    /* And the same race for the terminal: the child may reach its first read
     * before the parent's `tcsetpgrp` lands. */
    if (tty) hero_run_tty_give(getpid());

    /* An empty path leaves the stream alone, so the child reads and writes
     * whatever this process was reading and writing.
     *
     * STDIN JOINED THE OTHER TWO ON 2026-09-04, and the reason it was missing
     * says something about how a harness grows: the two streams a golden
     * DIFFS were plumbed first, and nothing had ever needed to feed a program.
     * The corpus has no program that reads its input, every other language's
     * example suite has one (Wren spells it `// stdin:`), and the price was
     * measured before it was paid — one handle here and one on the Windows
     * arm, which is exactly the gate the scheduling item set. */
    if (!hero_run_inherits(in_path)) {
        int in = open(in_path, O_RDONLY);
        if (in < 0) {
            int failure = errno;
            ssize_t ignored = write(report, &failure, sizeof failure);
            (void)ignored;
            _exit(126);
        }
        dup2(in, 0);
        if (in > 2) close(in);
    }
    if (!hero_run_inherits(out_path)) {
        int out = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out < 0) {
            int failure = errno;
            ssize_t ignored = write(report, &failure, sizeof failure);
            (void)ignored;
            _exit(126);
        }
        dup2(out, 1);
        if (out > 2) close(out);
    }
    if (!hero_run_inherits(err_path)) {
        int err = open(err_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (err < 0) {
            int failure = errno;
            ssize_t ignored = write(report, &failure, sizeof failure);
            (void)ignored;
            _exit(126);
        }
        dup2(err, 2);
        if (err > 2) close(err);
    }

    execvp(program, hero_run_words);

    /* Only reachable when the exec failed. The pipe is close-on-exec, so the
     * parent seeing bytes here means "never started" and seeing none means the
     * program ran — which is the distinction `system()` cannot make. */
    int failure = errno;
    ssize_t ignored = write(report, &failure, sizeof failure);
    (void)ignored;
    _exit(127);
}
#endif

/* **THE WATCHDOG LIVES HERE, and it used to be a program called `timeout`.**
 *
 * A program that never returns burns a whole unattended run rather than one
 * case — measured 2026-08-14, when an `examples/sdl/` binding opened a MODAL
 * DIALOG on a machine with no SDL3 and two processes sat at 0% CPU for ninety
 * minutes waiting for a click nobody could give them.
 *
 * The harness used to buy that protection by prefixing every invocation with
 * coreutils' `timeout 120`. Windows ships a `TIMEOUT.EXE` that PAUSES for N
 * seconds and launches nothing, so on that platform the prefix broke 271 of
 * the net's checks (2026-08-31) — and once the probe was taught to reject it,
 * Windows had no watchdog at all. It showed within one run: two orphan
 * `heroes.exe` and a `clang.exe` held `build/harness/stdout` open, and Windows
 * will not let anybody delete a file a handle still holds, so the NEXT run
 * could not even clear its build directory.
 *
 * So the limit stops being an external program with two meanings and becomes
 * what this milestone made everything else: a runtime call. `WaitForSingleObject`
 * already takes a deadline on Windows, and POSIX gets one by waiting in small
 * steps instead of forever. Both answer **124** on a kill, which is coreutils'
 * own code — chosen so the callers that used to read `timeout`'s answer read the
 * same number here.
 *
 * Zero means no limit, which is the default and what every existing caller
 * gets. */
static _Thread_local int64_t hero_run_limit_seconds = 0;

void hero_run_limit(int64_t seconds) {
    hero_run_limit_seconds = seconds > 0 ? seconds : 0;
}

/* **A CEILING ON WHAT THE NEXT CHILD MAY WRITE**, for the one case in the net
 * that must make a write fail half-way (defect 136). That case holds that a
 * verb rewriting the author's file and unable to finish leaves the file as it
 * was, and the failure it must make is the defect's own: a disk that fills while
 * the new text is written. A full disk cannot be asked for in a test (the
 * reproduction used a 2 MB disk image), so the kernel is asked for the nearest
 * thing: RLIMIT_FSIZE, with SIGXFSZ ignored, makes a write past `bytes` fail
 * with EFBIG as a full disk fails with ENOSPC, on the child alone, set between
 * fork and exec. The next `hero_run_go` takes it and clears it, so it never
 * reaches a second child. Windows keeps no ceiling on the size a process may
 * write, so there the answer is 0 and the case says it did not run. */
#if !defined(_WIN32)
static _Thread_local int64_t hero_run_write_limit = 0;
#endif

int64_t hero_run_limit_writes(int64_t bytes) {
#if defined(_WIN32)
    (void)bytes;
    return 0;
#else
    hero_run_write_limit = bytes > 0 ? bytes : 0;
    return 1;
#endif
}

/* A number no other call in this process will answer, for a caller that needs
 * a FILE NAME nothing else will reuse.
 *
 * **It is here because the language has no mutable global** (spec § 2: *"There
 * are no mutable globals"*), so a Heroes caller cannot keep a counter of its
 * own; and it is `_Atomic` rather than `_Thread_local` because two threads
 * sharing one directory must not both be handed 1. `alloc.c`'s live-block
 * counters are the precedent (panel 174, route B). */
static _Atomic int64_t hero_run_serial_next = 0;

int64_t hero_run_serial(void) {
    return ++hero_run_serial_next;
}

/* The operating system's reason for the last refusal to start a child, held
 * until the next `hero_run_go` clears it. `hero_os.h` carries what it is for;
 * what it is NOT is a second status: 0 here means either that the child started
 * or that the failure was this runtime's own, and `*status` is what separates
 * those two. Thread-local for the same reason the limit is. */
static _Thread_local int64_t hero_run_why_code = 0;

int64_t hero_run_why(void) {
    return hero_run_why_code;
}

/* WHAT ENDED THE LAST CHILD WHEN IT DID NOT END ITSELF (defect 170,
 * 2026-10-03). `hero_run_go` answers a child a signal killed as 128 plus the
 * signal, which a child may also exit with of its own accord; this says which
 * it was, POSIX's own number, so a caller can tell a C compiler that died from
 * one that refused. Debian clang 22.1.8 killed on its own stack by 10,000
 * nested variants wrote nothing at all, measured on Linux arm64. Windows has
 * no signals: a child that crashed exits with its exception's code, in
 * NTSTATUS's error range (0xC0000000 and above), and that code is the answer
 * there, unrun on the box. 0 where the child exited by itself or the watchdog
 * ended it; cleared by every `hero_run_go`. Thread-local, as the why is. */
static _Thread_local int64_t hero_run_signal_code = 0;

int64_t hero_run_signal(void) {
    return hero_run_signal_code;
}

/* Run the program with the words pushed so far, `hero_run_words[0]` included as
 * argv[0] by convention. Returns the exit code; `*status` is HERO_OS_OK when the
 * program ran at all, HERO_OS_NOT_FOUND when it could not be started, and
 * HERO_OS_FAILED when this runtime could not try. A caller reads `*status`
 * first and the code second (panel 098 condition 3).
 *
 * The word list is left in place: `hero_run_reset` is the caller's to call, and
 * the leak gate says so at exit if it forgets. */
int64_t hero_run_go(const char *program, const char *in_path,
                    const char *out_path, const char *err_path,
                    int64_t *status) {
    /* Cleared on the way in, so a reader of `hero_run_why` after a successful
     * call sees 0 rather than the last refusal of an hour ago. */
    hero_run_why_code = 0;
    hero_run_signal_code = 0;

    if (hero_run_count == 0) {
        *status = HERO_OS_FAILED;
        return -1;
    }
    hero_run_words[hero_run_count] = NULL;

#if defined(_WIN32)
    /* Windows has no argv at the operating-system level: `CreateProcess` takes
     * one command-line STRING which the child's C runtime parses back. So the
     * quoting an argument list deletes on POSIX does not disappear here, it
     * changes algorithm and moves into C — see `hero_run_win_quote`. */
    HeroStr line = hero_run_win_command_line();
    SECURITY_ATTRIBUTES inherit;
    inherit.nLength = sizeof inherit;
    inherit.lpSecurityDescriptor = NULL;
    inherit.bInheritHandle = TRUE;

    /* **`FILE_SHARE_DELETE` and NOT `FILE_SHARE_WRITE`, and the difference is
     * the whole of panel 174.**
     *
     * DELETE lets a directory holding one of these files be removed while a
     * descendant still holds it: without it `hero_dir_remove_tree` fails,
     * `tests/harness/main.hero` exits 2 after printing a green line, and that
     * is the failure `tests/harness/shell.hero` recorded in August. Measured at
     * the sitting: `RemoveDirectory` refused, error 145.
     *
     * WRITE is vetoed. It would let the reopen succeed under a live writer, so
     * `CREATE_ALWAYS` truncates while an orphan keeps its own file offset and
     * that orphan's later bytes land in the capture a DIFFERENT case is judged
     * on — measured at 150 bytes where 17 were owed, on Windows, Darwin and
     * Linux alike. A loud `ERROR_SHARING_VIOLATION` traded for a silent wrong
     * answer, which design.md §1.12 refuses. */
    HANDLE out = hero_run_inherits(out_path)
        ? GetStdHandle(STD_OUTPUT_HANDLE)
        : CreateFileA(out_path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE,
                      &inherit, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE err = hero_run_inherits(err_path)
        ? GetStdHandle(STD_ERROR_HANDLE)
        : CreateFileA(err_path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE,
                      &inherit, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (out == INVALID_HANDLE_VALUE || err == INVALID_HANDLE_VALUE) {
        /* Asked BEFORE the CloseHandle calls below, which would overwrite it. */
        hero_run_why_code = (int64_t)GetLastError();
        if (out != INVALID_HANDLE_VALUE && !hero_run_inherits(out_path)) CloseHandle(out);
        if (err != INVALID_HANDLE_VALUE && !hero_run_inherits(err_path)) CloseHandle(err);
        hero_str_decref(line);
        *status = HERO_OS_FAILED;
        return -1;
    }

    /* **A std handle can be DEAD, and inheriting a dead one kills the launch.**
     * A detached compiler — `nohup` on a build server, an ssh session that
     * ended, a CI daemon — keeps the VALUE GetStdHandle answers while the
     * console behind it is gone, and CreateProcess refuses to inherit it:
     * the child never starts, the error file stays empty, and the caller
     * reports "the runtime did not compile" with nothing to read. Measured
     * 2026-08-31 on the Windows VPS: the same net run works in a live ssh
     * session and dies at the FIRST clang the moment the session that
     * spawned it is gone. GetFileType is the probe — a live handle has a
     * type, a dead one answers UNKNOWN with an error — and the stand-in is
     * NUL, which is what a detached process's stdin honestly is. */
    HANDLE in = INVALID_HANDLE_VALUE;
    HANDLE nul_in = INVALID_HANDLE_VALUE;
    HANDLE file_in = INVALID_HANDLE_VALUE;

    /* A named path is a file the child reads; an empty one is the inherited
     * handle, and the paragraph below is why inheriting needs a probe. */
    if (!hero_run_inherits(in_path)) {
        file_in = CreateFileA(in_path, GENERIC_READ, FILE_SHARE_READ, &inherit,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file_in == INVALID_HANDLE_VALUE) {
            hero_run_why_code = (int64_t)GetLastError();
            if (!hero_run_inherits(out_path)) CloseHandle(out);
            if (!hero_run_inherits(err_path)) CloseHandle(err);
            hero_str_decref(line);
            *status = HERO_OS_FAILED;
            return -1;
        }
        in = file_in;
    }
    SetLastError(0);
    if (file_in == INVALID_HANDLE_VALUE) {
        in = GetStdHandle(STD_INPUT_HANDLE);
    }
    if (file_in == INVALID_HANDLE_VALUE &&
        (in == NULL || in == INVALID_HANDLE_VALUE ||
         (GetFileType(in) == FILE_TYPE_UNKNOWN && GetLastError() != 0))) {
        nul_in = CreateFileA("NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                             &inherit, OPEN_EXISTING, 0, NULL);
        in = nul_in;
    }

    STARTUPINFOA startup;
    memset(&startup, 0, sizeof startup);
    startup.cb = sizeof startup;
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = in;
    startup.hStdOutput = out;
    startup.hStdError = err;

    PROCESS_INFORMATION child;
    memset(&child, 0, sizeof child);

    /* **`NULL` for the application name, and the program is the command
     * line's first word instead.** Passing it as `lpApplicationName` makes
     * Windows look for that exact path: no PATH search and no `.exe` appended,
     * so `clang` is not found on a machine that has `clang.exe` on its PATH —
     * which is precisely how the first Windows run of this file failed, with
     * `heroes doctor` reporting no C toolchain on a runner that ships one.
     * With NULL, the documented search runs: the working directory, the
     * system directories, then PATH, with the executable extensions applied.
     *
     * `hero_run_win_command_line` already puts the program first, because
     * argv[0] is a pushed word like every other. */
    (void)program;

    /* **THE CHILD IS PUT IN A JOB, AND THE JOB IS WHAT DIES.**
     *
     * `TerminateProcess` kills one process and not its descendants — measured
     * on the author's Windows box at panel 174, where a terminated middle
     * process left its own child STILL_ACTIVE with the heartbeat still growing,
     * and the next `CreateFileA` on the redirect path answered
     * ERROR_SHARING_VIOLATION. That is defect 074's signature end to end, and
     * `tests/harness/shell.hero` had recorded the holder a month before
     * anybody measured it: a `clang.exe` under a `heroes.exe`.
     *
     * A job object closes it, because a process created by a process in a job
     * joins that job by default: the grandchild was measured inside ours
     * without ever being named. `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE` then makes
     * the last handle's close a kill, so no path out of this function can leave
     * the tree running.
     *
     * `CREATE_SUSPENDED` is not decoration. Assigning after the child is
     * running leaves a window in which it can spawn a grandchild OUTSIDE the
     * job, and the grandchild is the holder.
     *
     * A failure to make the job is not a failure to run the program: the job is
     * a guard, so `job == NULL` falls back to exactly what this function did
     * before. */
    HANDLE job = CreateJobObjectA(NULL, NULL);
    if (job != NULL) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits;
        memset(&limits, 0, sizeof limits);
        limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation,
                                     &limits, sizeof limits)) {
            CloseHandle(job);
            job = NULL;
        }
    }

    BOOL started = CreateProcessA(NULL, (char *)line.ptr, NULL, NULL, TRUE,
                                  job != NULL ? CREATE_SUSPENDED : 0,
                                  NULL, NULL, &startup, &child);
    /* **Read on the very next line, because every call below resets it.**
     * `CloseHandle` succeeding sets the thread's last error to 0 on some
     * Windows versions, so a `GetLastError` after the cleanup answers about the
     * cleanup. This is the number the caller wants and the only place it is
     * still true. */
    if (!started) hero_run_why_code = (int64_t)GetLastError();
    if (started && job != NULL) {
        /* If the assignment is refused the child is still suspended, so it is
         * resumed unguarded rather than left frozen for ever. */
        if (!AssignProcessToJobObject(job, child.hProcess)) {
            CloseHandle(job);
            job = NULL;
        }

        /* **AND IF THE RESUME ITSELF FAILS THE CHILD IS KILLED, not waited
         * for.** A thread that never resumes never exits, so the wait below
         * would hang this process and the 120 s limit would only shorten it. A
         * launch we cannot complete is a launch that did not happen, and the
         * caller is told so in the one vocabulary it has. */
        if (ResumeThread(child.hThread) == (DWORD)-1) {
            hero_run_why_code = (int64_t)GetLastError();

            if (job != NULL) {
                TerminateJobObject(job, 1);
                CloseHandle(job);
                /* NULLed, because the `!started` line below closes it again
                 * otherwise — a double CloseHandle, which is the one thing a
                 * guard against a hang must not introduce. */
                job = NULL;
            } else {
                TerminateProcess(child.hProcess, 1);
            }
            CloseHandle(child.hProcess);
            CloseHandle(child.hThread);
            started = FALSE;
        }
    }

    if (!started && job != NULL) CloseHandle(job);
    if (!hero_run_inherits(out_path)) CloseHandle(out);
    if (!hero_run_inherits(err_path)) CloseHandle(err);
    if (nul_in != INVALID_HANDLE_VALUE) CloseHandle(nul_in);
    if (file_in != INVALID_HANDLE_VALUE) CloseHandle(file_in);
    hero_str_decref(line);
    if (!started) {
        *status = HERO_OS_NOT_FOUND;
        return -1;
    }

    DWORD waited = WaitForSingleObject(
        child.hProcess,
        hero_run_limit_seconds > 0 ? (DWORD)(hero_run_limit_seconds * 1000) : INFINITE);
    if (waited == WAIT_TIMEOUT) {
        /* 124 is what the caller is told, whatever the kill reports: the child
         * is gone by our hand, so its own code would be a fiction. */
        if (job != NULL) TerminateJobObject(job, 124);
        else TerminateProcess(child.hProcess, 124);
        WaitForSingleObject(child.hProcess, 5000);
        if (job != NULL) CloseHandle(job);
        CloseHandle(child.hProcess);
        CloseHandle(child.hThread);
        *status = HERO_OS_OK;
        return 124;
    }
    DWORD code = 0;
    GetExitCodeProcess(child.hProcess, &code);
    if (code >= 0xC0000000u) hero_run_signal_code = (int64_t)code;

    /* **THE SWEEP RUNS ON THE ORDINARY EXIT TOO, and that is the point.** The
     * recorded holder was left by a `heroes.exe` that FINISHED; no watchdog was
     * involved, and the timestamps of the failing CI run exclude one — the
     * whole 136-case `run` suite took 65 s against a 120 s limit. A
     * `hero_run_go` call means "run this and give me its exit code", so a
     * descendant outliving the call is the defect rather than a feature. */
    if (job != NULL) {
        TerminateJobObject(job, 0);
        CloseHandle(job);
    }
    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);
    *status = HERO_OS_OK;
    return (int64_t)code;
#else
    /* This child's alone, and never a second one's. */
    int64_t write_limit = hero_run_write_limit;
    hero_run_write_limit = 0;

    int report[2];
    if (pipe(report) != 0) {
        hero_run_why_code = (int64_t)errno;
        *status = HERO_OS_FAILED;
        return -1;
    }
    if (fcntl(report[1], F_SETFD, FD_CLOEXEC) != 0) {
        /* Before the closes, which are free to set `errno` themselves. */
        hero_run_why_code = (int64_t)errno;
        close(report[0]);
        close(report[1]);
        *status = HERO_OS_FAILED;
        return -1;
    }

    pid_t tty_holder __attribute__((cleanup(hero_run_tty_restore))) =
        hero_run_tty_holder();

    /* A signal ending this process reaches the child it is about to wait on
     * (defect 425): the four caught where they were at their default, and held
     * on this thread from the look at `hero_run_ending` until the child's pid
     * is in the table, so the handler finds it there or the launch never
     * happens. */
    hero_run_catch_endings();
#if defined(__APPLE__)
    hero_run_sentinel_ready();
#endif
    sigset_t endings;
    sigset_t before;
    hero_run_ending_set(&endings);
    pthread_sigmask(SIG_BLOCK, &endings, &before);
    atomic_fetch_add(&hero_run_launching, 1);
    if (atomic_load(&hero_run_ending) != 0) {
        atomic_fetch_sub(&hero_run_launching, 1);
        pthread_sigmask(SIG_SETMASK, &before, NULL);
        close(report[0]);
        close(report[1]);
        *status = HERO_OS_FAILED;
        return -1;
    }

    /* Asked before the fork, so the child compares its parent with this
     * process and not with whatever adopted it (defect 437). */
    pid_t parent = getpid();
    pid_t child = fork();
    if (child < 0) {
        hero_run_why_code = (int64_t)errno;
        atomic_fetch_sub(&hero_run_launching, 1);
        pthread_sigmask(SIG_SETMASK, &before, NULL);
        close(report[0]);
        close(report[1]);
        *status = HERO_OS_FAILED;
        return -1;
    }
    if (child == 0) {
        close(report[0]);
        hero_run_child(program, report[1], tty_holder != -1, in_path, out_path, err_path, write_limit, &before, parent);
    }

    /* The other half of the race: EACCES here means the child has already
     * exec'd, having set it itself, which is the outcome we wanted. */
    setpgid(child, child);

    int slot = hero_run_hold(child);
    atomic_fetch_sub(&hero_run_launching, 1);
#if defined(__APPLE__)
    hero_run_sentinel_say(slot < 0 ? HERO_SENTINEL_FREE : HERO_SENTINEL_HOLD, (int32_t)child);
#endif
    if (slot < 0) {
        kill(-child, SIGKILL);
        while (waitpid(child, NULL, 0) < 0 && errno == EINTR) { }
        hero_panic("more than 1024 children were waited on at once");
    }

    if (tty_holder != -1) {
        atomic_store(&hero_run_tty_back, (int)tty_holder);
        hero_run_tty_give(child);
    }
    pthread_sigmask(SIG_SETMASK, &before, NULL);

    close(report[1]);
    int failure = 0;
    ssize_t got = read(report[0], &failure, sizeof failure);
    close(report[0]);

    int wait_status = 0;
    int killed = 0;

    if (hero_run_limit_seconds > 0) {
        /* Polling rather than `alarm` plus EINTR, because `alarm` reaches into a
         * signal disposition this runtime does not own and a program that binds
         * C may have its own SIGALRM.
         *
         * **THE STEP DOUBLES, and that is a measurement rather than a
         * flourish.** A flat 20 ms step was written first, on the reasoning
         * that 20 ms is under a frame and 120 s is only 6,000 polls. That
         * reasoning is about the LIMIT and the cost is paid by the COMMON CASE:
         * the net starts thousands of processes, most of which live a few
         * milliseconds, and a flat step adds up to a full step of latency to
         * every one of them. The net went past 600 s where it had been ~300.
         *
         * So the first check is free (WNOHANG, no sleep at all), and the step
         * starts at 1 ms and doubles to a 50 ms ceiling. A child that exits in
         * 5 ms is reaped after ~3 polls and under 7 ms of sleeping; a child
         * that hangs costs 12 polls to reach a second and then one every 50 ms,
         * which is nothing beside the thing it is waiting for. */
        int64_t slept_ms = 0;
        int64_t step_ms = 1;
        int64_t limit_ms = hero_run_limit_seconds * 1000;
        for (;;) {
            pid_t seen = hero_run_reap(child, slot, &wait_status, 1, tty_holder);
            if (seen == child) break;
            if (seen < 0 && errno != EINTR) {
                hero_run_why_code = (int64_t)errno;
                hero_run_let_go(slot);
                *status = HERO_OS_FAILED;
                return -1;
            }
            if (slept_ms >= limit_ms) {
                /* The negative pid is the group, and the child is still alive,
                 * so its pid is its own and cannot be anybody else's.
                 *
                 * **SIGTERM FIRST, SIGKILL AFTER A GRACE** (defect 425). A
                 * child killed outright cannot end its own children, and
                 * every child of `hero_run_go` is in a group of its own: a
                 * `heroes` this watchdog killed left the program it ran,
                 * which the harness then never saw. Asked with SIGTERM, a
                 * `heroes` ends its child within `HERO_RUN_GRACE_MS`, and
                 * this grace is longer. SIGCONT after it, so a child stopped
                 * meanwhile acts on it now rather than at the SIGKILL
                 * (defect 436). */
                kill(-child, SIGTERM);
                kill(-child, SIGCONT);
                int64_t grace_ms = 0;
                while (grace_ms < HERO_RUN_WATCHDOG_GRACE_MS) {
                    seen = hero_run_reap(child, slot, &wait_status, 1, tty_holder);
                    if (seen == child || (seen < 0 && errno != EINTR)) break;
                    struct timespec pause_ms;
                    pause_ms.tv_sec = 0;
                    pause_ms.tv_nsec = 10L * 1000L * 1000L;
                    nanosleep(&pause_ms, NULL);
                    grace_ms += 10;
                }
                if (seen != child) {
                    kill(-child, SIGKILL);
                    hero_run_let_go(slot);
                    hero_run_unless_ending();
                    while (waitpid(child, &wait_status, 0) < 0 && errno == EINTR) { }
                }
                killed = 1;
                break;
            }
            struct timespec step;
            step.tv_sec = 0;
            step.tv_nsec = step_ms * 1000L * 1000L;
            nanosleep(&step, NULL);
            slept_ms += step_ms;
            if (step_ms < 50) step_ms *= 2;
        }
    } else {
        /* A stop answered is 0, and the wait goes on (defect 436). */
        for (;;) {
            pid_t seen = hero_run_reap(child, slot, &wait_status, 0, tty_holder);
            if (seen == child) break;
            if (seen < 0 && errno != EINTR) {
                hero_run_why_code = (int64_t)errno;
                hero_run_let_go(slot);
                *status = HERO_OS_FAILED;
                return -1;
            }
        }
    }

    if (killed) {
        *status = HERO_OS_OK;
        return 124;
    }

    if (got == (ssize_t)sizeof failure) {
        /* `failure` IS the child's `errno` at the failed `execv`, written down
         * the report pipe before `_exit`. It is the POSIX half of what
         * `GetLastError` gives the Windows arm, and it was already being read
         * here to decide NOT_FOUND while its value was thrown away. */
        hero_run_why_code = (int64_t)failure;
        *status = HERO_OS_NOT_FOUND;
        return -1;
    }
    *status = HERO_OS_OK;
    if (WIFSIGNALED(wait_status)) {
        hero_run_signal_code = (int64_t)WTERMSIG(wait_status);
        return 128 + WTERMSIG(wait_status);
    }
    return WIFEXITED(wait_status) ? WEXITSTATUS(wait_status) : -1;
#endif
}
