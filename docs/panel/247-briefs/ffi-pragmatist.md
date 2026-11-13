# Panel 247, ffi-pragmatist

Read `00-shared.md` first, whole: **R4 is a ratified rule and this sitting rules how it is enforced
or honoured**, D is the floor, and the class is Heroes code inside a signal reaching C that is not
safe there. You judge every route from the C side, with a veto on ABI breakage and on soundness at
the boundary. Your folder is `.claude/worktrees/p247-scratch/247-ffi-pragmatist/`, your tree `tree/`
in it, a `git archive` of **`ae5e2cedd`** (tree `3e22d53467b606946a70a6f9dfdea6f46ea47612`), your
compiler built there from the seed in two stages (`sources 7cc70d136b590d46`), then D applied and
stage 2 built again (00-shared.md § D, the floor). Your report is
`docs/panel/247-reports/ffi-pragmatist.md` in `.claude/worktrees/p247`, in your definition's output
structure, written as you go. **Compile, don't opine**: every mechanism below is C you write, compile
with clang on this Mac and run, and in Linux arm64 where the coordinator runs your script; a platform
you cannot run on is named as unrun, never inferred. **Every Darwin storm reads its crash reports**
(`p247-scratch/tools/ips.py`, 00-shared.md § The rules): the deaths that matter here say nothing on
stderr.

## What is there to read first

- The lane's C probes, read-only, copied into your folder before use:
  `.claude/worktrees/b34-scratch/b34-signals/probes/routing.c` (where a signal sent to the process
  goes when every thread blocks it at the instant it is sent, its first comment lines) and
  `probes/blocked_fault.c` (what a fault does on a thread that blocks every signal, with a SIGSEGV and
  SIGBUS handler installed); `sigaction/glue.h` and `sigaction/sa.hero` (a Heroes function reaching
  `sigaction` through a group record's field, refused `error[ffi_field_type]`, `sigaction/check.txt`);
  and route A's measured hole, a wrapper taking the handler as a parameter
  (`.claude/worktrees/p247-scratch/coord/wrap/`, 00-shared.md: it ran a formatting Heroes handler
  inside the signal at exit 0).
- The prototypes: `protoC/runtime/parts/thread.c` (the delivery's return address learned by a SIGURG
  probe at start, a six-frame walk inside the thread's stack bounds, a refusal formatted with
  `snprintf`, written with `write(2)`, then `abort()`) and `protoD/runtime/parts/alloc.c`; and D as it
  landed, `git -C .claude/worktrees/b34-round show 932876f95 -- runtime/parts/alloc.c` (its stop
  written by `write(2)`, SIGABRT's default restored and raised, `_exit` after).
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
2. **Every refusal and every stop, async-signal-safe end to end.** protoC's refusal formats with
   `snprintf` inside the handler and **lost its own message in 1 of 3 `quiet` runs**: its crash report
   (pid 75776, 00-shared.md) is the locale lock's SIGKILL, taken by the refusal's `snprintf` over
   `main`'s. The base's 2492 refusal formats with `snprintf` and ends through `hero_panic`'s
   `fflush`, `fprintf` and `abort()`; and `abort()` inside a handler on an exiting thread ended by
   Darwin's `__abort` trap, SIGTRAP (two reports, 00-shared.md). So: POSIX's list of async-signal-safe
   functions (the standard's page, read today, its URL), the route's refusal written with nothing off
   it (D's shape is one: a fixed text by `write(2)`, SIGABRT's default restored, `raise`, `_exit`),
   and what exit status a refusal then ends with on Darwin and in Linux, measured, since a case's
   `.expected` pins it.
3. **The class, from the C side**: which of the runtime's own calls a Heroes body reaches are off the
   async-signal-safe list (`snprintf` through `hero_int_to_str`, `runtime/parts/f64.c:267-273`, and
   the file's other calls of `snprintf(`; stdio through `print`; `exit` through `hero_exit`,
   `runtime/parts/os.c:1005-1011`; `malloc` and `free`), counted from the runtime's source, so every
   route is judged against the list and not against the allocator alone.
4. **What glibc does where Darwin kills.** On Linux arm64 D's case read 0 of 50 and 0 of 20 sanitized
   off the stop (`b34-scratch/b34-signals/c2494/linux-2494*.log`), and glibc's `snprintf` takes no
   lock there (D's comment): run a handler's `snprintf` interrupting `main`'s in the container, under
   ASan and under glibc's own checks (`MALLOC_CHECK_=3`, `GLIBC_TUNABLES=glibc.malloc.check=3`, read
   its manual first), and say whether the result is harmless or silent corruption.
5. **E, S and F from the C side**: the mechanisms a runtime could use to run a program's function
   later, never inside the signal: a self-pipe written by a C handler and read by a thread the runtime
   starts and marks as the program's own (E); a flag the C handler sets and compiled code polls at its
   safe points on the thread itself (S); `sigwait` on a dedicated thread with the signal blocked
   everywhere else; kqueue's `EVFILT_SIGNAL` and Linux's `signalfd`. For each: what it does to a
   handler that must run before the program ends (a SIGTERM's cleanup), to a signal sent twice before
   the deferred call runs (standard signals do not queue: read `sigaction(2)` and say), to an
   `SA_SIGINFO` handler's `siginfo_t *` and context, and to a fault signal (SIGSEGV, SIGBUS, SIGFPE,
   SIGILL, SIGABRT) that cannot be deferred. Write the C a program would see, compile it, run
   `storm.hero`'s shape through it (the lane's `r2494/storm.hero` with the shared driver,
   `p247-scratch/tools/storm.py`, copied, run unwrapped), and say what the platform layer's stop
   (`runtime/platform/hero_net.c:573-650`, a C handler writing one byte down a pipe) already is of
   this mechanism. And F's case alone (00-shared.md): a Heroes function called from the program's own
   C handler, which expects the call done when it returns.
6. **G and I from the C side** (00-shared.md): G, a door the runtime owns for signals other than
   SIGINT and SIGTERM, R4's byte down a pipe offered for any signal and counted, read between steps:
   write it, run a SIGUSR1 counter through it under the storm, and say what it would add to
   `hero_net.h` or `hero_os.h` (R4's veto refuses a *stop* door, `docs/panel/246-*.md:351-355`). I,
   the handler made safe in place (a handler-time arena, an integer formatter without `snprintf`,
   `print` by `write(2)`): price what it would have to cover, from the list of item 3; it leaves Heroes
   code inside the signal, which changes R4: say so if you recommend it.
7. **Windows**, compiled with `zig cc -target x86_64-windows-gnu`, `ZIG_GLOBAL_CACHE_DIR` and
   `ZIG_LOCAL_CACHE_DIR` inside your folder, never run (the box is offline, 00-shared.md): UCRT's
   `signal(SIGINT)`, whose handler Microsoft's `signal` page says runs on a thread of its own (read the
   page today and quote it), `SetConsoleCtrlHandler`'s HandlerRoutine, and UCRT's SIGABRT, SIGFPE,
   SIGILL and SIGSEGV, which run where; what each route does there, and whether `hero_thread_guard`'s
   thread question already refuses a Heroes SIGINT handler on Windows (thread.c's comment says so from
   a reading).
8. **A from the C side**: what a name-based refusal of `signal`, `sigset` and `bsd_signal` misses, as
   C a program can ship: `wrap` (run, 00-shared.md), `pthread_atfork`'s handlers, `atexit` from a
   handler, a library that installs a handler on the program's behalf. Compile each with a Heroes
   function passed in, and run it.

## What your verdict owes

For the route or combination you would adopt: whether it enforces R4, honours it, or changes it (the
author's call); the C it needs on each platform, compiled; what it does at the alternate stack, at a
frame with no record, in a C library, and on Windows; the async-signal safety of every function it
calls inside a handler, from the standard's list; and what it refuses or changes that runs today. A
veto names the program that corrupts memory, dies in silence or breaks the boundary under the route
vetoed, compiled and run.

**Linux arm64**: `linux-247.sh` in your folder, the body of `docs/platforms/linux/LINUX-MACHINE.md`
§ How to run something on one of them (its line 126: a read-only mount copied inside, no Dockerfile,
the seed and your C built in the container); bound its output yourself, as panel 246's
`.claude/worktrees/p246-scratch/246-ffi-pragmatist/linux-a.sh` did with `dd bs=1 count=4194304`
(`grep -c -F count=4194304` there: 1; the Linux page itself names no output bound). The coordinator
runs it with `heroes-linux-arm64:latest` (image `338a1044ac90`, `docker images`) and hands you the
log, the leg unrun in your report until then.
