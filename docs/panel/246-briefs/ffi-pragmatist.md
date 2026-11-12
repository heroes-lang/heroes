# Panel 246, ffi-pragmatist

Read `00-shared.md` first, whole. You judge **Part A** (the soundness lane, with the
compiler-engineer) and **Part B** (the full panel). Your folder is
`.claude/worktrees/p246-scratch/246-ffi-pragmatist/`, your tree `tree/` in it, a `git archive` of
**`ef50a7478`** (tree `084b36b7d5db6b342c281468fff120b2fe98349b`), your compiler built there from the
seed in two stages (`sources 796d1133542b1909`). Your report is
`docs/panel/246-reports/ffi-pragmatist.md` in `.claude/worktrees/p246`, one section per part, in your
definition's output structure, written as you go. **Compile, don't opine**: every door below is C you
write, compile with clang on this Mac and run; a platform you cannot run on is named as unrun, never
inferred.

## Part A: the lock door and what it meets

Defect 2453 is `blocking` since 2026-11-12 (00-shared.md § Part A, with the measurement that made it
so). The runtime has no lock: `grep -rln -E 'flock|LockFileEx|F_SETLK|F_OFD_SETLK|lockf\(|LOCK_EX|
OpenProcess' runtime selfhost tests/harness` finds 0 files at the base. **The door is
compiler-private**, in `runtime/hero_compiler.h` (80 lines), by the lane the author chose (00-shared.md
§ Part A); a door you find must be in `hero_os.h` is named in your report as **outside this lane**, for
the synthesis to return to the author, with what it would owe there (the 241 names of
`heroes_guard_open.h` and `heroes_guard_close.h`, `selfhost/emit/guarded_names.hero`, panel 232's
promise). The runtime already opens with `O_CLOEXEC` (`grep -n O_CLOEXEC runtime/parts/replace.c`),
and the platform layer makes every socket *"never inherited by a program the runtime spawns"*
(`runtime/platform/hero_net.c:249`). Children start by `execvp` (`runtime/parts/run.c:1340`) and the
runtime ends them with a killed parent (`run.c:506-557`, `:734-783`; 00-shared.md § Part A, A3).

**Write and run:**

1. **The POSIX arm** of a door that takes an exclusive lock on a file without waiting, answers
   *taken*, *held by another* or *this file system has none*, and holds it until the process ends:
   `flock(LOCK_EX | LOCK_NB)` against `fcntl(F_SETLK)` (a process loses those when it closes ANY
   descriptor of the file, a question to run) and `F_OFD_SETLK` where the platform has it (whether
   Darwin does is a question). For each: what `kill -9` of the holder does, read from a second
   process; the descriptor close-on-exec, and what the window between a child's `fork` and its
   `execvp` (`run.c:1340`) does to a lock whose open file description the child shares.
2. **The file systems**: on this Mac, a FAT32 and an exFAT image made with `hdiutil create` and
   attached with `-mountpoint` at a folder **inside your folder** (never under `/Volumes`, outside the
   repository's root), the lock asked there; in the Linux arm64 container, its own file system. NFS
   and SMB: unrun unless you find a route inside the hard stops; say what `flock(2)`'s and Darwin's
   manual pages state of them, read today, and what route A does on a file system with no lock.
3. **The Windows arm**, compiled: `CreateFileW` and `LockFileEx(LOCKFILE_EXCLUSIVE_LOCK |
   LOCKFILE_FAIL_IMMEDIATELY)` with `zig cc -target x86_64-windows-gnu`, the caches
   (`ZIG_GLOBAL_CACHE_DIR`, `ZIG_LOCAL_CACHE_DIR`) inside your folder, as the coordinator's
   `p246-scratch/a-win/lock.c` compiled at exit 0 (00-shared.md § Part A, Platforms). A compile, not a
   run: the Windows box is off and its run is the CI's leg after the push. Read Microsoft's pages today
   and say what they state: when a lock is released after its process is ended, whether at once,
   whether a `CreateProcessW` child inherits the handle, and whether a folder holding an open locked
   file can be removed.
4. **PID reuse, a live owner, a folder with no lock**: what the lock decides that the number in
   `.fetching-<pid>-<n>` cannot; the coordinator's measurement (00-shared.md: a later `fetch` or `check`
   handed the dead number removed the edit at exit 0, and a killed `check` left a folder with no lock
   file); route D's refusal of a standing name (`selfhost/cli/stub.hero:175-176`), from the C side.
5. **Linux arm64**: `linux-a.sh` in your folder, the body of `docs/platforms/linux/LINUX-MACHINE.md` §
   How to run something (a read-only mount copied inside, no Dockerfile), building the seed and your
   C probes in the container, every case `git init -q`ing its own repository and none needing `curl`
   (the image has none); the coordinator runs it with `heroes-linux-arm64:latest` and hands you the log.
6. **Rule 3**: the held descriptor is an object a runtime part keeps between calls, so
   `tests/harness/suite_runtime.hero`'s Rule 3 (line 148) judges it: `_Thread_local`, `_Atomic` or a
   `SHARED_BY_DECISION` line with its reason, as `runtime/parts/panic.c:32`'s `hero_runtime_spoke` is
   (`suite_runtime.hero:233`). Say which and why.

Then A1 from your seat: whether a lock file private to one run is the *"fifth input class"* panel
235's R12 refused, read beside its R7 (*"no lock between two programs is needed"*) and R19 (the verbs'
output as goldens), all three quoted in 00-shared.md.

## Part B: the server step

**B1, the rung at the C boundary.** `hero_thread_spawn(int64_t (*body)(int64_t), int64_t arg)` at
`runtime/hero_os.h:435`, `hero_thread_spawn_sized` 441, `hero_thread_join` 443, `hero_thread_limit`
447, the comment at 430-434 false today (00-shared.md); the arms in `runtime/parts/spawn.c` (444 lines),
the guard in `runtime/parts/thread.c` (99). Write the C a program could ship to carry more than a
number through the `i64`: a header of its own whose `static inline` turns a `cstr`, a `ptr` or a
`hero_net_socket *` into an `int64_t` and back (a program may ship one by panel 114's R2, ratified and
never written into the spec; a package may not by panel 234's R6). Run it, under `--sanitize` too:
does T1's *"nothing else crosses"* hold of the program's values only, or at the boundary, and which
sentence would be true. And the C of a body that takes a `str` (panel 113's R5 rung, ratified, never
shipped): what the runtime's arm must do for it on each platform.

**B2, the stop, on three platforms.** Write and compile the runtime part each route needs:

- **POSIX, with a handler**: `sigaction` for SIGINT and SIGTERM setting an async-signal-safe flag,
  with and without `SA_RESTART`; a loop blocked in `hero_net_accept` on a loopback listener and one
  blocked in `read` on a pipe, each sent the signal (the binary under `timeout 30`, its output through
  `head -c`). `runtime/platform/hero_net.c:509-512` asks `accept` again after `EINTR`, so the first
  loop never returns to ask: measure it, and what each choice does to every other blocking call.
- **POSIX, without one**: route (a), SIGINT and SIGTERM blocked with `pthread_sigmask` in every thread
  and `sigpending` asked between steps; route (b), the stop as a descriptor (a self-pipe, kqueue's
  `EVFILT_SIGNAL`, Linux's `signalfd`) in one `poll` beside the listener; route (c), the stop inside
  the layer's own `accept` and `recv`, and whether `hero_net_close` from another thread wakes a blocked
  `accept` on a listening socket by its `shutdown` (`hero_net.c:240-247`; `hero_net.h:93`), on Darwin
  and in the container.
- **Which thread runs a POSIX handler**: a process-directed SIGINT with a thread C made alive (a
  `pthread_create` from the program's C) and with a `hero_thread_spawn` worker alive, on this Mac and in
  the container; where a thread C made takes it, `runtime/parts/thread.c`'s guard stops a Heroes
  handler (panel 114's R4, exit 134).
- **Windows**: `SetConsoleCtrlHandler` (CTRL_C_EVENT, CTRL_BREAK_EVENT, CTRL_CLOSE_EVENT) and the C
  runtime's `signal(SIGINT)`, compiled with `zig cc` as Part A's arm; read Microsoft's pages today: on
  which thread the handler runs (if one the runtime did not start, the guard stops a Heroes function
  bound as one), how long the system waits after CTRL_CLOSE_EVENT, and what wakes a blocked `accept`
  (`hero_net.c:146-152` asks again after `WSAEINTR`).
- **Its neighbours in the runtime**, which no route may break: `runtime/parts/stack.c` takes SIGSEGV
  and SIGBUS (1049-1055); `runtime/parts/os.c` takes SIGTRAP, SIGABRT and SIGILL (488-492);
  `runtime/parts/panic.c:54` restores SIGABRT before `abort()` (defect 2257); `runtime/parts/run.c`
  catches SIGTERM, SIGINT, SIGHUP and SIGQUIT while a child is waited on (518-525), four where S1 to
  S3 take two: say why two, or four.
- **The program's own header** (feature 1069's re-check, `gh issue view 1069 --repo heroes-lang/heroes
  --comments`): measured at the base, one module drains after 3 requests and two modules run to the
  probe's cap of 1000, the handler's flag in one unit and the loop's in another
  (`p246-scratch/a-probes/stop1`, `stop2`). Price it against the runtime part: whether any C a
  header alone can hold gives one flag per program rather than one per unit (a question to compile,
  on all three platforms), and whether Heroes can hold the rule that makes it safe.
- **Rule 3** judges the flag as it judges Part A's descriptor; `hero_runtime_spoke` is the precedent of
  a `volatile sig_atomic_t` a handler path sets (`suite_runtime.hero:233`).

**B3, from the C side.** A connection is a handle: panel 234's R3 (*"a descriptor ... is wrapped in C
as a struct behind a pointer so it carries the marks"*, `sed -n 321,324p docs/panel/234-*.md`), and at
the base `runtime/hero_net.h:45-46` (`hero_net_socket`), whose comment at 21-23 says *"Windows'
`SOCKET` is an unsigned 64-bit handle"*. So the question is no longer whether an `i64` holds a
descriptor on every platform but whether one connection per thread can be written: price, as C, the
numbered handle (a table index the layer issues and checks, against R3's *never another socket that
took its number*, `hero_net.h:33-36`), a spawn door that takes a handle and what moving it across the
boundary does to its marks, and one thread. Then whether a C route reaches `cow.c`'s `refcount == 1`
(`cow.c:44`, `:88`, `:93`, `map-write.c:128`) under the rung other than *C releasing a reference it
still lends*, which panel 113's R4 excludes (`docs/panel/113-*.md:246-257`): a library calling a Heroes
callback from its own thread, a value C hands back that two threads touch, and the shapes feature 925
names as untried (`sort` through a function pointer, the `eq` and `hash` descriptor walks, the
drop-list drain under contention) where C is the one calling.
