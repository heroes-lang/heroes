# Panel 247, ffi-pragmatist's report

Written as the sitting goes, started 2026-11-13 09:27 CET (`date`). Every number carries the command
that produced it; every cost is instructions retired (`/usr/bin/time -l`), never seconds (load 17.33
at 09:27, `uptime`, a session commenting on issues beside it). My folder is
`.claude/worktrees/p247-scratch/247-ffi-pragmatist/` (below `F/`); every probe and log named here is
in it. **Status: complete** (11:00 CET; the coordinator's session restarted at about 10:15 and I resumed
where I stopped, nothing of mine left running, `pgrep -f p247-scratch/247-ffi-pragmatist`: exit 1).

## 0. Setup, as run

- `git -C .claude/worktrees/b34-round rev-parse ae5e2cedd 'ae5e2cedd^{tree}' 932876f95 932876f95^`:
  `ae5e2cedddee93e0d53715a5b5ae6d7833c569df`, `3e22d53467b606946a70a6f9dfdea6f46ea47612`,
  `932876f9585817ec888d1ba875c880f2f1a4b3e3`, and as D's parent the base. `git archive` of the base
  into `F/tree`; `git check-ignore -v`: `.gitignore:34:.claude/worktrees/`. `TMPDIR=F/tmp` for every
  build and run.
- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0. Stage 2: `./heroes build
  selfhost/main.hero -o heroes-next`, exit 0, 890,864,591,347 instructions (`F/stage2-base.log`);
  `--version`: `heroes 0.6.0`, **`sources 7cc70d136b590d46`**, the base. Kept as `tree/heroes-base`.
- D: `git diff ae5e2cedd 932876f95 -- runtime/parts/alloc.c > F/d.patch` (238 lines), `patch -p1`,
  exit 0; `shasum -a 256 runtime/parts/alloc.c` = `git show 932876f95:runtime/parts/alloc.c | shasum -a
  256` = `5bea9640...`. Stage 2 again, exit 0, 891,086,776,167 instructions (`F/stage2-d.log`);
  **`sources 560c95206c908459`**, the brief's line; `heroes-d doctor`: `ok runtime runtime` (the
  copy's own). Kept as `tree/heroes-d`.
- Route C, to test it where its author did not run it: `F/tree-c`, an APFS clone of `F/tree` (base and
  D) with the compiler-engineer's nine changed files copied in (`diff -rq` against its tree's `runtime/`
  and `selfhost/` afterwards: no difference), its `thread.c` at `HERO_SIGNAL_DEFER 0`,
  `HERO_SIGNAL_WALK 8`. Built in my folder, never in its.
- **Where the brief is wrong or has moved**: the round's head is now `09685b1bc` (`git -C
  .claude/worktrees/b34-round rev-parse HEAD` at 09:27), past the `2a3a37fc3` the briefs read; the
  runtime's files are D's and the round's as the brief lists, unchanged by that move for this seat's
  purpose (not diffed further). My brief says the coordinator runs the Linux leg; the coordinator's
  message asks me to run it with my tree copied in, and Docker is up (`docker images`:
  `heroes-linux-arm64:latest` `338a1044ac90`), so I run it myself.

## 1. The delivery's return address, run (brief item 1)

**Darwin arm64, `F/c/retaddr.c`** (`clang -std=gnu11 -O2 -Wall -Wextra -pthread`, exit 0, no warning;
run under `timeout -s KILL 60`, exit 0; `F/logs/retaddr-darwin.txt`). The compiler-engineer's probe,
line for line, learns **one address for both handler kinds**: `_sigtramp+56` in
`/usr/lib/system/libsystem_platform.dylib` (`dladdr`). Then a SIGUSR1 handler records
`__builtin_return_address(0)` at every delivery:

| how the signal lands | plain | `SA_SIGINFO` | with `SA_ONSTACK` (an alternate stack) |
|---|---|---|---|
| `raise`, 1000 times | 1000, all `_sigtramp+56` | 1000, all | 1000 and 1000, all |
| sent by a thread while the target spins | 14,111, all | 16,662, all | 15,801 and 18,153, all |
| the target blocked in `read(2)` | 11,349, all | 10,632, all | 10,185 and 11,744, all |
| the target in `nanosleep` | 8,457, all | 7,423, all | 9,661 and 8,129, all |
| the target in a `malloc`/`free` loop | 13,781, all | 14,355, all | 15,223 and 14,909, all |
| sent by another process (`kill`, process-directed) | 602, all | 619, all | 611 and 642, all |
| on a worker thread | 25,908, all | 26,745, all | 20,547 and 29,597, all |
| nested once under `SA_NODEFER` | 2000, all | | 2000 (info, onstack), all |
| **the program calling its own handler** | 1, **not** `_sigtramp` (`scenario+1720`) | 1, not | 1 and 1, not |

So on Darwin arm64 the exact test is exact for a handler the delivery calls: 315,846
deliveries (summed from the log by `awk`), one address, the alternate stack and `SA_SIGINFO` included, and a call that is not a
delivery never matches.

**The walk, Darwin arm64, `F/c/walk_main.c` with `F/c/walk_h.c`** (the program's own C handlers, in a unit
built apart; `F/logs/walk-darwin.txt`). `check` is the compiler-engineer's `hero_callback_entered` line
for line (exact test, then at most 8 frame records inside the thread's stack and one known alternate
stack), answering where it found the delivery instead of refusing:

| the handler's way to the thunk | handler unit `-O0` | `-O2` | **`-O2 -fomit-frame-pointer`** |
|---|---|---|---|
| the thunk is the handler | exact | exact | exact |
| a C handler calls it | walk 1 | walk 1 | **missed** |
| through `qsort` (8 values) | walk 2, 18 of 18 calls | walk 2 | **missed, 0 of 18** |
| through `qsort` (10,000 values) | first call walk 2; **423 of 134,665 calls missed** (deeper than 8) | the same | **missed, 0 of 134,665** |
| through `bsearch` | walk 2 | walk 2 | **missed** |
| through `fork`'s `pthread_atfork` prepare | walk 4 | walk 4 | **missed** |
| through `exit`'s `atexit` | walk 3 | walk 3 | **missed** |
| a C handler, on an alternate stack the walk knows | walk 1 | walk 1 | **missed** |
| **a C handler, on an alternate stack the program made** | **missed** | **missed** | **missed** |
| the thunk is the handler, on that alternate stack | exact | exact | exact |
| a C handler, sent from another thread 150 times | 150 of 150 | 150 of 150 | **0 of 150** |

Darwin's clang honours `-fomit-frame-pointer` on arm64 (`objdump -d`: the handler saves x29 and x30 but
never writes `add x29, sp, #0x10`), so **a handler unit without frame pointers hides the delivery from
the walk at every depth**, and an alternate stack the runtime did not make stops it before its first
step.

## 2. Every refusal async-signal-safe, and the status it ends with (brief item 2)

`F/c/refuse.c` and `F/c/refuse.py` (`clang -O0 -g -pthread`, 5 runs a cell, Python's `returncode`,
negative for a death by that signal; `F/logs/refuse-darwin.txt`). The handler writes its line by
`write(2)` and ends three ways: **raise**, D's and the compiler-engineer's (SIGABRT's default
restored and let through, `raise(SIGABRT)`, `_exit(134)` if it returns); **kill**, the same with
`kill(getpid(), SIGABRT)`; **abort**, `abort()`.

| where the handler runs | raise | kill | abort |
|---|---|---|---|
| `main` | -6 x5 | -6 x5 | -6 x5 |
| a worker | -6 x5 | **134 x5** (the worker reached `_exit` before the process-directed signal landed) | -6 x5 |
| a thread whose thread-specific data is being destroyed, the refusal's code run there | **134 x5** (`raise` returned: `pthread_kill` on that thread answers ESRCH) | 134 x5 | **-5 x5, SIGTRAP** |

So on Darwin the shape D landed and route C's prototype copies is the right one: SIGABRT where a
signal can be raised, `_exit(134)` where it cannot, both a panic to the harness (nonzero, words
present, `expectation.hero:178-200` as the compiler-engineer read it), never the SIGTRAP `abort()`
gives on an ending thread (the crash reports 00-shared.md cites, reproduced in plain C). A signal
SENT to that ending thread is never delivered on Darwin: `pthread_kill` answered `No such process`
and a process-directed one stayed pending (`ending` rows: exit 0, *"not delivered on the ending
thread"*).

## 3. The class, from the C side (brief item 3)

Counted in the base runtime, comment lines dropped (`grep -hE "(^|[^A-Za-z0-9_])<fn>\(" parts/*.c |
grep -vE '^\s*(\*|//|/\*)' | wc -l` in `F/tree/runtime`), the C calls off POSIX's async-signal-safe
list that a Heroes body reaches through the runtime: `snprintf` **31** code lines in 9 files, every
integer by `"%lld"` (`parts/f64.c:269`, `"%llu"` at 281) and every float by `"%.*g"`
(`parts/f64.c:87`); `print` by `fwrite(s.ptr, 1, len, stdout)` (`parts/str.c:349`), `fflush` **24**,
`fprintf` **18**, `fputs` 3, `fwrite` 4, `printf` 2, `putchar` 1; `malloc` 1, `calloc` 1, `free` **11**
(D brackets 11 of these 13, its comment naming the two frees left outside at a thread's end);
`exit` 3 (`hero_exit`, `parts/os.c:1010`); `pthread_mutex_lock` **8** (the handle table's at
`parts/alloc.c:156`); `strtod` 1, `getenv` 3, `qsort` 1. D sees the 11 bracketed allocator calls and
nothing else on that list.

**The class is the C library's, not the Heroes runtime's: plain C, no Heroes anywhere, measured on
Darwin** (`F/c/reent.c`, one binary per mode, the shared driver `storm.py <binary> SIGUSR1 4 0.3`,
2 runs each, `F/logs/reent-darwin.txt`, crash reports `F/logs/reent-darwin-ips.txt`):

| a C handler that re-enters, while `main` does the same | outcome | crash report |
|---|---|---|
| `snprintf("%lld")` | **2 of 2 `exit -9`, nothing said** | 2 SIGKILL, `_os_unfair_lock_recursive_abort < ... < localeconv_l < __vfprintf < _vsnprintf < snprintf < on_signal < _sigtramp < localeconv_l < __vfprintf < _vsnprintf < snprintf < main` |
| `snprintf("%.*g")` | **2 of 2 `exit -9`** | 2 SIGKILL, the same lock |
| `fwrite` to one `FILE` | **2 of 2 hung**, killed 30 s after the storm by the driver | none; `sample` of a third hung process: `fwrite < funlockfile < _sigtramp < on_signal < fwrite < flockfile < _pthread_mutex_firstfit_lock_slow < __psynch_mutexwait`, waiting for ever on the lock `main` held |
| `malloc`/`free`, 16 to 79 bytes and 2 to 4 KB | 4 of 4 exit 0, every check good (about 1.2 million handler runs each) | none: a sample, POSIX calls it undefined |

So the remedy D's words give, *"a handler written in C"*, is itself in the class unless that C is
async-signal-safe: a C handler that formats a number dies like a Heroes one.

## 4. Route C as built, its holes, run on Darwin (with D under it)

`F/tree-c/heroes-c` (route C, walk 8) against `F/tree/heroes-d` (D alone); every case built at exit 0
(`F/logs/build-*.log`), each run under `timeout -s KILL 30` (`F/logs/single-darwin.txt`); `pgrep -fl
247-ffi-pragmatist/h/` after each batch: nothing.

| case (`F/h/`) | how Heroes code is reached | D alone | route C |
|---|---|---|---|
| `wrap`, `wraponstack`, `infocase` | the program's own `sigaction` hands the Heroes function to the delivery | ran inside the signal, exit 0 | **refused, exact test**, exit 134 |
| `fcase` | the program's own C handler calls it | ran, exit 0 | **refused, walk** |
| `flib-fp`: a library of the program's (`libsigcb.a`, built `-O2`) whose handler calls the program's callback | ran | **refused, walk** |
| **`flib-nofp`: the same library built `-O2 -fomit-frame-pointer`** | ran | **ran inside the signal: `answer ran for 30`, `60`, exit 0** |
| **`fown`: the program's own C handler on an alternate stack the program made** | ran | **ran inside the signal, exit 0** |
| `fexit`: the program's C handler calls `exit()`, which runs a Heroes `atexit` function (spec § 13's own example) | ran: `the atexit function ran: 42` | **refused, walk** |
| `ffork`: the program's C handler calls `fork()`, which runs a Heroes `pthread_atfork` prepare function | ran: `prepare ran: 42` | **refused, walk** |
| `chandler`: a C handler handed to `signal` from Heroes | `1`, exit 0 | `1`, exit 0 |

**And under a storm, the two holes die in silence under route C as built**
(`storm.py <binary> SIGUSR1 4 0.3`, 3 runs each, `F/logs/storm-holes-darwin.txt`; `ips.py`):
`c-fownlocale` and `c-fliblocale` (the Heroes function makes one C call, `localeconv`, and builds no
value, while `main` formats integers) **6 of 6 `exit -9`, nothing on stderr; 6 SIGKILL reports, the
locale lock**: `localeconv_l < h_fownlocale_answer < h_0cb_fownlocale_answer < fown_handler <
_sigtramp < localeconv_l < __vfprintf < ... < hero_int_to_str < h_fownlocale_round`, and for the
library `... < h_0cb_fliblocale_answer < sigcb_handler < localeconv_l < ...`, where the crash
reporter's own unwind skips `_sigtramp` for the reason the walk cannot see it.

## 5. Two repairs of route C's holes, built in C and run on Darwin

**The alternate stack, closed exactly by asking the kernel.** Where the thunk's frame lies outside the
thread's own stack, one `sigaltstack(NULL, &now)` says whether the thread runs on an alternate signal
stack (`SS_ONSTACK`), where only a delivery puts it: POSIX's `sigaltstack` page defines the flag as the
process *"currently executing on the alternate signal stack"* (a reading of the standard's wording,
not fetched today). Built into `F/c/walk_main.c` as `-DONSTACK_TEST=1` (`F/logs/walk-onstack-darwin.txt`):
**every alternate-stack row of § 1's table answers `onstack`**, the stack the program made and the
handler unit built `-O2 -fomit-frame-pointer` included, at all three flag sets. Its cost falls only on
a call whose frame is off the thread's stack (a syscall there), never on an ordinary callback; under
`--sanitize`, where the compiler-engineer found the bounds empty, the bounds must be read from
`pthread_get_stackaddr_np` / `pthread_getattr_np` first or every callback pays the syscall (unbuilt).
In the runtime it would replace the walk's alternate-stack bound; `fown` and `fownlocale` are its cases.

**Interposition, ThreadSanitizer's way (the historian's finding 8), built: `F/c/ipose.c`.** The
executable defines `sigaction`, `signal` and `sigset`; each puts the program's handler behind one of
the runtime's own, which counts a thread-local depth around the call, and a query hands back the
program's own handler. The thunk then asks one thread-local, with no frame record and no stack bound.
Run with the handler in a static library and in a shared library, both built `-O2
-fomit-frame-pointer` (`F/logs/ipose-darwin.txt`, plain and under `-fsanitize=address,undefined`, the
same answers):

| who installs the handler that reaches the thunk | Darwin |
|---|---|
| the program's own `sigaction`, the thunk the handler | seen inside a handler |
| the program's own `signal`, a C handler calling the thunk | seen |
| a static library without frame pointers, by `sigaction`, `signal` or `sigset` | **seen, 3 of 3** |
| a shared library without frame pointers, by any of the three | **missed, 3 of 3**: Darwin's two-level namespace binds a dylib's `sigaction` to libSystem's |
| `sigaction(sig, NULL, &old)` after an install | the program's own handler (after one repair of my prototype's query path) |

So interposition sees what the walk cannot (no frame pointers) and misses what the exact test never
misses (a dylib handing the thunk itself to the delivery is still the exact test's). Linux's answer
for a shared library is the container's (§ 7).

## 6. Windows, compiled with zig 0.15.2 and unrun (the box is offline)

**Microsoft's pages, fetched today** (`curl` into `F/pages/`, the text extracted by a script): the
`signal` page, *"SIGINT is not supported for any Win32 application. When a CTRL+C interrupt occurs,
Win32 operating systems generate a new thread to specifically handle that interrupt."*; *"Before the
specified function is executed, the value of func is set to SIG_DFL."*; *"The SIGILL and SIGTERM
signals aren't generated under Windows. They're included for ANSI compatibility"*; and what a handler
may not do: *"Don't issue low-level or STDIO.H I/O routines (for example, printf or fread). Don't
call heap routines or any routine that uses the heap routines (for example, malloc, _strdup, or
_putenv). ... Don't use any function that generates a system call (for example, _getcwd or time)."*
It names no thread for SIGABRT, SIGFPE or SIGSEGV (the historian's finding 1, confirmed). The
`HandlerRoutine` page: *"When the signal is received, the system creates a new thread in the process
to execute the function."*

So by Microsoft's own list **D's Windows stop is off it** (`_write`, a low-level I/O routine and a
system call, `parts/alloc.c:551-552`), as is any refusal that says why; the page leaves a Windows
handler nothing to say anything with. That is the page's rule, not a run.

**Built, unrun** (`F/win/`; `zig cc -target x86_64-windows-gnu` and `aarch64-windows-gnu`, `-std=gnu11
-Wall -Wextra -O2`, caches in `F/win/cache`: 4 of 4 exit 0, 0 warnings; `file`: PE32+ console x86-64
and Aarch64):

- `winprobe.c`, the box's questions: for `raise(SIGTERM)`, `raise(SIGFPE)`, `raise(SIGABRT)`, a real
  access violation, `abort()`, and Ctrl-C and Ctrl-Break through `signal` and `SetConsoleCtrlHandler`,
  the thread each handler runs on against `main`'s, its return address against a `raise(SIGTERM)`
  probe's, and whether `ntdll!KiUserExceptionDispatcher` stands among the frames above it
  (`CaptureStackBackTrace`, which unwinds by the images' tables and needs no frame pointer).
- `thread_win.c`, **the Windows arm route C would need**: the console's thread is already refused by
  `hero_thread_guard`'s first question (it was never claimed), on an ordinary thread, so that refusal
  may format; `raise`'s synchronous call is learned like the POSIX probe, by a SIGTERM probe; a fault's
  handler is seen by the dispatcher's frame. Its refusal writes with `WriteFile` and ends with
  `TerminateProcess(..., 134)`. Whether UCRT calls every handler from one site in `raise` (SIGFPE
  takes a second argument) is the box's first question; whether the dispatcher's frame is within 32
  is its second. **Until they are run, every Windows sentence of this sitting is a reading.**

## 7. Linux arm64, run in the container (`heroes-linux-arm64:latest`, `338a1044ac90`)

`F/linux-247.sh`, run as `docker run --rm -v "$PWD/$F":/src:ro heroes-linux-arm64:latest bash
/src/linux-247.sh 2>&1 | dd bs=1 count=4194304 2>/dev/null > "$F/logs/linux-247.log"` (docker exit 0;
Debian 13, glibc 2.41, clang 22.1.8); `F/linux-247b.sh` and `F/linux-247c.sh` the same way into
`linux-247b.log` and `linux-247c.log`. My two trees copied in, each compiler built there from its seed:
**D's stage 2 `sources 560c95206c908459`, route C's `sources 4521403569c5fe86`, the keys this Mac
built.**

- **The delivery's return address** (`retaddr.c`): both probes learn `__kernel_rt_sigreturn+0` in
  `linux-vdso.so.1`; every one of 2,001,938 deliveries (summed by `awk`) (raise, spin, `read`, `nanosleep`,
  `malloc`, another process, a worker, nested, each plain and `SA_SIGINFO`, with and without an
  alternate stack) returns there; the program's own call never does. **The exact test is exact on
  Linux arm64.**
- **The walk** (`walk_main.c` + `walk_h.c`): as on Darwin, every walk row missed with the handler's
  unit built `-O2 -fomit-frame-pointer`, and the program's own alternate stack missed at every flag
  set; glibc's own frames are crossed for `fork` (walk 3), `exit` (walk 3) and `bsearch` (walk 1 at
  `-O2`), but **through glibc's `qsort` of 10,000 values the FIRST comparator call is already missed,
  61,280 of 121,205 calls** (8 values: walk 5). A refusal by the walk there comes after Heroes code has
  already run inside the signal. **The `SS_ONSTACK` query answers every alternate-stack row, at all
  three flag sets**, as on Darwin. clang's default on this target keeps frame records at `-O2`
  (`stp x29, x30` in the probe's assembly); `-fomit-frame-pointer` removes them.
- **The refusal's status** (`refuse.c`): `raise` ends with SIGABRT, -6, on `main`, a worker and an
  ending thread alike (5 of 5 each); `abort()` -6 everywhere (no SIGTRAP on Linux); `kill(getpid())`
  gave -6 or 134 by a race. So D's shape ends -6 on Linux and -6 or 134 on Darwin, both a panic.
- **Question 7, glibc re-entered from a handler** (`reent.c`, storm 4 s, 2 runs each):

| a handler re-enters, while `main` does the same | plain glibc | ASan + UBSan | glibc's malloc check (`libc_malloc_debug.so`, `glibc.malloc.check=3`, live: a double free under it aborted, exit 134) |
|---|---|---|---|
| `snprintf("%lld")` | 2 of 2 exit 0; about 1.5 million handler calls each, **0 wrong on either side** | 1 of 1 exit 0, 0 wrong | |
| `snprintf("%.*g")` | 2 of 2 exit 0, about 2 million handler calls each, 0 wrong | 1 of 1, 0 wrong | |
| `fwrite` to one `FILE` | **2 of 2 corrupted in silence**: 50,456,847 and 48,962,103 lines where 30 million plus the handler's 2.2 million stand, 39,950,036 and 38,465,886 malformed, 21.6 and 21.7 million of `main`'s missing; glibc said nothing | **1 of 1 corrupted**, ASan said nothing | **1 of 1 corrupted** (46,742,288 lines, 37,126,129 malformed) |
| `malloc`/`free`, small and large | 4 of 4 exit 0, nothing found | **2 of 2 hung**, killed by the driver 30 s after the storm | 2 of 2 exit 0, nothing found |

**So the answer to question 7, measured**: where Darwin kills, glibc's integer and float formatting
re-entered from a handler was harmless in about 7 million checked calls (a sample; glibc's manual
marks `snprintf` *"AS-Unsafe heap"*, the historian's quote); **glibc's stdio re-entered corrupts in
silence**, its lock being recursive (Darwin's hangs instead); and the sanitizer's own allocator
deadlocks. None of the three is the Heroes runtime's: each is plain C.

- **Route C as built, in Linux** (the cases of § 4, `F/logs/linux-247.log` § 4b and 4c): the exact test
  refused `wrap`, `wraponstack`, `infocase`; the walk refused `fcase`, `flib-fp`, `fexit`, `ffork`;
  **`fown` and `flib-nofp` ran inside the signal, exit 0**; `chandler` ran, `1`. Storms: `storm` 3 of 3,
  `quiet` 3 of 3, `clocale` 2 of 2 refused with the words (-6); **`fownlocale` and `fliblocale` 6 of 6
  ran their Heroes function inside the signal through 1.8 to 2.0 million signals each, `built 20000000
  words`, exit 0** (glibc's `localeconv` has no lock to die on; R4 is broken in silence instead).
  **Under `--sanitize`, route C refused `wrap` alone: `fcase`, `fexit`, `ffork`, `flib-fp` and `fown`
  ran Heroes code inside the signal, exit 0** (the walk has no bounds there, the compiler-engineer's
  Darwin finding reproduced on Linux); the sanitized storm 2 of 2 refused. `lldb` could not launch a
  process in the container (*"personality set failed: Operation not permitted"*): the debugger's view
  of the probe on Linux is **unrun**; on Darwin it is the compiler-engineer's (SIGURG passed, not
  stopped).
- **Interposition in Linux** (`ipose.c`, `linux-247b.log`): **every row seen inside a handler, the
  shared library's three included**, with or without `-rdynamic` and under ASan + UBSan; the query
  returns the program's own handler.

## 8. E, S and F from the C side (brief item 5): `F/c/defer.c`, run on both platforms

One C program per mechanism, `main` doing the storm's work and the deferred `work` doing everything
a handler may not (`malloc`, `snprintf`, `fwrite`), storm 4 s (`F/logs/defer-g-darwin.txt`,
`linux-247b.log` § 5):

| mechanism | Darwin storm | Linux storm | latency behind a 2 s C call | `siginfo` |
|---|---|---|---|---|
| E: C handler writes a byte down a pipe, a thread reads and runs the work | exit 0; 345,730 deliveries, work ran 345,730 times, 0 dropped | exit 0; 882,615, all run | **0.000 s** (both) | lost unless copied into the note |
| S: C handler counts, `main` polls at each loop head | exit 0; 478,804 deliveries, **work ran 1,187 times** | exit 0; 701,130, ran 1,192 times | **2.000 s**: the work waits for C to return (both) | lost |
| `sigwait` on a thread, the signal blocked everywhere else | exit 0; 505,288, all run | exit 0; 1,038,251, all run | 0.000 s | **by value on Linux** (`sigwaitinfo`: the sender's pid right 1,038,251 times); **none on Darwin**, which has no `sigwaitinfo` |
| kqueue `EVFILT_SIGNAL` (Darwin) / `signalfd` (Linux) | exit 0; counted **3,295,177, every signal sent** (it counts generations while the signal is ignored); work ran 2,022,128 times | exit 0; 1,421,997, all run, `ssi_pid` right every time | 0.000 s | Linux by value; Darwin a count only |

`main` was never wrong in any of the eight storms. What the table says for a program: **standard
signals do not queue** (the kernel merged about a quarter to three quarters of the sends before any
handler ran: Linux pipe 882,615 handled of 1,148,773 sent), so any deferred handler is a "some signal
came" event, never a count of sends, except kqueue's on Darwin; **S cannot answer while the thread is
in C** (2.000 s); **the context pointer of `SA_SIGINFO` cannot be deferred at all** (it describes an
interrupted state that no longer exists), and the `siginfo_t` only where the mechanism hands it by value.
**A fault cannot be deferred**: a SIGSEGV handler that only notes the fault and returns re-runs the
faulting instruction until killed (`defer-pipe pipe fault`: exit 142, the 2 s alarm, both platforms).
**A SIGTERM cleanup that must finish before the program ends** runs, under E, on a thread `main`'s
return must join, and under S only when the thread polls (by reading `defer.c`, not run for `exit`).

**The platform layer's stop is already E's mechanism for two signals**: its handler is the layer's own
C, writing one byte down a pipe the waiting calls watch (`runtime/platform/hero_net.c:560-573`,
`hero_net.h:145-147`, read). **F's case, a Heroes function the program's own C handler calls and
waits for**, cannot be deferred by any mechanism above: the C handler needs the result before it
returns. The compiler-engineer's F refuses it, by the walk; § 4 shows the walk misses it without
frame records.

## 9. G, built and run: `F/tree-g`, the runtime's own door

`runtime/parts/sigdoor.c` (71 lines, `wc -l`), included by `runtime.c`, and three declarations in
`hero_os.h`: `hero_signal_watch(sig) -> i64` (the runtime's C handler: one process-wide `_Atomic`
counter per signal and one byte down a non-blocking pipe, both async-signal-safe; -1 for a fault's
signal, SIGKILL, SIGSTOP, SIGINT and SIGTERM, whose door is the platform layer's stop),
`hero_signal_take(sig) -> i64` (the deliveries since the last take, swapped to 0) and
`hero_signal_fd() -> i32` (readable once one came, for `poll`). **No compiler change.** Built by
`heroes-d` copied beside that runtime (Darwin) and by `tree-g`'s seed (Linux), each exit 0.

| program (`F/h/g/`) | Darwin, 2 storms | Linux arm64, 2 storms |
|---|---|---|
| `gcount`: watch SIGUSR1, `main` builds the storm's strings and takes between rounds | exit 0 both; counted 1,126,099 and 891,492 (of 1,196,402 and 964,051 sent) | exit 0 both; 1,184,597 and 1,024,584 (of 1,704,972 and 1,561,463) |
| `gtwo`: `main` watches, **another module** takes | exit 0 both; 780,523 and 606,994 | exit 0 both; 1,532,787 and 1,482,231 |

No Heroes function is handed to C, no crash report, and the second module reads the one counter:
**the two-module trap of a `static` flag in a shared header (the spec-warden's swp6, ignored 2 of 2)
has no place to stand.** On Windows the door has nothing to watch: UCRT raises SIGINT and SIGBREAK
for console events (the stop's), never generates SIGILL or SIGTERM (Microsoft's page, § 6), and the
rest are faults; so G is POSIX's alone, and its Windows arm answers -1 (unbuilt). G enforces nothing
by itself and owes Principle 0's ticket as a new surface in `hero_os.h`.

## 10. A from the C side: what a refusal by the installer's name misses, compiled and run

Each below hands no Heroes function to `signal`, `sigset` or `bsd_signal`, and each ran a Heroes
function inside a signal under D alone on both platforms (§ 4, `d-*`, and `linux-247.log` § 4b):
`wrap` (the coordinator's), **`fexit`** (`atexit`, spec § 13's own example, run by `exit()` from the
program's C handler: *"the atexit function ran: 42"*), **`ffork`** (`pthread_atfork`'s prepare run by
`fork()` from a C handler: *"prepare ran: 42"*), **`flib-fp` and `flib-nofp`** (a library's own handler
calling the program's callback, as a fault library or a crash reporter does), `fown`. A's list sees
none of them, so under A they run as under D. And A in its declaration form refuses `chandler`, the
C handler R4 allows (the compiler-engineer's run): **a binding of `signal` itself becomes impossible
to write**, which is this seat's own veto ground.

## 11. The repair, built into route C's runtime and run on both platforms

`F/tree-c2`: route C's tree with § 5's two questions added to `parts/thread.c` (the runtime defines
`sigaction`, `signal` and `sigset`, each handler put behind `hero_signal_around`, which counts a
`_Thread_local` depth; `hero_callback_entered` then refuses, after the exact test, where the depth is
not 0, and, for a frame off the thread's stack, where `sigaltstack` answers `SS_ONSTACK`; the walk
kept after both). On Darwin its programs are built by route C's compiler beside that runtime; in
Linux by `tree-c2`'s own stage 2 (`sources 415ae53c4d33e9d4`, `F/logs/linux-247c.log`).

| | Darwin arm64 | Linux arm64 |
|---|---|---|
| six tracked run cases, judged by their `.expected` with the harness's rule (`F/tools/expect.py`): the stack guard's 2253 pair and 2277, whose handlers now pass through the interposed `sigaction`, and the three 2257 cases | **6 of 6 PASS** (and 6 of 6 under D and under route C as built, `F/logs/runcases-darwin.txt`) | **6 of 6 PASS** |
| `wrap`, `wraponstack`, `infocase`, `fcase`, **`fown`**, **`flib-nofp`**, `fexit`, `ffork` | **8 of 8 refused**, the words whole, exit 134 | **8 of 8 refused** |
| the same five walk cases under `--sanitize` (`fcase`, `fown`, `flib-nofp`, `fexit`, `ffork`) | **5 of 5 refused** | **5 of 5 refused** |
| `chandler`, a C handler handed to `signal` | `1`, exit 0 | `1`, exit 0 |
| storms: `fownlocale` x3, `fliblocale` x3, `storm` x2, `quiet` x2, `clocale` x2 (Darwin) / sanitized `storm` x1 (Linux) | **12 of 12 refused with the words, -6; 12 crash reports, every one SIGABRT through the refusal's `raise`, no SIGKILL** (`ips.py`) | **11 of 11 refused, -6** |

**Its cost** (`F/h/qsortcost.hero`, the compiler-engineer's, `-O2`, `/usr/bin/time -l`, 3 runs each,
`F/logs/qsortcost-darwin.txt`): D 1,508,339,769 to 1,510,062,411 instructions; route C as built
(walk 8) 5,561,806,326 to 5,563,383,717 (3.7x, the compiler-engineer's own figure reproduced); the
repair with the walk 6,106,665,014 to 6,107,978,269; **the repair without the walk
(`HERO_SIGNAL_WALK 0`) 2,994,758,182 to 2,999,757,615 (2.0x), still refusing all five F cases**
(`w0-*`, exit 134 with the words). Its three thread-local reads per callback (the depth and the two
stack bounds, each a call of Darwin's accessor) are the price; one thread-local record read once is
the obvious next step, unbuilt and unmeasured. A Heroes call pays nothing, as under route C.

**What it still leaves, measured or named**: a shared library on **Darwin** whose own handler calls
the program's callback and keeps no frame record (`ipose.c`'s shared rows: missed, two-level
namespace; the walk sees it only with frame records); an installer the interposition does not define
(glibc exports nine: `sigaction`, `__sigaction`, `signal`, `sigset`, `bsd_signal`, `sysv_signal`,
`__sysv_signal`, `ssignal`, `sigvec`, by `nm -D --defined-only libc.so.6` in the container; the
prototype defines three, a landing owes the list with a test that reads it from the C library);
a raw `rt_sigaction` system call; and **a handler left by `siglongjmp`, which leaves the depth raised
and would refuse a later, correct callback** (unbuilt and unrun: ThreadSanitizer interposes the jumps
for this reason, the historian's source). D stays the backstop for all of them.

## 12. The questions, answered from this seat

- **Q1, the class**: C's, not the runtime's (§ 3: plain C dies of the locale lock on Darwin, hangs in
  stdio on Darwin, corrupts stdio in silence on Linux); every handler a program can install reaches it:
  by `signal`, by its own `sigaction` (`wrap`), from a library (`flib`), through `exit` and `fork`
  (`fexit`, `ffork`), on any alternate stack (`fown`), on a worker (`retaddr`).
- **Q2**: route C enforces R4 for a Heroes function a delivery calls, exactly, on both POSIX platforms
  measured; for one a C handler calls, as built, only where frame records stand and the alternate stack
  is the runtime's, and with the repair everywhere § 11's table names. It refuses what runs today: the
  2496 case and D's own case (the compiler-engineer's), `quiet`, `t3x1` and every `wrap`-shaped program.
- **Q3**: every refusal by D's shape ends -6 on Linux everywhere and on Darwin where a signal can be
  raised, 134 on an ending Darwin thread; `abort()` gives SIGTRAP there (§ 2). The harness's `!panic:`
  holds for both (`expectation.hero:196-200`).
- **Q4**: the exact test holds on Darwin arm64 (`_sigtramp+56`) and Linux arm64
  (`__kernel_rt_sigreturn`), the alternate stack and `SA_SIGINFO` included; the walk fails without
  frame records on both, on an alternate stack it does not know, past its depth inside glibc's `qsort`,
  and under `--sanitize`; **x86-64 is unrun** (no native machine; the emulated container left the
  routine). The probe leaves a program's own SIGURG handler and an ignored SIGURG as they were (`urg.c`,
  100 of 100 counted after); a SIGURG sent from outside during the probe's two `raise`s would be taken by
  the probe (by reading, unrun). The debugger's view: Darwin the compiler-engineer's; Linux unrun (§ 7).
- **Q5**: § 8. A fault cannot be deferred; `SA_SIGINFO`'s context never can, its `siginfo_t` only by
  copy or `sigwaitinfo`/`signalfd`; standard signals merge; S waits for C.
- **Q6, D's words**: they stay true as the backstop's words under the repair, but **their remedy is
  wrong as written**: *"a handler written in C"* dies like a Heroes one when that C formats a number
  (§ 3, plain C, 4 of 4 SIGKILL) and corrupts output when it prints (Linux), and in two modules sharing
  a header it is ignored (the spec-warden's swp6). The remedy that holds is a C handler that only counts
  or writes a byte, which is G (§ 9); whoever lands the route amends the words in the same commit.
- **Q7**: § 7's table: harmless for `snprintf` in about 7 million checked calls, **silent corruption
  for stdio**, a hang in ASan's allocator.
- **Q9, Windows**: § 6, compiled, unrun.
- **Q10, D's place**: kept, as the backstop for what § 11 leaves; its cost is its commit's.

## The verdict, in this seat's structure

- `verdict`: **approve route C in the thunk form with this seat's repair** (the exact test; a
  thread-local depth kept by the runtime's own definitions of the C library's installers; the
  `SS_ONSTACK` question for a frame off the thread's stack; the walk optional), **with D kept as the
  backstop and G as the door a counting program is sent to**; **veto route C as built, with the walk as
  the only answer to F's case** (`fownlocale` and `fliblocale` dead in silence 6 of 6 on Darwin, run
  inside the signal 6 of 6 on Linux, five walk cases unrefused under `--sanitize` on Linux); **veto D
  alone, A alone, A in its declaration form, E by name, H and I**; **object to F and S**, which honour
  R4 at the price § 8 measures and change what runs today (the author's call); **approve G as a
  complement**.
- `section`: design.md §1.11 and §4.19 (everything comes from C, so every handler a C library or the
  program's own C installs is the boundary's), §1.12 (a program must not corrupt memory or die in
  silence), and CLAUDE.md § Precedence rank 3 over rank 5 for the repair's cost. design.md does not
  cover interposing the C library's own names from the runtime; that is a new architectural act the
  synthesis must name (panel 239's `ffi_defined_elsewhere` judges a library defining a runtime name,
  and these three would become runtime names).
- `experiment`: C written and compiled with clang on Darwin arm64 (Apple clang 21) and in Linux arm64
  (Debian clang 22.1.8), every probe exit 0 at its build but `ipose_lib.c` in glibc before
  `_GNU_SOURCE` (`sigset` undeclared, repaired, § 7): `F/c/retaddr.c`, `walk_main.c` + `walk_h.c`
  (three flag sets, the `ONSTACK_TEST` repair), `refuse.c`, `reent.c` (five modes, plain, ASan, glibc's
  check), `ipose.c` + `ipose_lib.c` (static and shared libraries without frame pointers, ASan),
  `defer.c` (four mechanisms, latency, fault), `urg.c`; Heroes cases in `F/h/` (my `fown`,
  `fownlocale`, `flib`, `fliblocale` with `lib/sigcb.c` built with and without frame pointers, `fexit`,
  `ffork`, G's `g/gcount` and `g/gtwo`, and the compiler-engineer's, copied), built by D's compiler,
  route C's and the repair's, all exit 0; runtime C for G (`F/tree-g/runtime/parts/sigdoor.c`) and the
  repair (`F/tree-c2/runtime/parts/thread.c`), each accepted by clang inside the runtime's own build on
  both platforms; Windows C (`F/win/winprobe.c`, `thread_win.c`) accepted by `zig cc` for x86-64 and
  Aarch64, 0 warnings, **unrun**.
- `argument`: The exact test is exact: 2.3 million deliveries on two kernels, one address each,
  never a false match. The walk is not: it needs frame records a library may lack, a stack it knows,
  depth glibc's qsort exceeds, and bounds the sanitizer removes, and route C as built let Heroes code
  run inside signals there, killing silently on Darwin. Asking the delivery's own state instead (a
  depth kept by the runtime's own installers, the kernel's SS_ONSTACK) refused every case on both
  platforms, sanitized too, at 2.0x the callback cost where the walk costs 3.7x. A C handler that formats
  dies like a Heroes one, so the remedy D's words give must become G's byte. (114 words, counted by `str.split`)
- `prediction`: **(1)** at the adopted route's landing, `F/h/fliblocale.hero` (a static library whose
  handler, built `-O2 -fomit-frame-pointer`, calls the program's callback) and `F/h/fownlocale.hero`,
  built by the landed compiler, end `-6` with the refusal's words in **20 of 20** runs each of
  `storm.py <binary> SIGUSR1 4 0.3` on Darwin arm64 and in Linux arm64, `ips.py` reading only SIGABRT;
  **landed without the depth and the `SS_ONSTACK` question, both end `-9` with nothing on stderr in at
  least 18 of 20 on Darwin**. **(2)** A Heroes comparator handed to **glibc's `qsort`** of 10,000 values
  from inside the program's own C handler is refused at its **first** call under the repair in Linux
  arm64, where the walk alone misses that call (61,280 of 121,205 missed here). **(3)** glibc's stdio
  re-entered from a handler corrupts its `FILE` in silence in at least 9 of 10 runs of `reent-print`
  in Linux arm64, plain, under ASan and under `glibc.malloc.check=3`, so no handler of any language
  may print there. Each falsified by the first run that disagrees.
- `condition`: (a) the `siglongjmp` question measured on a real binding (a fault library that jumps
  out of its handler, then calls the program back): a false refusal there makes the depth need the
  jumps interposed too, or its loss accepted beside the walk; (b) a C library that installs handlers by
  a path the interposition cannot define (a raw `rt_sigaction`, a Darwin dylib without frame records)
  and calls the program back from them in a binding that matters: then D alone stands there and the
  synthesis says so; (c) the Windows box: if `winprobe raise` shows UCRT calling handlers from more than
  one site, or `winprobe fault` no dispatcher frame within 32, the Windows arm compiled here is wrong
  and the thread guard's refusal of Ctrl-C is Windows' only enforcement; (d) the author choosing to
  honour R4 rather than enforce it: the same depth and `SS_ONSTACK` answers are F's exact selector, and
  my objection to F becomes § 8's costs alone.

## Unrun, said once

Linux x86-64 (any probe); Windows (every Windows sentence above is compiled or read, never run);
`lldb` on Linux (the container refused it a process); the repair's `siglongjmp` case; the repair with
every installer glibc exports; a refusal under the repair on a Darwin thread that is ending (the shape
of § 2's last row, in the runtime); a SIGTERM cleanup deferred past `exit(code:)`; the depth read from
one thread-local record. No paid run, no API call, no GitHub write; two public pages fetched with
`curl` (Microsoft's `signal` and `HandlerRoutine`, `F/pages/`). Every process I started was stopped
(`pgrep -fl 247-ffi-pragmatist` after each batch: nothing).
