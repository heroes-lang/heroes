# Panel 247, compiler-engineer

Read `00-shared.md` first, whole. You judge every route from the compiler's and the runtime's side,
with a veto on soundness. Your folder is `.claude/worktrees/p247-scratch/247-compiler-engineer/`,
your tree `tree/` in it, a `git archive` of **`ae5e2cedd`** (tree
`3e22d53467b606946a70a6f9dfdea6f46ea47612`), your compiler built there from the seed in two stages
(00-shared.md § The base, `sources 7cc70d136b590d46`). Your report is
`docs/panel/247-reports/compiler-engineer.md` in `.claude/worktrees/p247`, in your definition's output
structure, written as you go. **Every route you weigh, you build** in your tree and run on the cases
below; a route that does not build is reported as not built, never adopted (`/panel` 3c). Line counts
are `wc -l` at the base; where a count decides anything, ask `tests/harness/suite_layout.hero`'s unit.

## Where the code is (`wc -l` and `grep -n` at the base)

- `runtime/parts/thread.c`, 116 lines: `hero_thread_is_home` 74, `hero_thread_claim` 80, the comment
  82-101, `hero_thread_guard` 102-116.
- `selfhost/emit/callback_guard.hero`, 207: which functions C may call back (`callbacks_of`,
  `addresses_taken`, `takes_a_callback`) and `entry`, the guard line at a body's start.
- `selfhost/emit/callback_thunk.hero`, 296: the thunk C is handed, `WHERE, AND WHY NOT AT THE
  FUNCTION'S ENTRY` at 16-24, and the table `hero_callback_of` that finds it.
- `selfhost/emit/body.hero`, 397 (the prologue that writes `entry`'s line, defect 472).
- `selfhost/check/ffi.hero`, 412 (the callback rule, its doc from 47; `ffi_callback_carries` at 150);
  `selfhost/ffi_errors.hero`, 380, the FFI's diagnostics.
- `runtime/parts/alloc.c`, 1419 (D's sites); `os.c`, 1019 (the lease handler, 471 and 488; where
  protoC calls `hero_signal_learn`, one line after its 513 at `62ce17563`); `stack.c`, 1213 (the
  stack guard's handlers 1054-1055, `sigaltstack` 1009, `hero_stack_lo` and `hero_stack_hi` 335-336);
  `panic.c`, 148 (54); `run.c`, 1962 (518, 681-702); `runtime/platform/hero_net.c`, 1233 (the stop,
  573-650).
- The lane's prototypes, read-only, copied into your folder before use:
  `.claude/worktrees/b34-scratch/b34-signals/protoC/runtime/` and `protoD/runtime/`, each a copy of
  the runtime at `62ce17563` with the change 00-shared.md names (`diff -u` each against `git -C
  .claude/worktrees/b34-round archive 62ce17563 runtime`, and apply the change to your base's runtime,
  which also holds defect 2492's message, rather than copying their files over yours).

## The cases every route is run on

Copy each into your folder; never run them inside the lane's.

- `b34-scratch/b34-signals/r2494/storm.hero`, `quiet.hero` and the driver `storm.py` (`python3 -I
  storm.py <binary> 4 0.3`: 0.3 s to arm, SIGUSR1 as fast as `kill` returns for 4 s, then 30 s
  allowed and SIGKILL to its group), 5 runs of `storm` and 3 of `quiet` per route, as the lane ran.
- `b34-scratch/b34-signals/t3x1/t3x1.hero`, sent `kill -INT` at 2.5 s (`t3x1/t3run.py`).
- The three tracked programs that hand C a Heroes handler (00-shared.md § What the runtime does), run
  through `./heroes-next run tests/harness/main.hero -- ./heroes-next run <case-substring>` in your
  tree, one at a time.
- `b34-scratch/b34-signals/cost/guarded.hero`: a guarded function called 50,000,000 times through its
  value; and the compiler's own work, `heroes check selfhost/main.hero` and a warm `heroes build
  selfhost/main.hero`, each compiler built from the same tree with the route and without; counted in
  instructions (`/usr/bin/time -l`), three runs each, never seconds.

## The routes, each built

1. **C in its thunk form.** Move protoC's question out of `hero_thread_guard`, which every call of a
   guarded function passes, into the thunk C is handed (`callback_thunk.hero`), so a Heroes call by
   name pays nothing: what the emitted thunk calls, and what `hero_callback_of`'s table changes.
   Measure `guarded.hero` (the lane's prototype read 4,970,360,692 instructions on its base runtime
   and 19,976,640,456 with the check in the guard, `cost/gtime-*.txt`) and a guarded function C calls
   in a loop (`qsort` over a large array with a Heroes comparator is one; write it), the base against
   the thunk form. Then the detection's exceptions from your side: a handler entered on the alternate
   stack (the walk stops at the thread's bounds, `hero_stack_lo`/`hi`, so what does it answer?), a
   frame with no frame record between the delivery and the thunk, a handler the program's own C
   installs and that calls a Heroes function, and every start's SIGURG (what `lldb` and a program that
   handles SIGURG itself see; `tests/golden/lldb/` and `suite_lldb` are where a session's frames are
   pinned).
2. **A**: a refusal at build of a Heroes function passed to `signal`, `sigset` or `bsd_signal`, where
   it goes (the checker or the FFI's rule), its code and words, its `Fix` and whether it is `certain`
   (a C handler the program writes is not a fix the compiler can write for it), and what it refuses
   at build: the three tracked programs, `quiet`, `t3x1`. Build it far enough to run `check` on them.
   What it misses: a program's own C wrapper that takes the handler as a parameter and calls
   `sigaction` (write `glue.h` that way, with a parameter, not the field the lane's `sigaction/sa.hero`
   tried and the checker refused `error[ffi_field_type]`).
3. **E**: the function run later on a runtime thread. Say what the compiler must know to choose it
   (the callee's name at build, as A; or the delivery detected at run time, as C, which is F), what
   the runtime part is (a self-pipe or `sigwait` thread the runtime claims as the program's own, so
   `hero_thread_guard` passes), and what it does to the runtime's own handlers (00-shared.md lists
   them) and to `run.c`'s forwarding. Build it if you can, at least the deferring thunk for a plain
   `void (*)(int)` handler, and run `storm`, `quiet` and `t3x1` on it.
4. **F**: C's detection with E's delivery. What a deferred call does with an `SA_SIGINFO` handler's
   two pointers, and whether a fault signal (SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT) can ever be
   deferred: the runtime's own handlers take SIGSEGV, SIGBUS, SIGTRAP, SIGABRT and SIGILL, so say what
   a program's handler for one of them means under each route.
5. **D**, as landed or as `protoD` (00-shared.md § The base says which stands when you read): whether
   the route you would adopt keeps it, makes it moot or needs it, and **the compiler's own cost of D**
   in instructions, `heroes check selfhost/main.hero` and a warm build, if the lane's commit has not
   reported it when you read.

## What your verdict owes

For the route or combination you would adopt: its lines per file, what moves (the compiler's own
tests, the `run` cases above, `suite_runtime`'s Rule 3 for any object a runtime part keeps between
calls, the emissions where `runtime/` or `selfhost/emit/` moved), what it refuses that runs today,
and its cost in instructions; a veto, if you cast one, names the program that corrupts memory under
the route vetoed, built and run.
