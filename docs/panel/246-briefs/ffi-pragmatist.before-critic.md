# Panel 246, ffi-pragmatist

Read `00-shared.md` first, whole. You judge **Part A** (the soundness lane, with the
compiler-engineer) and **Part B** (the full panel). Your folder is
`.claude/worktrees/p246-scratch/246-ffi-pragmatist/`, your tree `tree/` in it at `b8c8240d4`, your
compiler built there from the seed in two stages. Your report is
`docs/panel/246-reports/ffi-pragmatist.md` in `.claude/worktrees/p246`, one section per part, in
your definition's output structure, written as you go. **Compile, don't opine**: every door below is
C you write, compile with clang on this Mac and run; a platform you cannot compile on is named as
unrun, never inferred.

## Part A: the lock door and what it meets

The runtime has no lock today: `grep -rln -E 'flock|LockFileEx|F_SETLK|F_OFD_SETLK|lockf\(|LOCK_EX|
OpenProcess' runtime selfhost tests/harness` finds 0 files. Its file doors are `runtime/hero_os.h`
(488 lines, `wc -l`; the `hero_fs_*` block 172-300) over `runtime/parts/fs.c` (415) and
`runtime/parts/replace.c` (1268), and the runtime already opens with `O_CLOEXEC` (`grep -n O_CLOEXEC
runtime/parts/replace.c`: lines 496, 554, 590, 673, 878, 918, 1030, 1216, 1245). Children start by
`execvp` on POSIX and `CreateProcessW` on Windows (`runtime/parts/run.c`, 1962 lines).

**Write and run:**

1. **The POSIX arm** of a door that takes an exclusive lock on a file without waiting, answers
   *taken*, *held by another* or *this file system has none*, and holds it until the process ends:
   `flock(LOCK_EX | LOCK_NB)` against `fcntl(F_SETLK)` (whose locks a process loses when it closes ANY
   descriptor of the file, a question to run) and against `F_OFD_SETLK` where the platform has it
   (whether Darwin does is a question). For each: what a `kill -9` of the holder does, read from a
   second process; and whether the descriptor is close-on-exec, so a `git` or `curl` child of the run
   does not hold the lock after its parent dies (run it: a holder that spawns a long child, the holder
   killed, the lock asked).
2. **The file systems**: on this Mac, a FAT32 and an exFAT disk image made with `hdiutil create` and
   attached with `-mountpoint` at a folder **inside your folder** (never under `/Volumes`, which is
   outside the repository's root), the lock asked there; in the Linux arm64 container (below), the
   container's own file system. NFS and SMB: unrun unless you find a route inside the hard stops; say
   what `flock(2)`'s and Darwin's manual pages state of them, read today, and what route A does on a
   file system with no lock (proceed and say so, refuse, or fall back to B).
3. **The Windows arm**, written: `CreateFileW` and `LockFileEx(LOCKFILE_EXCLUSIVE_LOCK |
   LOCKFILE_FAIL_IMMEDIATELY)`. `find /opt/homebrew -maxdepth 6 -name windows.h` and `which
   x86_64-w64-mingw32-gcc` found nothing on this Mac, so it is not compiled here unless you find a
   route (a question). Read Microsoft's pages today and say what they state: when a lock is released
   after its process is ended, whether that is immediate, whether the handle is inherited by a
   `CreateProcessW` child, and whether a folder holding an open locked file can be removed.
4. **PID reuse and a live owner**: what the lock decides that the pid in `.fetching-<pid>-<n>` cannot,
   and the case 00-shared.md poses (`staging.hero:47-48` removing a folder of its own name; the serial
   restarting at 1, `run.c:1425-1428`), from the C side.
5. **Linux arm64**: `linux-a.sh` in your folder, the body of `docs/platforms/linux/LINUX-MACHINE.md`
   § How to run something (a read-only mount copied inside, no Dockerfile), building the seed and your
   C probes in the container; the coordinator runs it with `heroes-linux-arm64:latest` (image
   `338a1044ac90`, `docker images`) and hands you the log.

Then A1 from your seat: whether a lock file private to one run is the *"fifth input class"* panel
235's R12 refused (quoted in 00-shared.md), and the door's home: `hero_os.h`, which panel 232 promises
at 1.0.0 to every program that binds it, or `hero_compiler.h` (80 lines), the compiler's own.

## Part B: the server step

**B1, the rung at the C boundary.** `hero_thread_spawn(int64_t (*body)(int64_t), int64_t arg)` at
`runtime/hero_os.h:435`, `hero_thread_spawn_sized` 441, `hero_thread_join` 443, `hero_thread_limit`
447; the arms in `runtime/parts/spawn.c` (444 lines), the guard in `runtime/parts/thread.c` (99).
Write the C a program could ship to carry more than a number through the `i64`: a header of its own
whose `static inline` turns a `cstr` or a `ptr` into an `int64_t` and back (panel 114's R2 and R6,
ratified 2026-09-06, let a program ship such a header, though `grep -n -i ships
spec/heroes-spec.md` finds no sentence saying so today; panel 234's R6 says *"no package ships
platform C of its own"*, `grep -n 'ships platform C' docs/panel/234-*.md`). Run it, under `--sanitize` too: does T1's *"nothing
else crosses"* hold of the program's values only, or at the boundary, and which sentence would be true.

**B2, the stop, on three platforms.** Write and compile the runtime part the proposal names:

- **POSIX**: a `sigaction` handler for SIGINT and SIGTERM that sets an async-signal-safe flag, with
  and without `SA_RESTART`; a loop blocked in `accept` on a loopback socket and one blocked in `read`
  on a pipe, each sent the signal (the binary under `timeout 30`, its output through `head -c`): which
  sees the flag, and what the other choice does to every blocking call a package makes. Then the
  self-pipe or a `poll` with a timeout, if a blocked server needs one.
- **Its neighbours in the runtime**, which it must not break: `runtime/parts/stack.c` takes SIGSEGV
  and SIGBUS (1049-1055); `runtime/parts/os.c` takes SIGTRAP, SIGABRT and SIGILL (488-492);
  `runtime/parts/panic.c:54` restores SIGABRT before `abort()` (defect 2257); `runtime/parts/run.c`
  catches SIGTERM, SIGINT, SIGHUP and SIGQUIT while a child is waited on, only *"where this process had
  left them at their default"* (505-560), and forwards them to the child's group.
- **Windows**: `SetConsoleCtrlHandler` (CTRL_C_EVENT, CTRL_BREAK_EVENT, CTRL_CLOSE_EVENT) and the C
  runtime's `signal(SIGINT)`. Read Microsoft's pages today: on which thread the handler runs (if it
  is one the runtime did not start, `thread.c`'s guard stops a Heroes function bound as one), how long
  the system waits after CTRL_CLOSE_EVENT, and what interrupts a blocked `accept` or `recv`. Written,
  not compiled here (A's search above).
- **Feature 1069's route**: its re-check of 2026-10-25 (`gh issue view 1069 --repo heroes-lang/heroes
  --comments`) built a three-line `stopflag.h`, `static volatile sig_atomic_t` and two `static inline`
  functions, which printed `drained after 3 requests`; it notes the flag is one per C unit. Build it
  again and price it against the runtime part: one module or many, Windows, and panel 234's R6.
- **Running Heroes code in the handler**: 00-shared.md measured a Heroes handler bound through libc's
  `signal` printing `caught 2` at exit 0. Whether a handler that allocates may run is the question
  1069 left open (*"ruled nowhere I searched"*); say what the door must forbid, and how.

**B3, from the C side.** Whether a C route reaches `cow.c`'s `refcount == 1` (`cow.c:44`, `:88`, `:93`,
`map-write.c:128`, `grep -n`) under the rung other than *C releasing a reference it still lends*, which
panel 113's R4 excludes (`docs/panel/113-*.md:246-257`): a library calling a Heroes callback from its
own thread (the guard), a value C hands back that two threads touch. And the descriptor: what type
`socket()` returns in this Mac's `sys/socket.h` and in the container's, and what winsock's `SOCKET` is
by Microsoft's page read today, so whether the `i64` crossing holds it on every platform.
