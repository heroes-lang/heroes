# Panel 247, ffi-pragmatist

Read `00-shared.md` first, whole. You judge every route from the C side, with a veto on ABI breakage
and on soundness at the boundary. Your folder is `.claude/worktrees/p247-scratch/247-ffi-pragmatist/`,
your tree `tree/` in it, a `git archive` of **`ae5e2cedd`** (tree
`3e22d53467b606946a70a6f9dfdea6f46ea47612`), your compiler built there from the seed in two stages
(`sources 7cc70d136b590d46`). Your report is `docs/panel/247-reports/ffi-pragmatist.md` in
`.claude/worktrees/p247`, in your definition's output structure, written as you go. **Compile, don't
opine**: every mechanism below is C you write, compile with clang on this Mac and run, and in Linux
arm64 where the coordinator runs your script; a platform you cannot run on is named as unrun, never
inferred.

## What is there to read first

- The lane's C probes, read-only, copied into your folder before use:
  `.claude/worktrees/b34-scratch/b34-signals/probes/routing.c` (where a signal sent to the process
  goes when every thread blocks it at the instant it is sent, its first comment lines) and
  `probes/blocked_fault.c` (what a fault does on a thread that blocks every signal, with a SIGSEGV and
  SIGBUS handler installed); `sigaction/glue.h` and `sigaction/sa.hero`
  (a Heroes function reaching `sigaction` through a group record's field, refused
  `error[ffi_field_type]`, `sigaction/check.txt`).
- The prototypes: `protoC/runtime/parts/thread.c` (the delivery's return address learned by a SIGURG
  probe at start, a six-frame walk inside the thread's stack bounds, a refusal by `write(2)` and
  `abort()`) and `protoD/runtime/parts/alloc.c` (the allocator's depth, a fixed message by `write(2)`
  and `abort()`), each a copy of the runtime at `62ce17563` plus that change (00-shared.md).
- The runtime's handlers no route may break, 00-shared.md § What the runtime does, with their lines.

## Write and run

1. **The delivery's return address, as C's detection uses it.** On this Mac (Darwin arm64) and in the
   container (Linux arm64), what the probe learns for a plain and an `SA_SIGINFO` handler, and whether
   the walk finds it from a function called by the handler through a C library frame: a handler that
   calls into libc (`qsort` with a callback is one route), built with and without
   `-fomit-frame-pointer`. **The alternate stack**: a handler installed with `SA_ONSTACK` after
   `sigaltstack` (the runtime's own stack guard uses one, `runtime/parts/stack.c:1009` and 1052) runs
   outside the thread's bounds the walk checks: what does protoC's walk answer then? A signal that
   lands while the thread is inside C, inside the kernel (a blocked `read`), and inside the runtime's
   allocator. What a debugger sees of the SIGURG every program would raise at its start, and a program
   that installs its own SIGURG handler before or after it.
2. **The refusal path, async-signal-safe end to end**: `write(2)` then what. POSIX's list of
   async-signal-safe functions (the standard's page, read today, its URL), whether `abort()` flushes
   stdio on Darwin and on glibc (the lane's report says Darwin's does: read each manual page and, if
   you can, show it with a probe whose stdio buffer is held by the interrupted thread), and the
   alternatives (`raise(SIGABRT)` with the default restored, `_exit`), against what panel 246's
   `panic.c:54` restore (defect 2257) and the lease handler (`os.c:471`) expect of a SIGABRT.
3. **E and F from the C side**: the mechanisms a runtime could use to run a program's function later,
   never inside the signal: a self-pipe written by a C handler and read by a thread the runtime starts
   and marks as the program's own; `sigwait` on a dedicated thread with the signal blocked everywhere
   else; kqueue's `EVFILT_SIGNAL` and Linux's `signalfd`. For each: what it does to a handler that
   must run before the program ends (a SIGTERM's cleanup), to a signal sent twice before the thread
   runs (standard signals do not queue: read `sigaction(2)` and say), to an `SA_SIGINFO` handler's
   `siginfo_t *` and context, and to a fault signal (SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT) that
   cannot be deferred. Write the C a program would see, compile it, run `storm.hero`'s shape through it
   (the lane's `r2494/storm.hero` and `storm.py`, copied), and say what the platform layer's stop
   (`runtime/platform/hero_net.c:573-650`, a C handler writing one byte down a pipe) already is of
   this mechanism.
4. **D's residue**: a Heroes handler that prints while `main` prints (stdio's lock), one that calls C's
   `malloc` through a C function while `main` is in C's allocator, the drop lists and the frame room:
   build each as a storm and say which route closes it.
5. **Windows**, compiled with `zig cc -target x86_64-windows-gnu`, `ZIG_GLOBAL_CACHE_DIR` and
   `ZIG_LOCAL_CACHE_DIR` inside your folder, never run (the box is offline, 00-shared.md): UCRT's
   `signal(SIGINT)`, whose handler Microsoft's `signal` page says runs on a thread of its own (read the
   page today and quote it), `SetConsoleCtrlHandler`'s HandlerRoutine, and UCRT's SIGABRT, SIGFPE,
   SIGILL and SIGSEGV, which run where; what each route does there, and whether `hero_thread_guard`'s
   thread question already refuses a Heroes SIGINT handler on Windows (thread.c's comment says so from
   a reading).
6. **A from the C side**: what a name-based refusal of `signal`, `sigset` and `bsd_signal` misses, as
   C a program can ship: a wrapper in its own header that takes a `void (*)(int)` as a PARAMETER and
   calls `sigaction` (not the record field the checker refuses), `pthread_atfork`'s handlers,
   `atexit` from a handler, a library that installs a handler on the program's behalf. Compile each with
   a Heroes function passed in, and run it.

## What your verdict owes

For the route or combination you would adopt: the C it needs on each platform, compiled; what it does
at the alternate stack, at a frame with no record, in a C library, and on Windows; the async-signal
safety of every function it calls inside a handler, from the standard's list; and what it refuses that
runs today. A veto names the program that corrupts memory or breaks the boundary under the route
vetoed, compiled and run.

**Linux arm64**: `linux-247.sh` in your folder, the body of `docs/platforms/linux/LINUX-MACHINE.md`
§ How to run something on one of them (its line 126: a read-only mount copied inside, no
Dockerfile, the seed and your C built in the container, output through `dd bs=1 count=4194304`); the
coordinator runs it with `heroes-linux-arm64:latest` (image `338a1044ac90`, `docker images`) and
hands you the log, the leg unrun in your report until then.
