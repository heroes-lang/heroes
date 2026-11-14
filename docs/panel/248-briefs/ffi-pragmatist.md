# Panel 248, ffi-pragmatist

Read `00-shared.md` first, whole: the base, the rules (**your binaries are named `fp-...`**, every
program that may not end has its bounds there), the platforms (**the Windows box is off**; zig here
compiles MinGW's target, the project's is MSVC's), why the sitting is the full panel, and the five
questions with every fact this brief leans on. You judge each question from the C side, with a veto on
ABI breakage and on soundness at the boundary. Your folder is
`.claude/worktrees/p248-scratch/248-ffi-pragmatist/`, your tree `tree/` in it, a `git archive` of
**`df7612ad3`** (tree `f72f48da250e919a68ebc6a98be0facccc29a052`), your compiler built there from the
seed in two stages (`sources 4c9a6a9800095eef`). Your report is
`docs/panel/248-reports/ffi-pragmatist.md` in `.claude/worktrees/p248`, in your definition's output
structure (`.claude/agents/ffi-pragmatist.md`, its line 33), one section per question, written as you
go. **Compile, don't opine**: every mechanism below is C you write, compile with clang on this Mac and
run, and in Linux arm64 by the Docker line in 00-shared.md § The platforms (`--name fp-248-<n>`, every
probe inside under its own `timeout -s KILL`, `docker ps` read and the container stopped after); a
Windows arm is compiled with `zig cc -target x86_64-windows-gnu`, said to stand for MinGW, and named
owed, never inferred. **The route you recommend on each question, you build and run on the cases you
name**, before the synthesis (`/panel` 3c). **Every Darwin storm reads its crash reports** (`ips.py`,
by your binary's name and the storm's pids), and counts only runs whose output holds `armed`.

## Question 1: defect 2522, the C library's installers

**Read first**: panel 247's R3 and R4 (`docs/panel/247-*.md:191-211`), your seat's own report of that
sitting (`docs/panel/247-reports/ffi-pragmatist.md:173-191`, `:280-282`, `:376-383`) and that critic's
re-run of it (`docs/panel/247-reports/critic-reports.md:84-110`); this sitting's critic, sections C and
D (`docs/panel/248-reports/critic-briefs.md`); the prototype in the evidence, read-only
(`.claude/worktrees/p248-scratch/briefs/ev247/247-ffi-pragmatist/c/ipose.c` and `ipose_lib.c`, its logs
under `logs/`; 247's critic's `direct.hero`, `dlocale.hero` and `hold.hero` under
`ev247/critic2/work/`); the coordinator's re-runs (`.claude/worktrees/p248-scratch/briefs/fix/`);
`runtime/parts/sigguard.c` (00-shared.md gives its lines). **Already measured and in 00-shared.md, so
re-run only where you lean on it**: Darwin's ten installers and the four undeclared ones run; glibc's
prototypes, its eight linkable handler takers, `sigvec` a compat symbol, `__libc_sigaction` private;
which glibc names reach an executable's `sigaction` from the program and from a shared library, and
`sigignore`'s and `siginterrupt`'s effect; `sysv_signal`'s `SA_NODEFER` and `SA_RESETHAND`; the flat
namespace on the executable; `--wrap` by each linker and its reach into the runtime's object; the
sanitizer's symbols on both legs; the C twin's death.

1. **Reproduce R3's two cases at the base on both POSIX legs** (00-shared.md names them and their build
   lines): `fp-flib-nofp`, 20 runs each leg; `fp-fliblocale`, five armed storms each leg, crash reports
   read on Darwin. Then the same library built as a **shared** library (`.dylib`, `.so`), since the
   two-level namespace question is a dylib's, and the C twin built as a shared library's handler.
2. **Route I, written in C and run against R4's list, one row each, Darwin and Linux arm64**: (a) the
   probes on the C library's own entry point (say which: `__sigaction`, `dlsym(RTLD_NEXT,
   "sigaction")` at start, or another, and whether the exact test still learns `_sigtramp`'s and
   `__kernel_rt_sigreturn`'s or `__restore_rt`'s address); (b) **every installer each C library exports
   that takes a handler**, defined with its whole meaning: Darwin's ten (00-shared.md), glibc's eight
   linkable, and `sigvec` for an old binary; and `sigignore` and `siginterrupt` mirrored so
   `sigaction(sig, NULL, &old)` stays true; (c) `SIG_HOLD`, `SIG_IGN`, `SIG_DFL` and the previous handler
   answered; (d) `siglongjmp` out of a handler, then a correct callback outside any handler: not
   refused; (e) R3's two cases refused, 20 of 20 each on each leg; (f) 247's critic's `direct.hero`,
   `dlocale.hero` and `hold.hero` shapes, re-run.
3. **How each platform resolves the name, what is still unrun**: Darwin: an `__interpose` section in the
   executable, a dylib itself linked `-flat_namespace`, and I-dylib, the runtime's installers shipped as
   a dylib with an `__interpose` section as the address sanitizer's is (quote `man ld` and `man dyld`
   where you lean on them; say what System Integrity Protection or the hardened runtime refuses); glibc:
   a library loaded by `dlopen` after start, and a raw `syscall(SYS_rt_sigaction, ...)`; Windows: what
   UCRT exports that installs a handler (its headers in zig's tree or Microsoft's page, quoted), whether
   an executable's `signal` is reached by a DLL's call, and the three Win32 installers the runtime itself
   uses (`AddVectoredExceptionHandler`, `SetUnhandledExceptionFilter`, `SetConsoleCtrlHandler`,
   00-shared.md), compiled for MinGW and owed.
4. **What it breaks, run**: a program built `--sanitize` beside route I (does it link, and which
   definition does a call reach, on both legs); a library of the program that defines `sigaction` itself
   (a crash reporter's shape) beside the runtime's, and what panel 239's member tells it
   (`.claude/rules/c-boundary.md:186-220`); the runtime's own 27 installs and 16 mask calls (00-shared.md).
5. **The alternatives, each written and run far enough to price**: W (`--wrap`, which reaches the
   runtime's object: the runtime calling `__real_<name>`, or the link split) and R (the archives'
   references renamed with `objcopy --redefine-sym` in the Linux image, where both `objcopy` and
   `llvm-objcopy` stand), each with its shared-library residue; U, an unwinder's walk
   (`_Unwind_Backtrace`, libunwind's `unw_is_signal_frame`) through `sigcb_handler` built
   `-fomit-frame-pointer`, with and without `-fno-asynchronous-unwind-tables`, its cost a callback and
   whether it is safe inside a signal (read glibc's `dl_iterate_phdr` and say whether it takes a lock);
   B, the asynchronous signals blocked on every claimed thread, and where a signal then lands
   (`pthread_kill` to a claimed thread, a fault's signal, a C library that unblocks); T, a bounded scan
   of the stack for the trampoline's return address, and the stale word it may misread. M is a question
   (00-shared.md): say whether an interposer of `pthread_sigmask` and `sigprocmask` would make it a route.
6. **Your verdict**: may the runtime define the C library's names, on which platforms and with what
   residue on each; or which route closes R3's two cases with the smallest residue, saying for each
   platform what it closes (Darwin: a refusal with words before the callback's first C call; Linux arm64:
   no Heroes code inside the signal); and the C each needs on each platform, compiled.

## Question 2: defect 2502, the waits

1. **The POSIX wake, written and run**: `eventfd` on Linux, `kqueue`'s `EVFILT_USER` on Darwin, a pipe on
   both, for each of the shapes 00-shared.md lists (per waiting thread, one shared with a generation, per
   socket, one poller for the process, a thread-directed signal): instructions while waiting (the
   corrected probe, `waiters-checked.c`, copied from `.claude/worktrees/p248-scratch/briefs/r2502/` and
   built as `fp-waiters`, against the base's 271.6 to 275.0 million at 5 s), and a close's latency, on this
   Mac and in Linux arm64.
2. **Under a small limit, server-shaped**: a server process holding N accepted connections, its peers in
   another process, under each shape and `ulimit -n 256` (this Mac's launchd soft limit, `launchctl limit
   maxfiles`; this session's shell reads 1048576, so set it): `lsof -p` at N = 100 and at the largest N
   that fits; what fails past it, and how the layer could answer (a fall-back to slices, or
   `HERO_NET_LIMIT`).
3. **Windows, compiled for MinGW and owed**: `WSAPoll` with a wake it can watch (a connected loopback
   socket pair; read whether `WSAPoll` watches anything but sockets on Microsoft's page and quote it),
   `WSAEventSelect` with `WaitForMultipleObjects` (whether it is bounded by `MAXIMUM_WAIT_OBJECTS`, and at
   what count, read and quoted), or the slices kept on Windows alone; what each costs in handles.
4. **The stop's pipe beside it** (`hero_net.c:62-69`, `613-657`): whether the wake replaces it, joins it,
   or stays apart, and what a fork between `hero_net_catch_stop` and `exec` does to each descriptor (both
   ends are close-on-exec, 620-631).

## Question 3: defect 2490, a child's working folder

1. **POSIX's list is read** (00-shared.md quotes it and the fork page: `chdir` and `fchdir` on it;
   `getrlimit`, `setrlimit` and `execvp` off it). Read every call the child already makes between `fork`
   and `exec` (`runtime/parts/run.c:1333-1477`, and what `hero_run_sentinel_child_hold` and
   `hero_run_tty_give` call) against the same list, and say for each off-list call whether glibc's and
   Darwin's implementations take a lock or allocate (read their sources or manuals and quote), and what
   would replace it (`execve` with a path the parent resolved, the limit set in the parent around the
   `fork`, or `posix_spawn`'s attributes).
2. **Run on both POSIX legs**: a threaded parent (several threads allocating and printing) that forks
   and `chdir`s, or `fchdir`s to a folder it opened before the `fork`, in the child before `exec`, at most
   10,000 forks under `timeout -s KILL 120`, plain and under `--sanitize`; a missing folder, an
   unreadable one and a file in its place, and what errno each gives down the report pipe, or in the
   parent's `open` under the `fchdir` shape; a relative folder, a relative program path (`./x`) and
   relative stream paths, each before and after the `chdir`, so the order the door promises is measured,
   not chosen. And `posix_spawn_file_actions_addchdir_np` (Darwin's SDK and glibc 2.41: say whether each
   declares it) and POSIX 2024's `posix_spawn_file_actions_addchdir` as the other shape.
3. **Windows, compiled for MinGW and owed**: `CreateProcessW`'s page is read (00-shared.md: *"The full
   path to the current directory for the process"*, nothing on a relative or missing folder, the program
   looked for in the parent's folder, the `=X:` entries an environment block drops); compile the arm,
   and write the case each of those owes on the box: a relative folder, a missing one and its error, and
   whether `hero_run_win_environment`'s block (`run.c:376-400`, built from `GetEnvironmentStringsW`)
   keeps the `=X:` entries. And whether clang on Windows reads `C_INCLUDE_PATH=:` and
   `=/nonexistent-2344:` as this Mac does (clang's driver source or documentation for its separator on
   Windows, quoted), which decides whether the two 2344 folder cases could pass there.

## Question 4: a stop the program asks itself

Write the door in C on both POSIX legs and compiled for MinGW: from any thread, no signal, the counter
`hero_net_stop_asks` and, where the stop is armed, one byte down its pipe (`hero_net.c:549-566`). Run,
under ThreadSanitizer by hand on a C probe (`-fsanitize=thread`, this Mac and Linux arm64, `timeout -s
KILL 120`, output `head -c 65536`), a door called from a worker while `main` waits in `accept` and other
workers in `recv`: every wait answers stopped within your bound, no race reported. Then the
second-signal question, run: after the program asked its stop, a SIGINT, and after it a second; today
any signal after the first ends the process (`hero_net.c:556-577`), which a program that asked its own
stop may not expect: say what the door should make of it. And on Windows, the console handler's counter
(`hero_net.c:520-533`) shared with the door: the same question, compiled and owed.

## Question 5: a connection that sends nothing

Write the deadline in C at the layer's wait (the sockets are non-blocking, `hero_net.c:46-48`, so the
wait's own `poll` or `WSAPoll` timeout is the place it would live; **run** whether `SO_RCVTIMEO` reaches a
non-blocking socket's `recv`, or the layer's `poll`, on each POSIX leg): per call, and per socket as
Go's `SetReadDeadline` (the absolute time, the clock that never goes back on each platform,
`CLOCK_MONOTONIC`, `mach_absolute_time` or `clock_gettime_nsec_np`, `QueryPerformanceCounter` or
`GetTickCount64`, each read from its manual and quoted). Run a quiet peer against it on both POSIX legs,
the server under `timeout -s KILL 60` and each peer under its own 10 s limit: answered at its deadline
within the bound you state, and a peer that sends one byte just before the deadline: not answered timed
out. Compile the Windows arm for MinGW and name it owed. Say what the deadline means for the stop and
for question 2's wake, and whether a deadline set on a socket must cross with it as a number
(`hero_net.h:103-121`).

## What your verdict owes

For each question: the route you would adopt, the C it needs on each platform, compiled, and run where
the platform is here; what it does to programs that run today; every crash report its storms left
(armed runs alone counted); whether it needs a spec sentence, a new or widened diagnostic, or a
reader-facing card change (the escape in 00-shared.md); and a veto, if you cast one, names the program
that corrupts memory, dies in silence or breaks the boundary under the route vetoed, compiled and run.

**Linux arm64**: `linux-248.sh` in your folder, run by the Docker line in 00-shared.md, every probe inside
under its own `timeout -s KILL` and its output bounded by `head -c`; the log is part of your report's
evidence and stays in your folder.
