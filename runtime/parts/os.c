/* os.c — the program's three edges, in C because the language cannot reach them
 * (design.md §1.11 Tier 1, `hero_os.h` for why they are not built-ins).
 *
 * Included by `runtime.c` into the one translation unit, like every other part.
 * Nothing here is generic and nothing here allocates outside `str`: the file
 * reader builds its result with `hero_str_from_bytes`, which is the single
 * allocation point §4.20 insists on.
 */

/* **stdout is bytes, on every platform.**
 *
 * Windows opens the standard streams in *text* mode, where the C runtime turns
 * every `\n` into `\r\n` on the way out. The spec says `print` writes its values
 * "with no separator and exactly one trailing newline" — one — and on Windows it
 * was writing two bytes for that one newline, behind the language's back. A
 * corpus program's `main.expected` is compared byte for byte across three
 * platforms, and it caught this on the third leg's second run.
 *
 * `_setmode` is the documented switch and it is called once, before anything is
 * written. Nothing changes on POSIX, where there was never a translation. */
#if defined(_WIN32)
#include <fcntl.h>
#include <io.h>
static void hero_stdout_is_bytes(void) {
    static int done = 0;
    if (!done) {
        done = 1;
        _setmode(_fileno(stdout), _O_BINARY);
        _setmode(_fileno(stderr), _O_BINARY);
    }
}
#else
static void hero_stdout_is_bytes(void) {}
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* **A PANIC'S MESSAGE IS ON THE WIRE BEFORE THE PROCESS CAN DIE**, whatever the
 * platform buffers. Every panic path in this runtime is `fprintf(stderr, ...)`
 * followed by `abort()`, and C11 7.22.4.1p2 leaves it implementation-defined
 * whether `abort` flushes an open stream. On glibc and on Darwin it does not
 * matter: 7.21.3p7 says stderr is "not fully buffered", both make it unbuffered,
 * and the text is already gone by the time `abort` runs.
 *
 * **Measured on Windows 2026-08-31, and it is the other half of that sentence.**
 * "Not fully buffered" permits LINE buffering, and a line-buffered stderr with a
 * message still in it loses the message when `abort` terminates the process:
 * `heroes run` on a program with a deliberate out-of-range index gave **exit 127
 * with both streams empty**, where the same program on Darwin prints
 * `panic: an index is out of range` at exit 134. Four rounds of CI went into
 * finding out what a compiler module was dying of, and the compiler had been
 * saying so all along into a buffer nobody drained.
 *
 * `parts/os.c:204` says of `hero_write_err` that no `fflush` is needed and that
 * this was measured rather than assumed. That measurement was right and its
 * platform was Darwin — and the instrument it names, 141 golden checks over the
 * diagnostic text, runs there too. A premise true where it was taken and false
 * one platform over is CLAUDE.md §11's exact shape, so the sentence keeps its
 * date and gains this one.
 *
 * `_IONBF` rather than an `fflush` at each of the six abort sites: a call that
 * has to be remembered at every exit is a call that will be forgotten at the
 * seventh. This runs once, before a program's first line.
 *
 * **STDOUT JOINED IT ON 2026-09-16, AT `_IOLBF` AND NOT `_IONBF`** (panel 156
 * R3). The function was `hero_err_unbuffered` until then and the name had
 * stopped being true of it.
 *
 * WHAT IT REPAIRS. A program that printed a line and then aborted lost that line
 * on Linux and kept it on this Mac — measured, the `nullread` fixture's `7`. The
 * divergence is two libcs: glibc flushed on `abort()` until 2.27 and removed it
 * citing *"deadlocks and data corruption"*, while the FreeBSD lineage still
 * flushes under a source comment reading `XXX ISO C requires that abort() be
 * async-signal-safe`. The panel's llm-ergonomist, handed the three behaviours
 * blind, reported that the surviving line was what located the defect — not the
 * blame name the sitting had been convened about.
 *
 * WHY NOT AN `fflush` ON THE ABORT PATH, and this is the one place in this file
 * where a standard settles it rather than a measurement: POSIX.1-2024 § 2.4.3's
 * async-signal-safe list contains `write()` and `abort()` and **does not contain
 * `fflush()`**, and C § 7.22.4.1 leaves flushing on `abort` implementation-defined.
 * The same standard downgraded its own *"shall include the effect of `fclose()`"*
 * to *"may"*, and says in its rationale that it did so because `abort()` must be
 * async-signal-safe. So the repair is to leave nothing in the buffer, never to
 * empty it from a handler.
 *
 * WHY `_IOLBF` AND NOT `_IONBF`. `print` ends every line with a newline
 * (`parts/panic.c`), so line buffering loses nothing a program wrote; measured
 * over 200 000 prints, the default reads 0.01 s, `_IOLBF` 0.23 s and `_IONBF`
 * 0.43 s, output byte-identical. And the panel's ffi-pragmatist refused a global
 * `_IONBF` on a ground that survives the price: stdout is a stream a linked C
 * library also writes to, and unbuffering it takes a decision on that library's
 * behalf. `_IOLBF` changes WHEN a line leaves and never whether it arrives, and
 * it is what a terminal-attached program already gets. The caution is recorded
 * rather than dismissed: a program that prints at a firehose while linking a C
 * library that shares stdout is the shape that would reopen this.
 *
 * **AND WINDOWS TAKES `_IONBF`, MEASURED ON THE BOX THE SAME DAY** — the line
 * below was `_IOLBF, 0` for both and that call **kills the process there**. Three
 * probes, one per call, so no abort could hide another: `setvbuf(stdout, NULL,
 * _IOLBF, 0)` is **exit 127 with nothing on either stream**, while `_IOLBF, 4096`
 * and `_IONBF, 0` both return 0 and print. Microsoft's CRT gives a size of 0 to
 * its invalid-parameter handler, which terminates at once and silently — the
 * compiler built from this file would not have survived `heroes doctor`.
 *
 * Passing a size would compile and would not repair anything: that CRT
 * implements `_IOLBF` as FULL buffering, so the line would still be in the
 * buffer when the process died. `_IONBF` is the only mode on that platform that
 * keeps the promise, which makes the ffi seat's caution unavoidable there rather
 * than declined — and the alternative it is weighed against is losing the line
 * outright. Each platform gets the weakest setting that keeps the promise, which
 * is why this is a split and not one call. */
static void hero_streams_survive_abort(void) {
    static int done = 0;
    if (!done) {
        done = 1;
        setvbuf(stderr, NULL, _IONBF, 0);
#if defined(_WIN32)
        setvbuf(stdout, NULL, _IONBF, 0);
#else
        setvbuf(stdout, NULL, _IOLBF, 0);
#endif
    }
}

/* The arguments, as the generated `main` received them. Static, like every other
 * piece of runtime state, so a decoy runtime linked beside this one cannot reach
 * them (runtime.c's own reason for one translation unit). */
static int hero_argc = 0;
static char **hero_argv = NULL;

/* ---- The runtime names the live leases at the crash (panel 172 R1, 173) -----
 *
 * When a C function frees the bytes of a live `.lease()`, the platform
 * allocator refuses the free and kills the process: SIGTRAP from Darwin's small
 * allocator (exit 133) or SIGABRT from its large one and from glibc, whose line
 * is `munmap_chunk(): invalid pointer` for this shape. Nothing in the checker
 * or the emitted C sees it, and stderr was empty — the one shape design.md
 * §1.12 forbids by name, and defect 070.
 *
 * IT SAYS WHAT IT SAW AND NOT WHY, which is the clause both seats of panel 173
 * vetoed the first draft on. The draft asserted *a C function freed bytes this
 * program still leases*, and that is FALSE on six of nine measured paths: a C
 * library's own `abort()`, its failed `assert`, its double free of its own
 * pointer, its `__builtin_trap()`. `siginfo_t` cannot tell any of them from the
 * true case — byte-identical on Darwin, `si_code` -6 for every self-raised
 * SIGABRT on Linux — and a SIGTRAP-only report would be sound here and lose 24%
 * of the true cases (152 of 200 at 133) and all of them on Linux. So the line
 * states the two facts the runtime holds, the count and the Heroes frame, and
 * names the C free as a condition the reader checks. §4.17's `guess` register.
 *
 * THREE THINGS IT MUST NOT DO, each a measurement from panel 173:
 *
 * - SPEAK AFTER THE RUNTIME HAS. Every runtime-initiated death is a line and
 *   then `hero_abort()` (parts/panic.c), which is SIGABRT, which is this
 *   handler. A flag on one of the fifteen sites left a second, false line under
 *   an index panic, a stack exhaustion and a failed `assert` with a lease live
 *   (282, 289, 300 bytes of stderr). `hero_runtime_spoke` is set in the funnel.
 * - SPEAK UNDER THE SANITIZER'S REPORT. ASan intercepts `free` before the
 *   allocator, reports `bad-free` with the `.hero` line and aborts; a line
 *   appended under its report (1324 bytes, measured) is redundant when right and
 *   false when ASan's reason was not the free. Yielded at compile time until
 *   panel 175, which moved the yield to run time: the sanitizer's death
 *   callback raises `hero_runtime_spoke`, so the line is still said where ASan
 *   has nothing to say (see `hero_lease_sanitizer_spoke`). On POSIX first; the
 *   Windows arm kept its compile-time yield until the box ran it on 2026-09-24,
 *   where the same callback silences it under ASan's `double-free` report and it
 *   speaks for an `abort` and a trap, which ASan does not report.
 * - DROP THE DISPOSITION IT FOUND. A handler that was there before us is called
 *   first, as `hero_stack_pass_on` does for SIGSEGV; then the default is
 *   restored and the signal re-raised, so the process dies with the status it
 *   always had rather than one this file chose.
 *
 * WHAT IT CAN AND CANNOT NAME. The Heroes function comes from stack.c's frame
 * walk over the interrupted context — intact, since this runs on the alternate
 * stack — and `abort` → `free` → the C callee → `h_<module>_<name>` is a few
 * frames up. WHICH lease is not recorded: `hero_live_held` is a counter, and a
 * name per lease is a pointer per `.lease()`, priced at panel 173 and not built.
 *
 * WINDOWS HAS ITS OWN ARM, and it was a stub with this paragraph saying so
 * until it was measured on the box (2026-09-21, at the close). Heap corruption
 * there is not a signal: `free` of an interior pointer raises the fail-fast
 * exception `STATUS_HEAP_CORRUPTION`, which a probe read as **0xC0000374 with
 * flags 0x81** — noncontinuable — and which a vectored handler DOES see. So the
 * arm is `stack.c`'s own shape one file over: catch that code, write the same
 * line with `_write`, and return `EXCEPTION_CONTINUE_SEARCH` so the process
 * dies exactly as it did. Without it the three programs defect 070 is about
 * died on Windows with **zero bytes** while saying their piece on the other
 * two, and the goldens that pin the report were red on that leg. */
#if defined(_WIN32)
#include <windows.h>
#include <io.h>

static void hero_lease_say_count_win(long long v) {
    char buf[24];
    int at = (int)sizeof buf;
    if (v == 0) buf[--at] = '0';
    while (v > 0 && at > 0) {
        buf[--at] = (char)('0' + (v % 10));
        v /= 10;
    }
    _write(2, buf + at, (unsigned int)((int)sizeof buf - at));
}

static void hero_lease_say_win(const char *s) { _write(2, s, (unsigned int)strlen(s)); }

/* The lease line, one text for the three ways this arm is reached. */
static void hero_lease_say_leases_win(void) {
    hero_lease_say_win("panic: the process died with ");
    hero_lease_say_count_win((long long)hero_live_held);
    hero_lease_say_win(" lease(s) still live\n"
                       "  `end_lease` is the only thing that may free a `.lease()`. If one reached a C\n"
                       "  function that frees what it is handed, that is this death; if not, this says\n"
                       "  only what was live when the process ended.\n");
}

static LONG WINAPI hero_lease_veh(EXCEPTION_POINTERS *ep) {
    /* 0xC0000374 is `STATUS_HEAP_CORRUPTION`; `winnt.h` does not name it, so
     * the number is written with what it is beside it, as the file's other
     * arms write theirs. */
    if (ep->ExceptionRecord->ExceptionCode == 0xC0000374L && !hero_runtime_spoke && hero_live_held > 0) {
        hero_lease_say_leases_win();
    } else if (ep->ExceptionRecord->ExceptionCode == 0xC0000374L && !hero_runtime_spoke) {
        /* With no lease live, and still a line (panel 175): a pointer C made and
         * gave back twice died here with zero bytes. The exception is
         * noncontinuable, so no later handler can recover it and saying it at
         * the first chance cannot be said above a process that lives on. */
        hero_lease_say_win("panic: the process is dying of exception 0xC0000374, the heap manager's report\n"
                           "  of a corrupted heap, and this runtime did not raise it. Memory given back twice\n"
                           "  is one way to get here; a reason C printed above, if any, is its own.\n");
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

/* THE TWO DEATHS THE HEAP EXCEPTION IS NOT, added at panel 175's landing when
 * the run suite on the box read 134 and 2: a C `abort()` and a C trap, each
 * with the POSIX arms saying their line and this one saying nothing.
 *
 * `abort()` IS NOT AN EXCEPTION HERE. The UCRT's `abort` calls `raise(SIGABRT)`
 * only when a handler is installed, and then `__fastfail`, which no exception
 * handler of any kind is shown. So the handler is `signal`'s, and it ends by
 * calling `abort` again with the disposition back at `SIG_DFL`, which is the
 * fail-fast the process always died of. `signal` resets the disposition before
 * calling the handler, where `sigaction` does not, so the handler puts itself
 * back first: a C library that recovers from one abort is not left with a
 * second one that says nothing.
 *
 * A TRAP IS AN EXCEPTION, AND A CONTINUABLE ONE. `__builtin_trap()` is `ud2`,
 * `STATUS_ILLEGAL_INSTRUCTION`, and a `__try` in C can recover from it. A
 * vectored handler runs before any `__try` does, so it would say *dying* above a
 * process that goes on; the filter of last resort runs only when nothing did,
 * and it calls the filter it found first, as the POSIX arm calls the handler it
 * found. The three codes are the POSIX arm's SIGILL and SIGTRAP. */
static void (__cdecl *hero_lease_prev_abrt_win)(int) = SIG_DFL;
static LPTOP_LEVEL_EXCEPTION_FILTER hero_lease_prev_filter = NULL;

static void __cdecl hero_lease_abrt_win(int signum) {
    signal(SIGABRT, hero_lease_abrt_win);
    void (__cdecl *prev)(int) = hero_lease_prev_abrt_win;
    if (prev != SIG_DFL && prev != SIG_IGN && prev != SIG_ERR && prev != NULL) prev(signum);
    if (!hero_runtime_spoke && hero_live_held > 0) {
        hero_lease_say_leases_win();
    } else if (!hero_runtime_spoke) {
        /* Raised by this process, always: a signal on this platform never comes
         * from another one. */
        hero_lease_say_win("panic: the process is dying of SIGABRT, raised by this process, and this runtime did not raise it.\n"
                           "  A C library ends a program this way when it catches misuse, memory given back\n"
                           "  twice among them; a reason C printed above, if any, is its own.\n");
    }
    signal(SIGABRT, SIG_DFL);
    abort();
}

static const char *hero_lease_trap_name_win(DWORD code) {
    if (code == 0xC000001DUL) return "exception 0xC000001D, an illegal instruction";
    if (code == 0xC0000096UL) return "exception 0xC0000096, a privileged instruction";
    if (code == 0x80000003UL) return "exception 0x80000003, a breakpoint";
    return NULL;
}

static LONG WINAPI hero_lease_last_chance(EXCEPTION_POINTERS *ep) {
    LONG verdict = EXCEPTION_CONTINUE_SEARCH;
    if (hero_lease_prev_filter != NULL) verdict = hero_lease_prev_filter(ep);
    if (verdict == EXCEPTION_CONTINUE_EXECUTION) return verdict;
    const char *what = hero_lease_trap_name_win(ep->ExceptionRecord->ExceptionCode);
    if (what != NULL && !hero_runtime_spoke && hero_live_held > 0) {
        hero_lease_say_leases_win();
    } else if (what != NULL && !hero_runtime_spoke) {
        hero_lease_say_win("panic: the process is dying of ");
        hero_lease_say_win(what);
        hero_lease_say_win(", and this runtime did not raise it.\n"
                           "  A C library ends a program this way when it catches misuse, memory given back\n"
                           "  twice among them; a reason C printed above, if any, is its own.\n");
    }
    return verdict;
}

/* The vectored handler is registered LAST, as `stack.c`'s is and for its
 * reason: a library that uses structured exceptions sees the fault first. None
 * of the three changes what the process does. There is no frame walk here —
 * `stack.c`'s Windows arm has none either, and its own comment says why — so the
 * Windows line names the count and not the function. An ignored SIGABRT stays
 * ignored, for the POSIX arm's reason (defect 089). */
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
#include <sanitizer/common_interface_defs.h>
static void hero_lease_sanitizer_spoke_win(void) { hero_runtime_spoke = 1; }
#endif

static void hero_lease_crash_install(void) {
    static int done = 0;
    if (done) return;
    done = 1;
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
    __sanitizer_set_death_callback(hero_lease_sanitizer_spoke_win);
#endif
    AddVectoredExceptionHandler(0, hero_lease_veh);
    hero_lease_prev_filter = SetUnhandledExceptionFilter(hero_lease_last_chance);
    hero_lease_prev_abrt_win = signal(SIGABRT, hero_lease_abrt_win);
    if (hero_lease_prev_abrt_win == SIG_IGN) signal(SIGABRT, SIG_IGN);
}
#else
/* One saved disposition per signal this handler takes. SIGILL joined SIGTRAP
 * and SIGABRT at panel 175 (defect 081): `__builtin_trap()` is `brk` on arm64,
 * which is SIGTRAP, and `ud2` on x86-64, which is SIGILL, so a C trap with a
 * lease live died at 132 with nothing on the x86-64 leg while the arm64 leg
 * named the lease. */
static struct sigaction hero_lease_prev_trap;
static struct sigaction hero_lease_prev_abrt;
static struct sigaction hero_lease_prev_ill;

static struct sigaction *hero_lease_prev_for(int signum) {
    if (signum == SIGTRAP) return &hero_lease_prev_trap;
    if (signum == SIGILL) return &hero_lease_prev_ill;
    return &hero_lease_prev_abrt;
}

static const char *hero_lease_signal_name(int signum) {
    if (signum == SIGTRAP) return "SIGTRAP";
    if (signum == SIGILL) return "SIGILL";
    return "SIGABRT";
}

static void hero_lease_say_count(long long v) {
    char buf[24];
    int at = (int)sizeof buf;
    buf[--at] = '\0';
    if (v == 0) buf[--at] = '0';
    while (v > 0 && at > 0) {
        buf[--at] = (char)('0' + (v % 10));
        v /= 10;
    }
    hero_stack_say(buf + at);
}

/* Who raised the signal, from `siginfo_t` and nothing else. `si_code <= 0` is a
 * signal sent by a process — `abort`, `raise` and `kill` all land here — and
 * then `si_pid` says which; `si_code > 0` is the kernel's, a trap instruction,
 * and `si_pid` then overlays `si_addr` and means nothing, which panel 175's
 * ffi seat measured reading -1118630236 on Linux. */
static void hero_lease_say_origin(int signum, const siginfo_t *si) {
    hero_stack_say(hero_lease_signal_name(signum));
    if (si == NULL) return;
    if (si->si_code <= 0 && si->si_pid == getpid()) {
        hero_stack_say(", raised by this process");
    } else if (si->si_code <= 0 && si->si_pid != 0) {
        hero_stack_say(", sent by process ");
        hero_lease_say_count((long long)si->si_pid);
    } else if (si->si_code > 0) {
        hero_stack_say(", at a trap instruction");
    }
}

/* PANEL 175 CHANGED THE ORDER AND THE WORD, and each change is a defect that
 * shipped in panel 173's line.
 *
 * - THE HANDLER FOUND BEFORE US RUNS FIRST (defect 082). This used to speak and
 *   then chain, so a C library that recovers from its own signal — a
 *   `siglongjmp` out of `abort` — left *the process died* above a process that
 *   went on and exited 0, measured on all three POSIX legs. Now the line is
 *   written only once that handler has RETURNED, and a handler that recovers
 *   never comes back here. What this gives up is a handler that ends the
 *   process itself with `_exit`: then nothing is said, which is silence and not
 *   a falsehood.
 * - IT SAYS *UNDER*, NEVER *IN* (defect 080). The frame walk names the nearest
 *   Heroes frame on the stack, and a noreturn call inside an optimised C library
 *   saves no return address, so the walk skips the direct caller: the line said
 *   *in e.main* for a lease and a call in `e.run`, on Darwin and Linux arm64.
 *   The named function is on the stack; that it made the call is not known.
 * - IT SPEAKS WITH NO LEASE LIVE (the milestone's first item). A pointer C made
 *   and freed twice through a `ptr` died with zero bytes on Darwin, where
 *   libmalloc writes its reason only into a crash report, and on Windows. The
 *   line states the signal, who raised it and that this runtime did not, and
 *   names C's usual reasons as usual. It never names a cause, and it never
 *   repeats E2's third sentence, which F1 and F3 measured false. */
static void hero_lease_crash(int signum, siginfo_t *si, void *ctx) {
    struct sigaction *prev = hero_lease_prev_for(signum);
    if ((prev->sa_flags & SA_SIGINFO) != 0 && prev->sa_sigaction != NULL) {
        prev->sa_sigaction(signum, si, ctx);
    } else if (prev->sa_handler != SIG_DFL && prev->sa_handler != SIG_IGN && prev->sa_handler != NULL) {
        prev->sa_handler(signum);
    }
    if (!hero_runtime_spoke) {
        int64_t held = hero_live_held;
        uintptr_t pc, fp, sp, lr;
        hero_stack_regs(ctx, &pc, &fp, &sp, &lr);
        const char *who = hero_stack_blame(pc, fp, lr);
        /* Only a HEROES frame is named. When the walk finds none it hands back
         * the symbol the signal arrived in, which serves stack.c's own report
         * and would here present `kill` as if the program had a function by that
         * name — measured on Linux x86-64 at panel 175's landing. */
        if (who != NULL && !hero_stack_is_heroes(who)) who = NULL;
        if (held > 0) {
            hero_stack_say("panic: the process died with ");
            hero_lease_say_count((long long)held);
            hero_stack_say(" lease(s) still live");
            if (who != NULL) {
                hero_stack_say(", under ");
                hero_stack_say_heroes_name(who);
            }
            hero_stack_say("\n  `end_lease` is the only thing that may free a `.lease()`. If one reached a C\n"
                           "  function that frees what it is handed, that is this death; if not, this says\n"
                           "  only what was live when the process ended.\n");
        } else {
            hero_stack_say("panic: the process is dying of ");
            hero_lease_say_origin(signum, si);
            if (who != NULL) {
                hero_stack_say(", under ");
                hero_stack_say_heroes_name(who);
            }
            hero_stack_say(", and this runtime did not raise it.\n"
                           "  A C library ends a program this way when it catches misuse, memory given back\n"
                           "  twice among them; a reason C printed above, if any, is its own.\n");
        }
    }
    struct sigaction dfl;
    memset(&dfl, 0, sizeof dfl);
    dfl.sa_handler = SIG_DFL;
    sigaction(signum, &dfl, NULL);
    raise(signum);
}

/* UNDER THE SANITIZER THE HANDLER STAYS, AND IS SILENCED ONCE ASAN HAS SPOKEN
 * (panel 175, item 4). Until then it was compiled out, for panel 173's reason: a
 * line appended under ASan's own report is redundant when right and false when
 * ASan's reason was not a free. But compiled out, it was silent exactly where
 * ASan is silent too — a C `abort`, a failed C `assert`, a trap — so no golden
 * could pin those deaths at `--sanitize` on any platform (panel 175's critic,
 * five of seven cases red). ASan calls its death callback before it ends the
 * process, and this one raises the runtime's own flag, so nothing is said after
 * a report. Measured: Darwin runs the callback and then aborts into this
 * handler, which stays silent; both Linux legs `_exit(1)` after a report and
 * never reach it; with no report, all three say their line. Setting ASan's
 * `handle_abort` instead was measured wrong: every runtime panic is SIGABRT, and
 * a plain index panic gained 1807 bytes of ASan report. One callback per
 * process is ASan's rule, so C that installs its own after this one wins. */
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
#include <sanitizer/common_interface_defs.h>
static void hero_lease_sanitizer_spoke(void) { hero_runtime_spoke = 1; }
#endif

/* AN IGNORED SIGNAL STAYS IGNORED (defect 089). Panel 173's install took the
 * three signals whatever it found, so a C library that had set SIGABRT to be
 * ignored and then raised it — a program that goes on, exit 0 — was killed at
 * 134 under a false lease line, on every platform since that handler shipped. A
 * signal found ignored is put back and not taken: `abort` still ends the
 * process, since it overrides an ignore, and says nothing, which is silence and
 * not a falsehood. */
static void hero_lease_take(int signum, const struct sigaction *sa, struct sigaction *prev) {
    if (sigaction(signum, sa, prev) != 0) return;
    if ((prev->sa_flags & SA_SIGINFO) == 0 && prev->sa_handler == SIG_IGN) sigaction(signum, prev, NULL);
}

/* Three `sigaction` calls, once per process, after the stack guard so the
 * alternate stack they run on exists: the disposition is the process's and the
 * stack is the thread's (stack.c, "ONCE PER PROCESS"). */
static void hero_lease_crash_install(void) {
    static int done = 0;
    if (done) return;
    done = 1;
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
    __sanitizer_set_death_callback(hero_lease_sanitizer_spoke);
#endif
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = hero_lease_crash;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    hero_lease_take(SIGTRAP, &sa, &hero_lease_prev_trap);
    hero_lease_take(SIGABRT, &sa, &hero_lease_prev_abrt);
    hero_lease_take(SIGILL, &sa, &hero_lease_prev_ill);
}
#endif

void hero_args_set(int argc, char **argv) {
    hero_stdout_is_bytes();
    hero_streams_survive_abort();
    /* The stack guard goes up here, before a program's first line, because
     * this is the one call every generated `main` makes first — so the
     * emitted C and the ABI stamp are untouched (panel 104). */
    hero_stack_guard_install();
    /* And the lease handler, after the guard whose alternate stack it runs on
     * (panel 172 R1; the mechanism panel 173 judged). */
    hero_lease_crash_install();
    /* And the thread itself, for the same reason and by the same precedent
     * (panel 111 R9): this is the thread a Heroes program owns, so a callback
     * entered from any other one is C calling back from a thread of its own. */
    hero_thread_claim();
    /* And the stack this thread runs on, which becomes the floor under every
     * thread the runtime starts (panel 115). Measured here for the same reason
     * as the two above: this is the thread the program owns, and by the time a
     * worker spawns a worker the calling thread is the wrong reference. */
    hero_spawn_measure_home();
    hero_argc = argc;
    hero_argv = argv;
}

/* argv[0] is the program's own name, and `args()` promises "the arguments after
 * the program name" — the off-by-one that every language's first CLI program
 * gets wrong, answered once, here. */
int64_t hero_args_count(void) {
    return hero_argc > 0 ? (int64_t)hero_argc - 1 : 0;
}

HeroStr hero_args_at(int64_t index) {
    if (index < 0 || index >= hero_args_count()) {
        hero_panic("argument index out of range");
    }
    return hero_str_from_cstr(hero_argv[index + 1]);
}

/* The same argument, BORROWED and unconverted — the one input a program cannot
 * decline to receive, handed over as bytes so the program can decide (panel 089's
 * ffi-pragmatist, queued there and landed 2026-08-24).
 *
 * `hero_args_at` above converts EAGERLY, which is why no `cstr` ever reaches a
 * Heroes program and why `validated` — which rescues every other C string in the
 * language — cannot rescue argv. Measured before this existed: `./prog $'\xff\xfe'`
 * was `panic: hero_str_from_bytes: not well-formed UTF-8`, exit 134, for an
 * argument the shell chose and the program never asked for.
 *
 * **Out of range is NULL and not a panic**, and that is the whole difference from
 * its neighbour: a caller that must branch cannot branch on a panic. `args_at`
 * keeps its abort because an out-of-range index is the program's own mistake
 * (§4.9's rule for every index in this language); this one is reached only by the
 * library's own loop, which cannot go out of range, and NULL is what makes it
 * checkable rather than fatal if it ever does.
 *
 * Borrowed, not owned: the bytes belong to `main`'s argv and outlive every
 * Heroes value built from them. Nothing here allocates, so nothing here can leak;
 * the copy happens in `validated`, through the one allocation point §4.20
 * insists on. */
const char *hero_args_raw(int64_t index) {
    if (index < 0 || index >= hero_args_count()) {
        return NULL;
    }
    return hero_argv[index + 1];
}

/* THE OPERATING SYSTEM'S OWN REASON FOR THE LAST FILESYSTEM FAILURE ON THIS
 * THREAD, `hero_run_why`'s counterpart for the files a build writes (defect
 * 134). A build that could not publish an object said *cannot publish* and no
 * more, where the number is the whole difference between a full disk, a
 * read-only directory and a file another process holds open: three repairs,
 * one silence.
 *
 * `errno` where a C call failed, `GetLastError()` where a Win32 one did; 0
 * after a call that succeeded, because each call below clears it on the way
 * in. Defined here rather than in `parts/fs.c` because this part is included
 * first and `hero_file_write` below sets it too. Thread-local for the reason
 * `hero_run_why_code` is: two threads' failures are two answers. */
#include <errno.h>
static _Thread_local int64_t hero_fs_why_code = 0;

int64_t hero_fs_why(void) {
    return hero_fs_why_code;
}

/* The whole file, read to its end.
 *
 * A binary read (`"rb"`), because a `str` is bytes: §4.3 measures and indexes a
 * string in bytes, so translating CRLF here would make `len` disagree with the
 * file on one platform and not the other.
 *
 * **Read to the end, never to a size asked first** (defect 273, 2026-10-04,
 * found by batch 8's Linux arm64 leg on defect 236's test). The size came from
 * `fseek(SEEK_END)` and `ftell`, which answer for a regular file and for
 * nothing else: a directory opens with `fopen` on Linux and on this Mac, and on
 * Linux its end is an offset near 2^63, so the buffer asked for that and
 * `read_file` died *out of memory* where it owes `.err`; a file under `/proc`
 * says 0 and read as empty. So the open file is asked what it is: a directory
 * is FAILED before a byte is read, a regular file's size is only the first
 * capacity, and the read goes on until `fread` meets the end, the buffer
 * doubling with its product guarded, as §4.20 asks of every length. */
#if defined(_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#endif

/* 1 for a directory, -1 where the descriptor cannot be described, 0 otherwise,
 * with `*first_room` the capacity to start from: one byte past a regular file's
 * stated size, so a file read whole meets its end in the same call, and 4096
 * where nothing states a size. Asked of the descriptor rather than the path, so
 * the answer is about the file that was opened. */
static int hero_file_kind(FILE *file, size_t *first_room) {
    *first_room = 4096;
#if defined(_WIN32)
    struct _stat64 info;
    if (_fstat64(_fileno(file), &info) != 0) return -1;
    if ((info.st_mode & _S_IFMT) == _S_IFDIR) return 1;
    int regular = (info.st_mode & _S_IFMT) == _S_IFREG;
#else
    struct stat info;
    if (fstat(fileno(file), &info) != 0) return -1;
    if (S_ISDIR(info.st_mode)) return 1;
    int regular = S_ISREG(info.st_mode);
#endif
    if (regular && info.st_size > 0 && (uint64_t)info.st_size < (uint64_t)(SIZE_MAX / 2))
        *first_room = (size_t)info.st_size + 1;
    return 0;
}
/* **The runtime does nothing to a path, and that is the portability** (recorded
 * 2026-08-14, after a day in which four separate things broke on Windows over path
 * separators and none of them was here).
 *
 * There is no `strcat`, no separator literal, no normalisation: the string the
 * program wrote reaches `fopen` verbatim. Windows accepts `/` in every filesystem
 * API — it is `cmd.exe`, not the OS, that reads a leading `/` as a switch — so a
 * program writing `read_file("data/in.txt")` works on all three platforms, and
 * `examples/adventure/main.hero` proves it on the Windows CI leg with a literal
 * `examples/adventure/walkthrough.txt`.
 *
 * **Written as a claim that can die**: if a platform is ever added whose `fopen`
 * refuses `/`, this comment is wrong and the corpus is what says so — the adventure
 * example fails on that leg and nowhere else. Until then, adding separator handling
 * here would be the repair that creates the defect.
 *
 * Both modes are binary (`rb`/`wb`) for the sibling reason: text mode on Windows
 * translates newlines, and a language whose `str` is measured in bytes cannot have
 * a file grow one byte per line on one platform. */
HeroStr hero_file_read(const char *path, int64_t *status) {
    /* NOT_FOUND only where nothing is at the path (defect 274, 2026-10-04): a
     * file of mode 000 was `file_not_found`, *no file at* a path where a file
     * is, and Windows' `fopen` of a directory fails EACCES. `ENOTDIR` is a path
     * through a file, where nothing can be. */
    errno = 0;
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        *status = (errno == ENOENT || errno == ENOTDIR) ? HERO_OS_NOT_FOUND : HERO_OS_FAILED;
        return hero_str_from_bytes("", 0);
    }
    size_t room = 0;
    if (hero_file_kind(file, &room) != 0) {
        fclose(file);
        *status = HERO_OS_FAILED;
        return hero_str_from_bytes("", 0);
    }
    char *buffer = hero_alloc(room);
    size_t got = 0;
    for (;;) {
        if (got == room) {
            if (room > SIZE_MAX / 2) {
                hero_release(buffer);
                fclose(file);
                *status = HERO_OS_FAILED;
                return hero_str_from_bytes("", 0);
            }
            char *grown = hero_alloc(room * 2);
            memcpy(grown, buffer, got);
            hero_release(buffer);
            buffer = grown;
            room = room * 2;
        }
        size_t want = room - got;
        size_t n = fread(buffer + got, 1, want, file);
        got += n;
        if (n < want) break;
    }
    int failed = ferror(file);
    fclose(file);
    if (failed || got > (size_t)INT64_MAX) {
        hero_release(buffer);
        *status = HERO_OS_FAILED;
        return hero_str_from_bytes("", 0);
    }
    /* **The bytes are not text, and that is a value rather than a death** (panel
     * 087). `hero_str_from_bytes` aborts on ill-formed UTF-8, and that abort is
     * load-bearing everywhere else — `hero_str_chars` takes one byte off an
     * invalid sequence and depends on it for the language-wide well-formedness
     * invariant — so the repair is a pre-check at the *caller*, never a weaker
     * conversion. Asking first is what turns the fourth program state into a
     * `status` the wrapper can translate.
     *
     * Before this, `read_file` was typed `-> str?` (spec § 11 Built-ins) and killed the
     * process on a photograph: a program handling both `.ok` and `.err` reached
     * neither arm. The Rust bootstrap's own record walk survived the same input
     * because `read_to_string` returns `Err`, and the Heroes port transcribed
     * that guard faithfully to `suite_records.hero:148` — where it could never
     * fire. defect 002 in `docs/work/DONE.md` is the record. */
    if (!hero_utf8_valid(buffer, (int64_t)got)) {
        hero_release(buffer);
        *status = HERO_OS_NOT_TEXT;
        return hero_str_from_bytes("", 0);
    }
    HeroStr text = hero_str_from_bytes(buffer, (int64_t)got);
    hero_release(buffer);
    *status = HERO_OS_OK;
    return text;
}

int64_t hero_file_write(const char *path, HeroStr text) {
    hero_fs_why_code = 0;
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_OS_FAILED;
    }
    int64_t len = hero_str_len(text);
    if (len > 0) {
        size_t put = fwrite(hero_str_cstr(text), 1, (size_t)len, file);
        if (put != (size_t)len) {
            hero_fs_why_code = (int64_t)errno;
            fclose(file);
            return HERO_OS_FAILED;
        }
    }
    /* A full disk is often reported here and not at `fwrite`: the bytes sat in
     * the stream's buffer until the close tried to write them. */
    if (fclose(file) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_OS_FAILED;
    }
    return HERO_OS_OK;
}

/* The error stream, written whole. No status, and that absence is the point.
 *
 * WHY IT IS HERE RATHER THAN A POSIX BINDING IN THE COMPILER (author decision
 * 2026-08-24). `selfhost/cli_io.hero` reached `write(2)` through
 * `extern "unistd.h"`, so `seed/heroes.c` `#include`d that header and the
 * self-hosted compiler could not be built on a platform without POSIX headers —
 * measured on the archive's own CI leg, which had been green while testing a
 * compiler that did not have the problem. `fwrite` to `stderr` is C89 and needs
 * no POSIX at all, and putting it here keeps the platform question in the one
 * file in this project that is allowed to know what machine it is on. It also
 * removes the seed's only POSIX include.
 *
 * WHY IT RETURNS NOTHING. `write(2)` may write fewer bytes than it was given,
 * and two of the compiler's three call sites discarded the count — a §1.12 hole
 * where a truncated diagnostic would look like a complete one. The loop below is
 * the answer: this writes all of it or the process has no error channel left, so
 * there is no count for a caller to ignore.
 *
 * NO `fflush`, AND THAT IS MEASURED RATHER THAN ASSUMED. C11 7.21.3p7 says the
 * standard error stream is not fully buffered, and the case that matters was run
 * before this was written: `fwrite(m, 1, n, stderr)` followed by `abort()`
 * delivers the text to a file **and** through a pipe, exit 134 (2026-08-23, the
 * measurement in this decision's queue item). The instrument that catches a
 * regression already exists and is large: every `tests/golden/check/` case
 * compares the diagnostic text that leaves through here byte for byte — 65
 * `check` plus 76 `annotations` checks in the net — so a truncating write is 141
 * red checks rather than a silent short line. */
void hero_write_err(HeroStr text) {
    int64_t len = hero_str_len(text);
    if (len <= 0) {
        return;
    }
    hero_stdout_is_bytes();
    const char *bytes = hero_str_cstr(text);
    size_t left = (size_t)len;
    while (left > 0) {
        size_t put = fwrite(bytes, 1, left, stderr);
        if (put == 0) {
            /* Nowhere left to say so: a CLI whose error channel has failed
             * cannot report that its error channel has failed. */
            return;
        }
        bytes += put;
        left -= put;
    }
}

/* The truncation is deliberate and is the reason `exit` is not a plain `extern`:
 * C's `exit` takes an `i64`, Heroes' `i64` is `int64_t`, and binding one to the
 * other is `conflicting types for 'exit'` (panel 030 R3, reproduced on clang 21).
 * A shell reads the low 8 bits anyway. */
_Noreturn void hero_exit(int64_t code) {
    exit((int)code);
}

/* `sizeof` rather than a preprocessor guess about the platform: the question is
 * what this compiler does with `long` on this target, and the compiler is the
 * only thing that knows. CHAR_BIT is 8 everywhere clang runs, and multiplying by
 * it rather than by 8 says which assumption is being made. */
int64_t hero_word_bits(void) {
    return (int64_t)(sizeof(long) * CHAR_BIT);
}
