# Panel 247, compiler-engineer's report

Written as the sitting goes, started 2026-11-13 05:32 (`uptime`, load 6.17 with a round gate running
four suites beside it, `pgrep -fl heroes`). Every number carries the command that produced it. No
duration below is a measurement: costs are instructions retired (`/usr/bin/time -l`).

**Corrected before hand-over, the same morning**: section 2 said *"14 deaths"* where its table holds
13; section 11 and the verdict said *"20 refusals"* before the reports of seven later runs and two
sanitized ones were read: **22 refusals under C, 22 crash reports, every one through the refusal's own
`raise`** (`ips.py`, read 06:40). `thread.c` grew after section 2's count, by the bounds read once and
the sanitizer's guard: **116 to 405 lines, route C's part 200, F's 88** (`grep -n` of the block's
bounds), where sections 2 and 3 say 397, 281 and 101.

## 0. Setup, as run (folder `.claude/worktrees/p247-scratch/247-compiler-engineer/`)

- Base: `git -C .claude/worktrees/b34-round rev-parse ae5e2cedd 'ae5e2cedd^{tree}' 932876f95 932876f95^`
  printed `ae5e2cedddee93e0d53715a5b5ae6d7833c569df`, `3e22d53467b606946a70a6f9dfdea6f46ea47612`,
  `932876f9585817ec888d1ba875c880f2f1a4b3e3` and, as D's parent, the base. `git archive` of the base
  into `tree/`; `git check-ignore -v`: `.gitignore:34:.claude/worktrees/`. `TMPDIR=$PWD/../tmp` for
  every build and run.
- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0. Stage 2: `./heroes build
  selfhost/main.hero -o heroes-next`, exit 0, 890,977,089,538 instructions retired; `--version`:
  `heroes 0.6.0`, **`sources 7cc70d136b590d46`**, the base. Kept as `tree/heroes-base`.
- D: `git diff ae5e2cedd 932876f95 -- runtime/parts/alloc.c > d.patch` (238 lines), `patch -p1`, exit 0;
  `shasum -a 256 runtime/parts/alloc.c` = `git show 932876f95:runtime/parts/alloc.c | shasum -a 256` =
  `5bea9640...`; `wc -l`: 1560. Stage 2 again, exit 0, 891,032,000,070 instructions; `--version`:
  **`sources 560c95206c908459`**, the brief's line. Kept as `tree/heroes-d`; `heroes-d doctor`: `ok
  runtime runtime` (the copy's own).

## 1. Where the briefs fall short, first

- `p247-scratch/coord/wrap/wrap.h`'s comment says *"sigaction installs it with SA_ONSTACK|SA_RESTART"*;
  its code sets `sa.sa_flags = SA_RESTART;` alone (read). So `wrap` does not exercise the alternate
  stack, which question 4 asks about; this seat builds `wraponstack` beside it (below).
- The 2496 case's line is computed from a count any thread raises (`fresh_heard`, an atomic of its
  header; `seen` sums `fresh_wait_heard(...) - round`), so it prints its `.expected` whichever thread
  ran the handler: a deferring route keeps the bytes and falsifies the words *"on the worker they were
  sent to"* (read; run below).
- D's own case (`fixedbugs-2494-...-stops-its-program-by-name`, at the round's head, `git show
  932876f95:<it>`) hands `signal` a Heroes function directly: every route but D alone changes its
  `.expected` (a refusal at delivery prints the refusal's words, not D's; a deferral ends with no stop;
  A refuses it at build). The spec-warden found the same count of tracked programs (four).

## 2. Route C in its thunk form, built and run (with D)

**What was built** (folder `tree/`, D applied; diffs kept beside it as `thread-base.c` and
`callback_thunk-base.hero`):

- `selfhost/emit/callback_thunk.hero` (296 to 324 lines, `wc -l`; 28 lines added, `diff | grep -c
  '^>'`): the thunk's first line is now `hero_callback_entered("<module.fn>",
  __builtin_return_address(0), __builtin_frame_address(0), <again>, <arg>)`, and `hero_callback_of`
  calls `hero_signal_learn()` first. For a function taking one `i32` and returning nothing the line
  is `if (...(..., h_0cb_<fn>, h0_sig)) return;` (route F's hook, inert under C); every other thunk
  passes `NULL, 0`. Emitted for `storm.hero` (`heroes-c build storm.hero --emit-c`):
  `if (hero_callback_entered("storm.on_signal", __builtin_return_address(0),
  __builtin_frame_address(0), h_0cb_storm_on_signal, h0_sig)) return;`.
- `runtime/parts/thread.c` (116 to 397 lines; 281 added, of which F's deferral is 101 behind
  `HERO_SIGNAL_DEFER`): the probe (protoC's SIGURG to a plain and an `SA_SIGINFO` probe, run **once,
  at the first crossing** of a function into C, not at every start, so a program that hands C no
  function never raises it); **the exact test**, the thunk's own return address against the
  delivery's, which needs no frame record; **the bounded walk** (`HERO_SIGNAL_WALK`, 8) of frame
  records inside the thread's stack or its alternate stack (`hero_stack_lo`/`hi`,
  `hero_stack_alt`), for a handler of the program's own C that calls the Heroes function; and the
  refusal, **async-signal-safe end to end**: the name copied byte by byte into a fixed buffer, one
  `write(2)`, SIGABRT's default restored and unblocked, `raise`, `_exit(134)` after: no `snprintf`,
  no stdio, no `abort()`, D's own shape.
- One line each in `runtime/heroes_runtime.h` (the two declarations), `heroes_guard_open.h`,
  `heroes_guard_close.h` (two `push_macro`/`pop_macro` lines each) and
  `selfhost/emit/guarded_names.hero` (two names), which the compiler's tests hold to one another.
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, exit 0, 890,865,870,155 instructions;
  `--version`: `sources 3a68143b1232d0f7`. Kept as `tree/heroes-c`.

**The cases** (`probe-c/`, each built by `heroes-c build`, exit 0; binaries copied to unique names
so their crash reports are this seat's; `pgrep -fl 247-compiler-engineer/probe` after each batch:
nothing):

| case | how it installs | outcome | crash report (`ips.py`, `SHOW=1`) |
|---|---|---|---|
| `storm`, 5 runs (`storm.py ./cecstorm SIGUSR1 4 0.3`) | `signal` | **5 of 5 `exit -6`**, the refusal's line whole on stderr | 5 SIGABRT, `raise < hero_signal_refuse < hero_callback_entered < h_0cb_storm_on_signal < _sigtramp < hero_release_block < hero_str_decref < h_storm_round` (pid 31338): the signal landed inside the allocator and the refusal touched none of it |
| `quiet`, 3 runs | `signal` | **3 of 3 `exit -6`**, words whole | 3 SIGABRT, `... < h_0cb_quiet_on_signal < _sigtramp < h_quiet_round` (protoC's `snprintf` refusal died silent in 1 of 3) |
| `t3x1` (`t3run.py ./cect3x1 SIGINT`) | `signal` | `tick 1`, `tick 2`, **`exit -6` 0.02 s after the signal**, refused | 1 SIGABRT, `... < h_0cb_t3x1_on_stop < _sigtramp < nanosleep < sleep < h_t3x1_main` |
| `wrap` (route A's hole), `timeout -s KILL 30` | the program's own `sigaction` | `0`, then **exit 134, refused by name** | 1 SIGABRT, `... < h_0cb_wrap_on_signal < _sigtramp < pthread_kill < raise < wrap_raise_usr1` |
| `wraponstack` (this seat's: `SA_ONSTACK`) | own `sigaction`, alternate stack | `0`, **refused** | 1 SIGABRT, `... < h_0cb_wraponstack_on_signal < _sigtramp < ...` |
| `infocase` (this seat's: `SA_SIGINFO`, `(sig: i32, info: ptr, context: ptr)`) | own `sigaction` | `0`, **refused** (the second probe's address) | 1 SIGABRT, `... < h_0cb_infocase_on_info < _sigtramp < ...` |
| `fcase` (this seat's: **F's case**, a C handler calling a Heroes `(i64) -> i64` for its result) | own `sigaction` | `0`, **refused: *"fcase.answer was called from inside a signal handler"*** (the walk) | 1 SIGABRT, `... < h_0cb_fcase_answer < fcase_handler < _sigtramp < ...` |

**Every one of the 13 deaths has its report, and every report is the refusal's own `raise`**: no
`SIGKILL` of the locale lock, no `SIGTRAP` of the freelist, nothing silent. The exact test saw every
installer this sitting names (`signal`, the program's own `sigaction`, the alternate stack,
`SA_SIGINFO`) with no list of names: it asks the value in hand (the delivery's own return address),
never a premise about which C function installs handlers.

**The cost** (`/usr/bin/time -l`, instructions retired, three runs each, `probe-c/cost-c.log`):

| program | D's compiler (`heroes-d`) | thunk form, walk 8 | walk 2 | exact test alone (walk 0) |
|---|---|---|---|---|
| `guarded.hero`, `-O0` | 4,970,241,005 to 4,970,799,250 | 4,969,376,578 to 4,971,948,953 | | |
| `guarded.hero`, `-O2` | 2,068,550,981 to 2,069,551,034 | 2,068,615,552 to 2,069,909,144 | | |
| `qsortcost.hero`, `-O0` | 3,018,431,696 to 3,020,167,884 | 22,138,147,775 to 22,144,468,829 (**7.3x**) | | 4,003,734,488 to 4,005,576,133 (**+32.6%**) |
| `qsortcost.hero`, `-O2` | 1,505,100,704 to 1,506,297,710 | 5,554,920,214 to 5,555,711,469 (**3.7x**) | 3,892,091,152 to 3,893,465,282 (2.6x) | 1,925,086,641 to 1,925,906,818 (**+27.9%**) |

- **A Heroes call pays nothing**: `guarded.hero` (a guarded `add` called 50,000,000 times through
  its value) is the same within the runs' spread, at both levels, as the thunk's module says it
  must be (*"A Heroes call by name never reaches it"*): protoC's guard form read 19,976,640,456
  against 4,970,360,692 on the same program.
- **C's calls pay, and the walk is what costs**: `qsortcost.hero` sorts 1,000,000 values with C's
  `qsort` through a C shim calling a Heroes comparator (about 20 million calls by n log2 n, an
  estimate). The exact test costs about 21 instructions a call at `-O2` (an out-of-line call, two
  relaxed loads); the walk at depth 8 about 180, and **most of it is not the depth**: depth 2 still
  costs 2.6x. The first build, which read the thread's bounds inside the loop, read
  28,371,026,013 to 28,372,195,764 at `-O0` (9.4x): every `_Thread_local` read is a call of
  Darwin's accessor, as D's own comment measured. A walk that reads the bounds once from one
  thread-local record, or bounds itself by the thunk's own frame, is unbuilt and unmeasured here.
- **The compiler's own code paths do not move**: the compiler hands C no function
  (`callback_guard.hero`'s header: *"A program that binds no callback guards nobody ... the compiler
  itself"*), so no thunk and no probe is in it; its `check` and warm build were not re-counted for
  this reason, which is a reading of that sentence plus `--emit-c` of the compiler not having been
  diffed: unrun.

## 3. Route F (C's detection with E's delivery), built and run (with D)

**What was built**: the same compiler (`heroes-c`, then `heroes-f` from the same sources, below),
the runtime switched by one line (`#define HERO_SIGNAL_DEFER 1`, `thread.c`): where the exact test
finds the delivery's own return address **and** the thunk is `void (*)(int32_t)` **and** the signal
is not a fault's (`SIGSEGV`, `SIGBUS`, `SIGFPE`, `SIGILL`, `SIGTRAP`, `SIGABRT`, `SIGSYS`), the
thunk writes one 16-byte note (itself, the signal) down a non-blocking pipe and returns, `errno`
kept; a thread the runtime starts through `hero_thread_spawn`'s own door (so it is claimed, has its
stack guard and is joined like any worker, all signals but a fault's blocked) reads the notes and
calls the thunk again, which is then not inside a signal and runs the function. Started at the
first crossing; **joined before the leak check** (`hero_runtime_check_leaks`, one `#if` of three
lines in `alloc.c`), because that check reads its counters *"after every thread has been joined"*
(`alloc.c:80-82`): a deferred handler still running as `main` returns would otherwise be counted
as a leak. Everything else (an indirect call, two pointers, a result, a fault's signal) is refused
as under C. 101 of `thread.c`'s 281 added lines.

**The cases** (`probe-f/`, built by `heroes-c` with that runtime, each exit 0; `pgrep` after each
batch: nothing):

| case | outcome under F | under the base or C |
|---|---|---|
| `storm`, 5 runs | **5 of 5 `exit 0`, `built 20000000 words`**, 1,018,646 to 1,057,740 signals | base: 5 of 5 died (00-shared.md); C: 5 of 5 refused |
| `quiet`, 3 runs | **3 of 3 `exit 0`** | C: 3 of 3 refused |
| `t3x1`, 3 runs (`t3run.py`, SIGINT at 2.5 s) | **3 of 3 `exit 0`**: two *"stopped after 3 ticks"* 0.00 s after the signal, **one *"stopped after 4 ticks"* 1.01 s after it**: the loop's probe ran before the runtime's thread had closed the descriptor | C: refused at the signal |
| `wrap` | `0`, `0`, *"caught 30 through the program's own sigaction"*, exit 0 | the handler's line moved **after** `raise` returned: the coordinator's base run printed `0`, `caught 30 ...`, `0` (00-shared.md) |
| `wraponstack` | the same as `wrap` | |
| `fcase` (a result awaited) | refused, exit 134 (the walk) | |
| `infocase` (two pointers) | refused, exit 134 (not deferrable) | |

Crash reports (`ips.py ... cefstorm cefquiet ceft3x1 cefwrap cefwraponstack ceffcase cefinfocase`):
`ceffcase` 1 SIGABRT and `cefinfocase` 1 SIGABRT, the two refusals; **none for the eight storms and
three `t3x1` runs**.

**What F changes that runs today, measured**: the order of a handler's effect and the code it
interrupted (`wrap`'s line after `raise`'s return; `t3x1` a tick late in 1 run of 3, its effect
racing the loop); the thread a handler runs on (every handler on the runtime's own, so the 2496
case's words, below); a handler awaited for its result or given two pointers, refused; a thread
alive in every program that hands C an `(i32) -> ()` function, which `main`'s end must join, so a
handler that never returns would hold the program's end (by reading the code above: unrun).

## 4. The tracked cases, the 2496 case's words, and what a debugger sees

**Through the harness in my tree** (`./heroes-X run tests/harness/main.hero -- ./heroes-X run
<substring>`, each compiler stage 2 of the tree as it then stood, the provenance hook satisfied by
an empty `git init` in `tree/`, inside my folder, since `.claude/hooks/trees.py:49-57` finds a tree
by its `.git` entry and otherwise judged my archive as the repository's root; D's own case copied
from `932876f95` into `tests/golden/run/`, its header with it):

| case | C (`heroes-c2`, `sources 1f83fdf86f0d2855`) | F (`heroes-f`, `sources 6fe8841f73943f3c`) |
|---|---|---|
| `fixedbugs-2496` | **0 passed, 1 failed**: exit 134, *"...on_signal ran as a signal handler..."* at `-O0` | **1 passed, 0 failed**, its words false (below) |
| `fixedbugs-2257` (three) | **3 passed, 0 failed** | **3 passed, 0 failed** |
| `fixedbugs-2494` (D's own case) | **0 passed, 1 failed**: stops with C's words, not D's | **0 passed, 1 failed**: runs to its end, *"sender started 1, built 5000000 rows, and no signal met the allocator"*, exit 0 |

**The 2496 case's sentence, measured**: `cases/w2496.hero`, the case with its header recording
`pthread_self()` in the handler and the worker asking whether the last handler ran on itself,
printed with D's compiler **`200 rounds, 200 signals handled on the worker they were sent to`** and
with F's **`200 rounds, 0 signals handled on the worker they were sent to`**. So under F (and under
any E) the tracked case passes the harness while its sentence is false: the harness compares bytes
and the bytes come from a count any thread raises. Under C and A it fails. **Every route changes
that case; under a deferring route only a reader of its words can see it.**

**My own prototype's first harness run failed to compile under `--sanitize`**: the stack guard's
alternate stack does not exist there (`stack.c:567-623`, `HERO_STACK_GUARD_YIELDS_TO_ASAN`, *"the
guard's two doors are empty"*), so the walk is blind under the sanitizer and the exact test is all
there is. Guarded with that macro and rebuilt; the rows above are after that repair.

**One `lldb` session by hand** (`lldb --batch` on `probe-c/qsortcost-w0`, the thunk form, a program
that hands C a function, `lldb-session2.txt`): **`process handle SIGURG` reads `PASS true, STOP
false, NOTIFY false`**, and the program ran to `exit 0` without a stop in the first session
(`lldb-session.txt`). With `process handle SIGURG -s true` it stops, `stop reason = signal SIGURG`,
at `__pthread_kill`, under `hero_signal_learn < hero_callback_of < h_qsortcost_main` (breakpoint 2's
`bt 3`). So a user sees nothing unless they asked to stop on SIGURG, and then two stops at the first
crossing whose frames name the probe. **The probe runs once per process, at the first crossing, not
at every start**: a program that hands C no function never raises it. `tests/golden/lldb/` does not
move: `grep -l -E '\(function\(' tests/golden/lldb/*.hero` and `grep -l -E 'h_0cb|hero_callback'
tests/golden/lldb/*` print nothing.

## 5. Route A, built far enough to run `check`

**What was built** (`apply-a.py`, applied to `tree/` and taken off after): 9 lines in
`selfhost/check/ffi.hero` (`diff ffi-base.hero ... | grep -c '^>'`), in `arguments_may_stand`, the
checker's existing walk of an extern's parameters, reusing its existing refusal
`ffi_parameter_position` (code `ffi_parameter_type`) with a new reason: an extern named `signal`,
`sigset`, `bsd_signal`, `ssignal` or `sysv_signal` may not declare a function-typed parameter. Stage
2: exit 0, `sources 12721d126766f4a8` (`tree/heroes-a`). A real landing would owe its own
diagnostic class (`selfhost/ffi_errors.hero`, 380 lines today), its `Fix` (none `certain`: the C
handler is the program's to write), a `tests/golden/check/` case annotated in its source, and the
`check`, `annotations`, `fixes`, `run`, `emission`, `determinism`, `corpus` and `compat` suites, a
change to what the checker refuses (`.claude/rules/verification.md`).

**`heroes-a check`** (`probe-a/check-*.txt`; each exit read straight after its command; `clocale` and
`wraplocale` checked last, after section 6 wrote them: exit 1 and exit 0):

| program | `check` under A | what it does at run time under C |
|---|---|---|
| `storm`, `quiet`, `t3x1`, `cmalloc`, `clocale` | **exit 1**, `error[ffi_parameter_type]` at the `handler` parameter of the `signal` binding | refused at the signal |
| the 2496 case, D's own case, the two Heroes 2257 cases | **exit 1** (the 2257 pair, whose handlers never run, refused too) | 2496 and D's refused at the signal; the 2257 pair pass |
| the 2257 case of the program's own C | exit 0 | passes |
| **`wrap`, `wrapstorm`, `wraplocale`, `fcase`** | **exit 0: A's hole** | refused at the signal (or, `fcase`, by the walk) |
| **`chandler`** (this seat's: a handler **written in C**, bound as an extern and handed to `signal` from Heroes, the shape R4 allows) | **exit 1: a correct program refused** | runs, prints `1`, exit 0 (also under D) |

So A by the installer's name, in the form that sees every Heroes value at the binding, **refuses a
program R4 allows** (`chandler`), and **passes four that run Heroes code in a signal** (`wrap`'s
three and F's case). Written at the call instead (refuse a Heroes function as the argument), it
would keep `chandler` but has to know where a function value came from, which the checker does not
track (`callback_thunk.hero`: *"the emitter does not chase values"*; the table `hero_callback_of`
decides at run time for that reason). A name list is also a premise about each C library's header:
the spec-warden read glibc declaring `ssignal` and `sysv_signal` and no `bsd_signal` under
`-std=gnu11`. One detail of the run: binding `raise` is refused (`error[reserved_word]`), so
`chandler` raises with `kill(getpid(), ...)`.

## 6. The hole A, E by name and H share, and the member no runtime reaches, built and run under D

| program (`probe-d/`, `heroes-d`) | the handler | under D, 5 storms each | crash reports |
|---|---|---|---|
| `wrapstorm` | the storm's, installed through `wrap.h` | **5 of 5 stopped by D's words**, exit -6 | 5 SIGABRT, `hero_alloc_reentered < ... < h_0cb_wrapstorm_on_signal < _sigtramp < hero_malloc_raw` |
| `cmalloc` | calls C's own `malloc(64)` and `free` through externs | **5 of 5 exit 0**, `built 20000000 words`, about a million signals each: no death, which proves nothing (POSIX calls it undefined) | none |
| **`clocale`** | calls C's `localeconv()` through an extern, nothing else; by `signal` | **5 of 5 `exit -9`, nothing on stderr** | **5 SIGKILL, `asi` *"Trying to recursively lock an os_unfair_lock"*, `localeconv_l < h_clocale_on_signal < h_0cb_clocale_on_signal < _sigtramp < localeconv_l < __vfprintf < ... < hero_int_to_str < h_clocale_round`** |
| **`wraplocale`** | the same, installed through `wrap.h` | **5 of 5 `exit -9`, nothing on stderr** | **5 SIGKILL, the same lock** |

The same two under the routes built (3 storms each): **C refused 6 of 6 with its words** (exit -6);
**F ran 6 of 6 to `exit 0`** (the call deferred to the runtime's thread, where the lock is only
contended). So **a handler that makes one C call and builds no Heroes value dies in silence under D,
every time**: D cannot see it (no runtime allocation), a runtime made safe in place (route I) cannot
reach it (the call is C's), and a refusal by the installer's name (A, and E's selection by name)
cannot see `wraplocale`. H would refuse `localeconv` by its list only where it knows the function is
a handler, which is A's question again: `wraplocale` passes it.

## 7. Route S, its polls built and counted

**What was built** (a second tree, `tree-s/`, the base with D, so C's changes are not in it): a
poll at **every function's entry** (`selfhost/emit/body.hero`, one line after the guard's, which is
where `entry`'s line already goes) and **before every unconditional jump**
(`selfhost/emit/term.hero:37`, a superset of the loops' back edges), each `HERO_POLL`, one load of a
**process-wide** `volatile int` and a branch to `hero_signal_run_pending()` (`heroes_runtime.h`, 5
lines; `parts/thread.c`, 4). That is **the cheapest S can be**: the flag a C handler would set, the
call that would run the Heroes function, and the polls inside the runtime's own waiting loops are
not built, nor is a per-thread flag, which S needs to run the handler on the thread it interrupted
and which costs a call of Darwin's thread-local accessor at every poll at `-O0` (D's comment).
Seed, exit 0; X (`heroes build selfhost/main.hero -o heroes-x`, its emitter writes the polls) and Y
(`heroes-x build selfhost/main.hero -o heroes-y`, the compiler with polls in it), both exit 0,
`sources 69dce1a64ebab338`; Y's C holds them (`build/tu-.../library.c`: `{ HERO_POLL; ...` at
entries, `HERO_POLL; goto` 17 times).

**The count** (`probe-s/cost-s.log`, `cost-s-check.log`; three runs each; without polls = `tree-s`'s
stage 1, the seed's emitter, on the same runtime):

| program | without polls | with S's polls | cost |
|---|---|---|---|
| `tight.hero` (this seat's: a 200,000,000-iteration loop, no call), `-O0` | 9,020,175,037 to 9,020,597,371 | 10,620,373,324 to 10,621,034,232 | **+17.7%** |
| `tight.hero`, `-O2` | 1,218,452,901 to 1,219,186,647 | 1,618,424,325 to 1,619,091,080 | **+32.8%** |
| `guarded.hero`, `-O0` | 4,969,185,195 to 4,969,835,901 | 5,769,311,562 to 5,769,860,088 | +16.1% |
| `guarded.hero`, `-O2` | 2,068,571,134 to 2,069,074,252 | 2,268,462,370 to 2,269,046,234 | +9.6% |
| `qsortcost.hero`, `-O0` | 3,018,219,363 to 3,018,805,045 | 3,437,316,570 to 3,438,011,020 | +13.9% |
| `qsortcost.hero`, `-O2` | 1,504,634,655 to 1,505,315,942 | 1,735,095,048 to 1,735,657,230 | +15.3% |
| **the compiler's own `check selfhost/main.hero`** (D's compiler against Y) | 121,289,894,007 to 121,415,711,310 | 127,133,714,784 to 127,154,178,425 | **+4.8%** |

The compiler hands C no function, so an S that polls only in a program that does would cost the
compiler nothing; the last row is what a program of the compiler's shape would pay the day it
binds one callback (an `atexit`, a `qsort`), **five times D's +0.9% on the same check**.

**What S still cannot do, by reading** (unrun): a signal that lands while the thread is in C waits
until C returns (Python's caveat, 00-shared.md). **The 2496 case is that shape**: its worker spins
inside the header's `fresh_wait_heard` (C, up to 10,000,000 `sched_yield`s, *"seconds"*) waiting for
the handler's count; under S the handler would run only at the worker's next poll, after C returned
with the count unchanged, so each round would add 0 and the case would print *"0 signals handled"*
after about 200 times the wait. And a fault's signal cannot wait for a poll (returning from it runs
the faulting instruction again).

## 8. E, G, H and I: built where they can be, priced where they cannot

- **E** is F's delivery with A's selection. **The delivery is built and run (section 3).** What the
  compiler must know to choose the deferring thunk at build is the callee's name at the extern call
  (`callback_thunk.crossings` already holds the callee's declaration, so it is one `if` there and a
  second table beside `hero_callback_of`): **that is A's list, and `wrap`, `wrapstorm` and
  `wraplocale` pass it (section 5)**, so E by name defers the plain cases and leaves exactly the
  programs A leaves, `wraplocale` dying 5 of 5 in silence under D. F asks the delivery's own return
  address instead, which no installer can hide. The runtime part is the one F built: a pipe and a
  thread started by `hero_thread_spawn`'s door, so it is claimed and `hero_thread_guard` passes,
  joined before the leak check. **The runtime's own handlers do not move under any route here**
  (read, not run): they are C and never reach a thunk (stack guard SIGSEGV/SIGBUS, the lease handler,
  the platform layer's stop); `run.c` catches its four only where they stand at their default (`run.c:681-702`:
  `before.sa_handler != SIG_DFL` is skipped), so a program's own handler, a thunk included, stays
  its own and is not forwarded.
- **G**, a door the runtime owns for other signals: not built. By reading, it is the platform
  layer's stop (`runtime/platform/hero_net.c:573-650`, INT and TERM, one byte down a pipe)
  generalised to a counter per signal: runtime C and one header's declarations, **no compiler
  change and no core construct**, so it fits the ceiling (estimate, unbuilt: 100 to 150 lines with
  Windows' console handler, which covers Ctrl-C and Ctrl-Break only). It does not enforce R4 by
  itself (a program can still hand `signal` a Heroes function) and owes Principle 0's ticket as a
  new surface: a complement to C, never a substitute.
- **H**, a refusal by the handler's body: not built. It needs (1) A's selection, so its soundness is
  A's: **`wraplocale` passes it** and dies 5 of 5 in silence under what H leaves, D; (2) a transitive
  analysis of the body over the IR's op kinds (22, the arms `callback_guard.addresses_taken` matches)
  and the call graph, deciding which allocate or free, which in Heroes is most of them; (3) a list
  of admitted C names (POSIX's async-signal-safe list, and each C library's deviations), a premise
  about the world. Estimate, by reading, 250 to 400 lines in `selfhost/check/` plus the list: not a
  core construct, but a special-purpose analysis keyed on a name list, the opposite of §1.7's
  criterion (*"remove a special case from the compiler"*), whose purpose is to admit what R4
  forbids.
- **I**, the handler made safe in place: not built. By reading what it touches: a second allocation
  mode in `alloc.c` (1,560 lines with D) for every allocation and drop a handler makes; an integer
  and a float formatter without `snprintf` (`f64.c`, 11 calls; shortest round-trip float printing is
  a library of its own); `print` by `write(2)`, against `main`'s buffered stdout; the handle and
  lease locks (`parts/lock.c`, 295 lines at the round's head) turned into try-locks with a refusal.
  Hundreds of lines at least, the ceiling's order, for a context R4 rules out; and **`clocale` dies 5
  of 5 in silence whatever the runtime does**, since its handler's one call is C's own.

## 9. Windows (question 9): compiled with zig, never run

`zig 0.15.2`, `-target x86_64-windows-gnu -std=gnu11 -Wall`, caches in `win/cache`: the runtime of
the base with D (the control) compiles, exit 0, **one warning** (`replace.c:89`, `#pragma comment`);
route C's and route F's runtimes compile, exit 0, **the same one warning**; `winsig.hero` (a Heroes
SIGINT handler, emitted by `heroes-c2 --emit-c`, the thunk's line in it) compiles, exit 0, **no
warning**. On Windows the prototype's `hero_signal_learn` and `hero_callback_entered` are stubs that
learn and refuse nothing: no SIGURG, no `_sigtramp` to learn. So on Windows the routes built here
reduce to the base's: Ctrl-C's handler runs on a thread UCRT makes (Microsoft's `signal` page, the
historian's `verified` quote) and `hero_thread_guard` refuses it as a thread the program did not
start (thread.c's comment; unrun on the box), and a handler UCRT calls on the faulting thread (the
historian: the page does not say which thread) is seen by nothing but D. A Windows detection is a
question: whether UCRT's `raise` calls the handler directly from its own C, which would leave no
delivery's return address to learn the same way, is this seat's recollection, unverified today.

## 10. D's place and D's words (questions 6 and 10)

- **D stays, as the backstop.** Under C, D is what stands where the exact test cannot see and the
  walk is blind: a handler of the program's own C calling a Heroes function on a platform whose C
  keeps no frame records (Heroes passes no frame-pointer flag, `grep -n -i 'frame-pointer\|omit-frame'
  selfhost/cli/*.hero` prints nothing; clang omits them on Linux x86-64 at `-O2` by default, a
  reading), under `--sanitize` (the walk sees nothing, section 4), beyond the walk's depth, and on
  Windows. Its price stays its commit's: +0.9% on the compiler's check, +1.1% on its warm build.
  Under F the same. **No route built here makes D moot**: none sees every shape on every platform.
- **D's own case changes under every route but D alone** (section 4): under C it stops with C's
  words; under F it runs to its end. A case that still reaches D under C is one the exact test does
  not see: `fcase`'s shape (a C handler calling a Heroes function) with the walk off or deeper than
  it, its function allocating.
- **D's words**: under C the half *"a Heroes function given to C as a signal handler"* can no longer
  reach D on POSIX (C refuses it first), so the words narrow to the shape that can, *"called from a
  signal handler of C's"*; under F their remedy is false, since a Heroes function given as a handler
  is then deferred and correct. Whoever lands the adopted route amends them in the same commit. Their
  remedy, *"a handler written in C in the program's own header"*, is the shape whose two-module trap
  the spec-warden measured (`swp6`, 2 of 2 ignored in silence); from this seat, a C handler handed to
  `signal` from Heroes as an extern function value (`chandler`, section 5) runs and counts correctly,
  and is the remedy's form with no `static` flag in a header two modules include.

## 11. The questions, from this seat

- **Q3, every refusal async-signal-safe end to end**: built (section 2) and run: **22 refusals under
  C** (`storm` 5, `quiet` 3, `t3x1`, `wrap`, `wraponstack`, `infocase`, `fcase` twice, at walk 8 and
  2, `clocale` 3, `wraplocale` 3, and under `--sanitize` `storm` and `wrap`), **every one with its
  words whole on stderr and its crash report the refusal's own `raise`** (`ips.py ... cecstorm
  cecquiet cect3x1 wrap wraponstack fcase infocase cecclocale cecwraplocale fcase-w2 cecsanstorm
  cecsanwrap`: 22 SIGABRT), none the lock's SIGKILL; `cecclocale`'s report (pid 31926) has the
  signal landing in `__vfprintf < _vsnprintf < __snprintf_chk < hero_int_to_str`, the instant that
  killed in silence under D. protoC's `snprintf` refusal died silent 1 of 3. Exit status:
  `storm.py` read `-6` (SIGABRT); a refusal case's `!panic:` holds where the status is nonzero and the text present (`expectation.hero:178-200`), so
  `_exit(134)` after a `raise` that did not end the thread holds too. A refusal on a thread that is
  ending (where `abort()` gave SIGTRAP) is unrun: the refusal never calls `abort()`.
- **Q4, C's detection and its exceptions**: the alternate stack, measured (`wraponstack`, refused by
  the exact test); a frame with no record between the delivery and the thunk: the exact test needs
  none, the walk does (above); a handler the program's own C installs, measured (four programs); a
  signal landing while the thread is in C, measured (`t3x1`'s report: `_sigtramp < nanosleep <
  sleep`); x86-64, Linux's delivery: **unrun here** (protoC's walk found Linux arm64's delivery in the
  lane's run, `r2494/linux-protoC.log`); the SIGURG probe: `lldb` session (section 4), once per
  process at the first crossing.
- **Q5**: F's order, thread, and refusals measured (section 3); a fault's signal is never deferred
  (F refuses it at delivery); `SA_SIGINFO`'s two pointers live on the delivery's frame, so F refuses
  rather than hand a later call dead pointers (`siginfo_t` could be copied, `ucontext` not: a
  reading); a handler that must run before the program ends runs before the leak check under F (the
  join), but **not before `exit(code:)`**, which ends the process with the runtime's thread in it (a
  reading of `hero_exit`, `os.c:1005-1011`: unrun); S's latency, section 7.
- **Q7, glibc**: unrun from this seat.

## 12. What moves under route C, measured

- **The compiler's own tests** (`heroes-c2 test selfhost/main.hero` in `tree/`): first **2116
  tests, 2 failed**, both the macro guard's (`emit/macro_guard.hero:263`, *"the list holds every word
  the runtime's headers write"*: the prototype's parameter names `again frame ret sig`; and `:422`,
  defect 2333's reserved list: the thunk now writes `__builtin_frame_address` and
  `__builtin_return_address`). Repaired by naming only `what` in the prototype and adding the two
  builtins to `RESERVED_LINES` (`guarded_names.hero`) and to `heroes_reserved_open.h` and
  `heroes_reserved_close.h`, one line each: **2116 tests, all passed** (`selftest-c2.log`). The
  instruments caught the route's two list obligations before any suite would.
- **The `run` cases**: the 2496 case and D's own case change (section 4); the 2257 three do not.
- **`suite_runtime`'s Rule 3** (`tests/harness/suite_runtime.hero:148`, *"every object in the runtime
  that survives between calls is `_Thread_local`, or `_Atomic`, or written down"*): C's two kept
  objects are `_Atomic` (`hero_signal_returns`, `hero_signal_state`); F adds two plain ones
  (`hero_signal_fds`, `hero_signal_worker`) that need `_Atomic` or an entry; S's flag is a plain
  `volatile int`. Read, not run.
- **The emissions**: every program that hands C a function moves (one line per thunk, one in the
  table), so `emission` and `emit` move wherever such a program is pinned, and `wholes`,
  `descriptors` and `determinism` are owed (`.claude/rules/verification.md`, the `selfhost/emit/**`
  row); the compiler's own emission does not (it hands C no function: no thunk is written into it).
  Not run.
- **Under `--sanitize`, measured** (`heroes-c2 build ... --sanitize`, `-fsanitize=address,undefined`):
  `wrap` refused (exit 134), `storm` refused (`exit -6`): ASan on Darwin does not wrap the program's
  handler, the delivery's return address is still `_sigtramp`'s. **But `fcase` ran inside the
  signal undetected: `0`, `0`, `60`, exit 0**, because the walk has no bounds under the sanitizer.
  **A measured hole of route C**: F's case under `--sanitize`, and D does not see it either, its
  function allocating nothing.

## 13. Verdict, route by route

Vetoes below are cast **on soundness**, the veto this seat's brief and CLAUDE.md § 4 give it, each
naming a program built and run that dies in silence under the route vetoed; this seat's own
definition reserves its veto for a core construct or a breach of the ceiling, which reaches I alone.

| route | verdict | the ground, each measured above unless said |
|---|---|---|
| **C in the thunk form** (exact test, bounded walk, refusal async-signal-safe), **with D** | **approve** | §1.12: built; 22 refusals of 22 said why, every report the refusal's `raise`; every installer seen without a list; a Heroes call pays nothing; no core construct |
| **D alone** | **veto** | `clocale` and `wraplocale`: **10 of 10 SIGKILL, nothing on stderr**, the locale lock, under D (section 6) |
| **A alone** | **veto** | `wraplocale` passes A's `check` (exit 0) and dies 5 of 5 in silence under what A leaves, D |
| A beside C, the declaration form built here | **object** | refuses `chandler`, a C handler R4 allows (a correct program refused); the call-site form, which would keep it, is unbuilt |
| **E by name** | **veto** (alone) | its selection is A's list: `wraplocale` is never deferred and dies as under D; its delivery is F's, built |
| **F** (C's detection, E's delivery) | **object**; the built alternative if the author chooses to honour R4 rather than enforce it | changes what runs today (section 3: `wrap`'s line after `raise`, `t3x1` a tick late 1 run of 3, every handler on the runtime's thread, the 2496 case passing while its words are false); §1.3: whether `on_signal` runs now or later, here or on another thread, depends on who calls it at run time, not on the line; a thread in every program handing C an `(i32) -> ()` function; Principle 0's ticket owed for a new meaning |
| **S** | **object** | its cheapest polls cost +4.8% on the compiler's own check (five times D's), +9.6% to +32.8% on loops; the per-thread flag it needs costs more; a signal while in C waits for C (the 2496 case would read 0, by reading) |
| **G** | **approve as a complement** (not built) | runtime and one header, no compiler change, no core construct; it enforces nothing and owes Principle 0's ticket |
| **H** | **veto** | its selection is A's: `wraplocale`; and §1.7 the wrong way: a special-case analysis keyed on a name list, to admit what R4 forbids (the author's call) |
| **I** | **veto** | `clocale`'s handler builds no Heroes value and makes one C call, and dies 5 of 5 in silence: no runtime made safe in place reaches a call that is C's; and the ceiling: a second allocation mode, formatters without `snprintf`, `print` by `write(2)`, the locks reworked, for a context R4 rules out |

**What conservative would have been** (CLAUDE.md § 4): C's exact test alone (`HERO_SIGNAL_WALK 0`),
with D: `qsortcost` +27.9% at `-O2`, nothing else moving, F's case left to D. The robust form keeps
the walk, whose measured price must come down before it lands (prediction 2).

## The verdict, in this seat's structure

- `verdict`: **approve** route C in the thunk form with D kept as the backstop; **veto** D alone, A
  alone, E by name alone, H and I; **object** to F, S, and A's declaration form beside C.
- `section`: design.md §1.12 (lines 620-658: *"must not segfault and must not corrupt memory"*;
  *"check rather than assume ... provenance is a premise about the world ... one predictable
  branch per FFI argument, and it is paid deliberately"*); §1.1 (174-186, the ceiling); §1.7 and Part
  5 (425-440, 3417-3440: no route here adds one of the seven constructs); §1.3 (206-216) against F;
  CLAUDE.md § Precedence, rank 3 over rank 5.
- `implementation_cost`: as built: `selfhost/emit/callback_thunk.hero` 296 to 324 lines (+28: the
  thunk's first line, the probe's call in `hero_callback_of`); `runtime/parts/thread.c` 116 to 405
  (route C's part 200 lines with comments and Windows stubs; F's 88 more, not adopted);
  `runtime/heroes_runtime.h` +5; `heroes_guard_open.h`, `heroes_guard_close.h`,
  `heroes_reserved_open.h`, `heroes_reserved_close.h` +2 each; `selfhost/emit/guarded_names.hero`
  4 names. **No checker change, no IR change, no core construct, no diagnostic class.** D's 141
  lines in `alloc.c` kept. Owed at the landing: the walk's bounds read once (its measured cost), the
  part moved to a `runtime/parts/` file of its own (`thread.c` is 405 lines), the 2496 case and D's
  case rewritten, the emissions of every pinned program that hands C a function blessed.
- `needed_for_self_hosting`: **no**: the compiler hands C no function (`callback_guard.hero`'s
  header), so no thunk is written into it.
- `argument`: The thunk is the only door from C into Heroes code (`callback_thunk.hero`'s own
  falsifiable claim), so the check belongs there: a Heroes call pays nothing (`guarded.hero`
  unmoved) and so does a program handing C no function. Asking the thunk's own return address
  against the delivery's sees every installer without a list: signal, the program's own sigaction,
  the alternate stack, SA_SIGINFO; 22 refusals of 22 said why, every crash report the refusal's own
  raise. A name list (A, E, H) is a premise about the world, and `wraplocale` dies 5 of 5 in silence
  through its hole; under D alone or I, `clocale`'s single C call dies 5 of 5 too. The walk costs
  too much as built.
- `prediction`: **(1)** at the adopted route's landing (M-core-packages), this seat's
  `cases/clocale.hero` and `cases/wraplocale.hero`, built by the landed compiler, end `exit -6` with
  the refusal's line on stderr in **20 of 20** runs each of `storm.py <binary> SIGUSR1 4 0.3` on
  Darwin arm64, and `ips.py` reads **40 SIGABRT reports through the refusal's `raise` and no SIGKILL**;
  falsified by one SIGKILL or one run with an empty stderr. **(2)** `cases/qsortcost.hero` at `-O2`,
  built by the landed compiler with the walk kept, reads **at most 2,032,000,000 instructions** (1.35
  times D's 1,505 M; the exact test alone read 1,925 M); falsified above. **(3)** the landing's `git
  diff --stat` against its parent adds **at most 250 lines under `runtime/` and 40 under
  `selfhost/emit/`**, tests and goldens apart; falsified above.
- `condition`: (a) a POSIX leg on which the exact test misses a direct delivery, a C library or a
  sanitizer that calls the program's handler through a trampoline of its own (the historian's TSan
  interposes so; Heroes' `--sanitize` is ASan and UBSan, which on Darwin did not, section 12): then
  the exact test is not the whole answer and a refusal at build beside it earns its place; (b) the
  author choosing to honour R4 rather than enforce it: then F, built here with its costs measured,
  and a spec sentence for it; (c) a walk measured at or under prediction 2's bound and not blind
  under `--sanitize`: then F's case is closed on POSIX and D's place narrows to Windows and to frames
  with no record.

## Unrun, said once

Linux (x86-64 and arm64) and Windows runs of every route; glibc's answer where Darwin kills
(question 7); the walk where C keeps no frame records (Linux x86-64 at `-O2`, by reading clang's
default); a refusal on a thread that is ending; F's deferred handler against `exit(code:)`; S's
delivery, its per-thread flag and its latency; E's selection by name as code (it is A's list, whose
hole is measured); G, H and I as code; the blind seat's effect of any sentence. No paid run, no API
call, no GitHub write; every process this seat started was stopped (`pgrep -fl
247-compiler-engineer/probe` after each batch: nothing).
