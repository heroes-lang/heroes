# Panel 248, ffi-pragmatist

Read `00-shared.md` first, whole: the base, the rules, the platforms (**the Windows box is off**), and
the five questions with every fact this brief leans on. You judge each question from the C side, with
a veto on ABI breakage and on soundness at the boundary. Your folder is
`.claude/worktrees/p248-scratch/248-ffi-pragmatist/`, your tree `tree/` in it, a `git archive` of
**`df7612ad3`** (tree `f72f48da250e919a68ebc6a98be0facccc29a052`), your compiler built there from the
seed in two stages (`sources 4c9a6a9800095eef`). Your report is
`docs/panel/248-reports/ffi-pragmatist.md` in `.claude/worktrees/p248`, in your definition's output
structure (`.claude/agents/ffi-pragmatist.md`, its line 33), one section per question, written as you
go. **Compile, don't opine**: every mechanism below is C you write, compile with clang on this Mac and
run, and in Linux arm64 by the script line in 00-shared.md § The platforms; Windows is compiled with
`zig cc` and named owed, never inferred. **The route you recommend on each question, you build and run
on the cases you name**, before the synthesis (`/panel` 3c). **Every Darwin storm reads its crash
reports** (`ips.py`, 00-shared.md): the deaths that matter here say nothing on stderr.

## Question 1: defect 2522, the C library's installers

**Read first**: panel 247's R3 and R4 (`docs/panel/247-*.md:191-211`), your seat's own report of that
sitting (`docs/panel/247-reports/ffi-pragmatist.md:173-191`, `:280-282`, `:376-383`) and the critic's
re-run of it (`docs/panel/247-reports/critic-reports.md:84-110`); the prototype and its logs in the
evidence, read-only (`.claude/worktrees/p248-scratch/briefs/ev247/247-ffi-pragmatist/`: the report's
`F/c/ipose.c` and `ipose_lib.c` stand there as `c/ipose.c` and `c/ipose_lib.c`, its logs under
`logs/`; the critic's `direct.hero`, `dlocale.hero` and `hold.hero` under
`ev247/critic2/work/direct/` and `work/hold/`); `runtime/parts/sigguard.c` (00-shared.md gives its
lines).

1. **Reproduce at the base on both POSIX legs**: `fliblocale` and its static library (00-shared.md
   § Question 1 has the coordinator's build, Darwin 3 of 3 `exit -9` and Linux 3 of 3 exit 0); five
   storms each, crash reports read on Darwin. Then the same library built as a **shared** library
   (`.dylib`, `.so`), since the two-level namespace question is a dylib's.
2. **Route I, R4's corrected form, written in C and run against R4's list, one row each, Darwin and
   Linux arm64**: (a) the probes on the C library's own entry point (say which: `__sigaction`,
   `dlsym(RTLD_NEXT, "sigaction")` at start, or another, and whether the exact test still learns
   `_sigtramp`'s and `__kernel_rt_sigreturn`'s or `__restore_rt`'s address); (b) **every installer each
   C library exports that takes a handler**, defined with its whole meaning: Darwin's five declared
   (`bsd_signal`, `sigaction`, `sigset`, `sigvec`, `signal`) and `__sigaction`, glibc's nine
   (00-shared.md lists both, each from a command), their prototypes read and quoted; (c) `SIG_HOLD`,
   `SIG_IGN`, `SIG_DFL` and the previous handler answered, `sigaction(sig, NULL, &old)` answering the
   program's own; (d) `siglongjmp` out of a handler, then a correct callback outside any handler: not
   refused; (e) R3's two cases refused 20 storms of 20 on Darwin and in Linux arm64; (f) panel 247's
   critic's `direct.hero`, `dlocale.hero` and `hold.hero` shapes (`critic-reports.md:93-108`), re-run.
3. **How each platform resolves the name, run**: Darwin: a dylib's `sigaction` against an executable
   that defines it, plain, linked `-flat_namespace`, and through an `__interpose` section in the
   executable (each unrun until now; say which System Integrity Protection or the hardened runtime
   refuses, and quote `man ld` and `man dyld` where you lean on them); glibc: a shared library, a
   library loaded by `dlopen` after start, a library calling `bsd_signal`, `sysv_signal`, `ssignal` or
   `sigset` (does any reach the executable's `sigaction`?), and a raw `syscall(SYS_rt_sigaction, ...)`;
   Windows: what UCRT exports that installs a handler (read its headers in zig's tree or Microsoft's
   page, quoted), and whether an executable's `signal` is reached by a DLL's call, compiled with `zig
   cc -target x86_64-windows-gnu` and owed.
4. **What it breaks, run**: a program built `--sanitize` (00-shared.md lists the address sanitizer's
   own `signal` and `sigaction` symbols on both legs, weak in the Linux image's archive and dyld
   substitutions on this Mac): does it link, and which definition does a call reach; a library of the program that
   defines `sigaction` itself (a crash reporter's shape) beside the runtime's, and what panel 239's
   member tells it (`.claude/rules/c-boundary.md:186-220`); the runtime's own 27 installs
   (00-shared.md lists them), which would pass through the runtime's definitions.
5. **The alternatives, each written and run far enough to price**: W, the link's `--wrap=<name>` on
   GNU ld and lld for the libraries a group names, and what Apple's linker offers instead (or that its
   manual holds nothing); U, an unwinder's walk (`_Unwind_Backtrace`, libunwind's
   `unw_is_signal_frame` where it exists) through `sigcb_handler` built `-fomit-frame-pointer`, with
   and without `-fno-asynchronous-unwind-tables`, its cost a callback and whether it is safe inside a
   signal (read glibc's `dl_iterate_phdr` locking and say); M, the thread's mask at the thunk against
   the thread's own, with `SA_NODEFER` and with a correct program that blocks a signal and calls back.
   For each, its residue, run where it can be.
6. **Your verdict**: may the runtime define the C library's names, on which platforms and with what
   residue each; or which alternative closes R3's two cases with the smallest residue; and the C each
   needs on each platform, compiled.

## Question 2: defect 2502, the waits

1. **The POSIX wake, written and run**: `eventfd` on Linux, `kqueue`'s `EVFILT_USER` on Darwin, a pipe on
   both, for each of the three shapes 00-shared.md lists (per waiting thread, one shared with a
   generation, per socket): descriptors per waiter, instructions while waiting (the lane's `waiters.c`,
   copied from `.claude/worktrees/p248-scratch/briefs/r2502/`, against the base's 273 to 275 million at
   5 s), and a close's latency, on this Mac and in Linux arm64.
2. **Under a small limit**: each shape run under `ulimit -n 256` (this Mac's launchd soft limit,
   `launchctl limit maxfiles`; this session's shell reads 1048576, so set it), with 100 and with 200
   waiting connections: what fails, and how the layer could answer it (a fall-back to slices, or
   `HERO_NET_LIMIT`).
3. **Windows, compiled and owed**: `WSAPoll` with a wake it can watch (a connected loopback socket pair;
   read whether `WSAPoll` watches anything but sockets on Microsoft's page and quote it),
   `WSAEventSelect` with `WaitForMultipleObjects` (whether it is bounded by `MAXIMUM_WAIT_OBJECTS`, and
   at what count, read and quoted), or the slices kept on Windows alone; what each costs in handles. `zig cc -target
   x86_64-windows-gnu`, the caches in your folder.
4. **The stop's pipe beside it** (`hero_net.c:62-69`, `613-657`): whether the wake replaces it, joins it,
   or stays apart, and what a fork between `hero_net_catch_stop` and `exec` does to each descriptor
   (both ends are close-on-exec, 620-631).

## Question 3: defect 2490, a child's working folder

1. **POSIX's list of async-signal-safe functions**: fetch POSIX.1-2024's page (the Open Group's System
   Interfaces, chapter 2, its section on signal actions, or the page you find, its URL in your report),
   quote whether `chdir` and `fchdir` stand
   on it, and read every call the child already makes between `fork` and `exec`
   (`runtime/parts/run.c:1333-1477`: `prctl` and `getppid` and `raise` on Linux (1354-1355),
   `setpgid`, `sigaction` and `sigprocmask` through `hero_run_child_signals` (814-825), `getrlimit`,
   `setrlimit`, `open`, `dup2`, `close`, `write`, `execvp`, `execvP`, `execvpe`, `_exit`, the
   `environ` assignment on Darwin, and what `hero_run_sentinel_child_hold` and `hero_run_tty_give`
   call) against the same list; name any that is not on it.
2. **Run on both POSIX legs**: a threaded parent (several threads allocating and printing) that forks
   and `chdir`s in the child before `exec`, many times, under `--sanitize` too; a missing folder, an
   unreadable one and a file in its place, and what errno each gives down the report pipe; a relative
   folder, a relative program path (`./x`) and relative stream paths, each before and after the
   `chdir`, so the order the door promises is measured, not chosen. And
   `posix_spawn_file_actions_addchdir_np` (Darwin's SDK and glibc 2.41: say whether each declares it,
   and POSIX 2024's `posix_spawn_file_actions_addchdir`) as the other shape.
3. **Windows, compiled and owed**: Microsoft's `CreateProcessW` page on `lpCurrentDirectory` (a full
   path? a relative one? a missing folder's error?), quoted; the arm compiled; and whether clang on
   Windows reads `C_INCLUDE_PATH=:` and `=/nonexistent-2344:` as this Mac does (read clang's driver
   source or documentation for its separator on Windows, quoted), which decides whether the two 2344
   folder cases could pass there.

## Question 4: a stop the program asks itself

Write the door in C on both POSIX legs and compiled for Windows: from any thread, no signal, the
counter `hero_net_stop_asks` and, where the stop is armed, one byte down its pipe (`hero_net.c:549-566`).
Run, under ThreadSanitizer by hand on a C probe (`-fsanitize=thread`, this Mac and Linux arm64), a door
called from a worker while `main` waits in `accept` and other workers in `recv`: every wait answers
stopped within your bound, no race reported. Then the second-signal question, run: after the program
asked its stop, a SIGINT, and after it a second; today any signal after the first ends the process
(`hero_net.c:556-577`), which a program that asked its own stop may not expect: say what the door should
make of it. And on Windows, the console handler's counter (`hero_net.c:520-533`) shared with the door:
the same question, compiled and owed.

## Question 5: a connection that sends nothing

Write the deadline in C at the layer's wait (the sockets are non-blocking, `hero_net.c:46-48`, so the
wait's own `poll` or `WSAPoll` timeout is the only place it can live; say whether `SO_RCVTIMEO` reaches
a non-blocking socket's `recv` on each platform, run on the two POSIX legs): per call, and per socket
as Go's `SetReadDeadline` (the absolute time, the clock that never goes back on each platform,
`CLOCK_MONOTONIC`, `mach_absolute_time` or `clock_gettime_nsec_np`, `QueryPerformanceCounter` or
`GetTickCount64`, each read from its manual and quoted). Run a quiet peer against it on both POSIX
legs: answered at its deadline within the bound you state, and a peer that sends one byte just
before the deadline: not answered timed out. Compile the Windows arm and name it owed. Say what the
deadline means for the stop and for question 2's wake, and whether a deadline set on a socket must
cross with it as a number (`hero_net.h:103-121`).

## What your verdict owes

For each question: the route you would adopt, the C it needs on each platform, compiled, and run where
the platform is here; what it does to programs that run today; every crash report its storms left;
whether it needs a spec sentence, a diagnostic or a reader-facing card change (the escape in
00-shared.md); and a veto, if you cast one, names the program that corrupts memory, dies in silence or
breaks the boundary under the route vetoed, compiled and run.

**Linux arm64**: `linux-248.sh` in your folder, run by the line in 00-shared.md § The platforms, its
output bounded by `head -c` inside the script; the log is part of your report's evidence and stays in
your folder.
