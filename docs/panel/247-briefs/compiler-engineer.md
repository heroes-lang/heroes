# Panel 247, compiler-engineer

Read `00-shared.md` first, whole: **R4 is a ratified rule and this sitting rules how it is enforced
or honoured**, and D is the floor. You judge every route from the compiler's and the runtime's side,
with a veto on soundness. Your folder is `.claude/worktrees/p247-scratch/247-compiler-engineer/`, your
tree `tree/` in it, a `git archive` of **`ae5e2cedd`** (tree `3e22d53467b606946a70a6f9dfdea6f46ea47612`),
your compiler built there from the seed in two stages (`sources 7cc70d136b590d46`), then **D applied
and stage 2 built again** (00-shared.md § D, the floor). Your report is
`docs/panel/247-reports/compiler-engineer.md` in `.claude/worktrees/p247`, in your definition's output
structure, written as you go. **Every route you weigh, you build** in your tree and run on the cases
below; a route that does not build is reported as not built, never adopted (`/panel` 3c). Line counts
are `wc -l` at the base; where a count decides anything, ask `tests/harness/suite_layout.hero`'s unit.

## Where the code is (`wc -l` and `grep -n` at the base)

- `runtime/parts/thread.c`, 116 lines: `hero_thread_is_home` 74, `hero_thread_claim` 80, the comment
  82-101, `hero_thread_guard` 102-116 (its refusal formatted with `snprintf` at 105-114, ended by
  `hero_panic` at 115).
- `runtime/parts/panic.c`, 148: `hero_panic`'s `fflush(stdout)`, `fprintf(stderr, ...)` and
  `hero_abort()` at 59-61; SIGABRT restored at 54.
- `runtime/parts/f64.c`: `hero_int_to_str` at 267-273, `snprintf` at 269 (the storm's deaths, 00-shared.md).
- `selfhost/emit/callback_guard.hero`, 207: which functions C may call back (`callbacks_of`,
  `addresses_taken`, `takes_a_callback`) and `entry`, the guard line at a body's start.
- `selfhost/emit/callback_thunk.hero`, 296: the thunk C is handed, `WHERE, AND WHY NOT AT THE
  FUNCTION'S ENTRY` at 16-24, and the table `hero_callback_of` that finds it.
- `selfhost/emit/body.hero`, 397 (the prologue that writes `entry`'s line, defect 472).
- `selfhost/check/ffi.hero`, 412 (the callback rule, its doc from 47; `ffi_callback_carries` at 150);
  `selfhost/ffi_errors.hero`, 380, the FFI's diagnostics.
- `runtime/parts/alloc.c`, 1419 at the base and D's 141 lines more at `932876f95`; `os.c`, 1019 (the
  lease handler, 471 and 488; `hero_exit` at 1005-1011; where protoC calls `hero_signal_learn`, one
  line after its 513 at `62ce17563`); `stack.c`, 1213 (the stack guard's handlers 1054-1055,
  `sigaltstack` 1009, `hero_stack_lo` and `hero_stack_hi` 335-336); `run.c`, 1962 (518, 681-702);
  `runtime/platform/hero_net.c`, 1233 (the stop, 573-650).
- The lane's prototypes, read-only, copied into your folder before use:
  `.claude/worktrees/b34-scratch/b34-signals/protoC/runtime/` and `protoD/runtime/`, each a copy of
  the runtime at `62ce17563` with the change 00-shared.md names (`diff -u` each against `git -C
  .claude/worktrees/b34-round archive 62ce17563 runtime`, and apply the change to your tree, which
  also holds defect 2492's message and D, rather than copying their files over yours).

## The cases every route is run on

Copy each into your folder; never run them inside the lane's.

- `b34-scratch/b34-signals/r2494/storm.hero` and `quiet.hero`, each under the shared driver
  (`python3 -I storm.py <binary> SIGUSR1 4 0.3`, `p247-scratch/tools/storm.py` copied, run unwrapped),
  5 runs of `storm` and 3 of `quiet` per route, **and every death read in its crash report**
  (`python3 -I ips.py ~/Library/Logs/DiagnosticReports storm quiet`, `p247-scratch/tools/ips.py`
  copied): a route is judged on what the reports say, since the lock's SIGKILL writes nothing to
  stderr.
- `b34-scratch/b34-signals/t3x1/t3x1.hero`, sent `kill -INT` at 2.5 s (`t3x1/t3run.py`).
- `p247-scratch/coord/wrap/wrap.hero` and `wrap.h`, route A's hole (00-shared.md), built and run under
  `timeout -s KILL 30`.
- The tracked programs that hand C a handler, run through `./heroes-next run tests/harness/main.hero
  -- ./heroes-next run <case-substring>` in your tree, one at a time: `fixedbugs-2496` (its
  `.expected`'s *"handled on the worker they were sent to"* is a claim each route keeps or falsifies:
  say which) and `fixedbugs-2257` (three cases; two Heroes handlers, one calling `exit`; one C
  handler).
- `b34-scratch/b34-signals/cost/guarded.hero`: a guarded function called 50,000,000 times through its
  value; and, for a route that moves the compiler's own code paths, `heroes check selfhost/main.hero`
  and a warm `heroes build selfhost/main.hero`, each compiler built from the same tree with the route
  and without, counted in instructions (`/usr/bin/time -l`), three runs each, never seconds. D's own
  cost is its commit's (00-shared.md); measure it again only where your route changes D.

## The routes, each built or priced

1. **C in its thunk form, with a refusal that is async-signal-safe end to end.** Move protoC's
   question out of `hero_thread_guard`, which every call of a guarded function passes, into the thunk
   C is handed (`callback_thunk.hero`), so a Heroes call by name pays nothing; what the emitted thunk
   calls, and what `hero_callback_of`'s table changes. **Its refusal is written without `snprintf`,
   stdio or `abort()`**: protoC's `snprintf` refusal died of the very lock it was refusing in 1 of its
   3 `quiet` runs (00-shared.md, pid 75776's report), and `hero_panic`'s path (`fflush`, `fprintf`,
   `abort()`) is no better; the function's name copied byte by byte into a fixed buffer and
   `write(2)`, then SIGABRT's default restored and `raise`, `_exit` after, is D's own shape
   (`932876f95:runtime/parts/alloc.c` from 539). Measure `guarded.hero` (the lane's prototype read
   4,970,360,692 instructions on its base runtime and 19,976,640,456 with the check in the guard,
   `cost/gtime-*.txt`) and a guarded function C calls in a loop (`qsort` over a large array with a
   Heroes comparator is one; write it), D's compiler against the thunk form. Then the detection's
   exceptions from your side: a handler entered on the alternate stack (the walk stops at the thread's
   bounds, `hero_stack_lo`/`hi`, so what does it answer?), a frame with no frame record between the
   delivery and the thunk, `wrap`'s handler, and every start's SIGURG: **one `lldb` session by hand**
   on a program that binds a callback, saying whether the SIGURG stops it and what a user sees; the
   `lldb` suite is not asked (00-shared.md allows whole suites only to a route that changes what the
   checker refuses), and if your route would move what `tests/golden/lldb/` pins, say so instead.
2. **A**: a refusal at build of a Heroes function passed to `signal`, `sigset` or `bsd_signal`, where
   it goes (the checker or the FFI's rule), its code and words, its `Fix` and whether it is `certain`
   (a C handler the program writes is not a fix the compiler can write for it), and what it refuses
   at build: the tracked programs, `quiet`, `t3x1`. Build it far enough to run `check` on them; `wrap`
   is its measured hole (00-shared.md).
3. **E**: the function run later on a thread the runtime owns. Say what the compiler must know to
   choose the deferring thunk (the callee's name at build, as A, which inherits `wrap`; or the
   delivery detected at run time, which is F), what the runtime part is (a self-pipe or `sigwait`
   thread the runtime claims as the program's own, so `hero_thread_guard` passes), and what it does to
   the runtime's own handlers (00-shared.md lists them) and to `run.c`'s forwarding. Build at least
   the deferring thunk for a plain `void (*)(int)` handler and run `storm`, `quiet`, `t3x1` and the
   2496 case on it.
4. **S, the safe point** (00-shared.md): the runtime's C handler records the signal and compiled
   code runs the Heroes function at its next safe point on the thread itself. Where the poll goes
   (loop heads, function entries, the allocator's return; the set `callback_guard.hero` already
   computes bounds which programs pay), what it costs in instructions on `guarded.hero` and on a tight
   loop, how long a signal waits while the thread is blocked in C (Python's caveat), and what it makes
   of the 2496 case. Build it far enough to measure, or say why it cannot be built in the time.
5. **F**: C's detection with E's or S's delivery. **Its case alone**: a Heroes function called from a
   handler the program's own C installed, which expects the call done, or a result, when it returns;
   say how F knows it must refuse there and build that case. What a deferred call does with an
   `SA_SIGINFO` handler's two pointers, and whether a fault signal (SIGSEGV, SIGBUS, SIGFPE, SIGILL,
   SIGABRT) can ever be deferred: the runtime's own handlers take SIGSEGV, SIGBUS, SIGTRAP, SIGABRT and
   SIGILL, so say what a program's handler for one of them means under each route.
6. **H, a refusal by the handler's body** (00-shared.md): how the checker would know a function
   becomes a handler (A's question), what list of C it would admit, and what it admits that leaves
   Heroes code inside the signal, which changes R4: say so if you recommend it.
7. **D's place**: whether the route you would adopt keeps D, makes it moot or needs it as the backstop
   for what it cannot see; and whether D's words (00-shared.md) stay true under it.

## What your verdict owes

For the route or combination you would adopt: whether it enforces R4, honours it, or changes it (the
author's call); its lines per file, what moves (the compiler's own tests, the `run` cases above,
`suite_runtime`'s Rule 3 for any object a runtime part keeps between calls, the emissions where
`runtime/` or `selfhost/emit/` moved), what it refuses or changes that runs today, its cost in
instructions, and its crash reports read; a veto, if you cast one, names the program that corrupts
memory or dies in silence under the route vetoed, built and run.
