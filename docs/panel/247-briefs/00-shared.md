# Panel 247, the brief every seat reads first

Convened 2026-11-13 by the coordinator, in M-core-packages (`gh issue view 2494 --repo
heroes-lang/heroes --json milestone`: `M-core-packages`). **The question is defect 2494's complete
repair**: how a program that hands C one of its own functions as a signal handler, which spec § 13
allows today, is kept from running Heroes code inside the signal. **Why a sitting**: lane b34-signals
of batch 34 found that *"every complete repair refuses a program that works today, so it needs a
sitting's ruling"* (its report, `.claude/worktrees/b34-scratch/b34-round/report-signals.md:3`, both
its reports in that one file), under its brief's rule *"if a repair ... needs a ruling no rule
reaches, stop on that item, write the sentence, the ruling needed and your recommendation in your
report, and go on"* (`.claude/worktrees/b34-scratch/brief-common.md`, step 4). **The mandate**:
CLAUDE.md § 4, a sitting asked for once per milestone and convened without asking again; this
milestone's sittings run from 234 to 246 (`ls docs/panel | grep -E '^2(3[4-9]|4[0-6])-' | sed
's/-.*//' | sort -u`). **The full panel**, all five seats: route A is a new diagnostic class and
every refusing route changes what a program the spec allows does (`.claude/skills/panel/SKILL.md`,
*"When in doubt take the full panel"*). **The one paid run is the blind seat's, awaiting the
author's yes** (a paid run is the author's every time, `.claude/rules/verification.md` § The batch);
its ceiling, if given, is **5 USD**, and without the yes the synthesis records the seat as unrun.

## What the sitting rules, and what it does not

**Panel 246's R4 is a ratified rule: no Heroes code runs in a handler** (the coordinator's ruling on
the critic's item 5). Its words, each read today:

- `docs/panel/246-*.md:318`, inside R4: *"**No Heroes code runs in a handler**: Windows runs a
  console control handler on a thread it creates for it ..., and a Heroes handler that allocates
  kills its program (the critic's item 12)."*; at 304-308, R4 reads the blind seat's one right t3
  answer, *"a program's own Heroes handler"*, as *"a route that is unsafe without the stop below"*;
  at 449-450, its prior art: *"refusal of Heroes code in a handler taken from Microsoft's
  HandlerRoutine page"*.
- Decision 2497 (`gh issue view 2497 --repo heroes-lang/heroes`: `CLOSED`, `kind:decision,
  area:runtime`), ratified 2026-11-12 as the author's reading: its recommendation's R4 ends *"so
  `main` returns and task 1072's leak check runs; no Heroes code in a handler."*, and its `## Prior
  art` files it **`Taken from Microsoft`**: *"When the signal is received, the system creates a new
  thread in the process to execute the function."*
- The base's runtime already reads it so: `runtime/parts/thread.c:99-100`, *"panel 246's R4 rules
  that no Heroes code runs in a handler"*, and its refusal tells every program *"give C no Heroes
  function as a signal handler"* (thread.c:111-112).

**So this sitting rules HOW R4 is enforced or honoured, never WHETHER.** CLAUDE.md § Precedence is
the frame: *"Ranks 3 and 4 read together: robustness decides THAT a measured class of wrong
programs closes, Principle 0 HOW"*. A route that **refuses** a Heroes handler (A, C) enforces R4; a
route that **defers** the Heroes function out of the signal (E, the safe point S, F) honours it; **a
route that leaves Heroes code running inside the signal** (D alone, H for the bodies it admits, I)
**changes a ratified resolution, and is the author's call**: a seat that recommends one says so in
those words.

## D, the floor every route builds on

**Route D is landed** (the coordinator's decision under the batch's standing default, the author's
*5a*, `.claude/rules/verification.md` § The batch; CLAUDE.md § Precedence: *"a guard that closes a
corruption class lands, with its cost measured and reported rather than argued"*). Its commit is
**`932876f9585817ec888d1ba875c880f2f1a4b3e3`** on branch `b34-signals`, its parent the base
(`git -C .claude/worktrees/b34-round rev-parse 932876f95^`: `ae5e2cedd...`), merged into the round at
**`2a3a37fc3be275bfa03062b59a1185f6161f8ab8`**, both read at 04:30 (`git -C
.claude/worktrees/b34-round log --oneline -1`); the coordinator confirms them before launch. Under
`runtime/` it changes `parts/alloc.c` alone (`git diff --stat ae5e2cedd 932876f95 -- runtime`: 141
lines added). **Defect 2494 stays open for this sitting** (its commit body: *"Defect 2494 STAYS OPEN
for panel 247's route; this is option D, the part that needs no ruling"*).

**D's words, as they landed** (`git -C .claude/worktrees/b34-round show 932876f95:runtime/parts/alloc.c`,
lines 539-545), written by `write(2)` before SIGABRT is raised with its default restored, `_exit`
after:

> panic: a signal handler used this runtime's allocator while the code it interrupted was inside it,
> which would corrupt memory: a Heroes function given to C as a signal handler, or called from one,
> does so whenever it builds or drops a value. To stop on Ctrl-C or SIGTERM, call
> `hero_net_catch_stop` from `hero_net.h`, and the platform layer's waiting calls end with
> `HERO_NET_STOPPED`; answer any other signal with a handler written in C in the program's own
> header, never with a Heroes function

**Those words are an input every seat judges**: their remedy is false under E, S or F (a Heroes
function would then be a correct handler), and *"a handler written in C in the program's own
header"* is the shape whose two-module trap panel 246's critic measured (`docs/panel/246-reports/
critic-briefs.md:156-162`: a `static` flag per C unit, *"drained after 1000 requests"*, the handler
setting one unit's flag and the loop reading another's).

**What D's commit body reports** (`git -C .claude/worktrees/b34-round show -s 932876f95`), the lane's
measurements, not re-run here: each of the runtime's 11 calls to `malloc`, `calloc` and `free`
bracketed by a flag of the thread's own; its case, *"handler and main allocating without
formatting"*, ending with the stop *"in 200 of 200 runs at -O0, 50 of 50 at -O2, sanitized and
under Guard Malloc"*, and in Linux arm64 *"50 of 50 and 20 of 20 sanitized"*; its cost in
instructions, *"the storm's loop with no signal +5.0% at -O0 (105.49 to 110.79 G), +5.4% at -O2; the
compiler's own check of itself +0.9% (120.27 to 121.36 G), its warm build +1.1% (663.38 to 670.75
G)"*; and **what it does not close**, measured: *"Heroes code in a handler that reaches snprintf
(every integer formatted, hero_int_to_str at runtime/parts/f64.c:267-273), stdio or exit dies on
Darwin by libplatform's SIGKILL with no word ... (12 of 200 runs of the storm's strings with D, every
report read that death; Linux 0 of 50); C's own allocator and stdio inside a C call; a handler's
print while main prints; the handle and lease locks"*.

**Every seat builds on D**: after building the base (below), apply D's change to its tree and build
stage 2 again, from `tree/`:

```
git -C .claude/worktrees/b34-round diff ae5e2cedd 932876f95 -- runtime/parts/alloc.c > ../d.patch
patch -p1 < ../d.patch
./heroes build selfhost/main.hero -o heroes-next                 # stage 2 again
./heroes-next --version                                          # second line: sources 560c95206c908459
```

Measured by the coordinator in `.claude/worktrees/p247-scratch/base-d/` at 04:32 (`patch` exit 0):
stage 2 printed `heroes 0.6.0` and `sources 560c95206c908459`, the line D's commit body gives for
its own stage 2. A route is measured against that compiler, the base's alone where the seat says
why.

## The base

**`ae5e2cedddee93e0d53715a5b5ae6d7833c569df`**, its tree **`3e22d53467b606946a70a6f9dfdea6f46ea47612`**
(`git -C .claude/worktrees/b34-round rev-parse ae5e2cedd 'ae5e2cedd^{tree}'`), the round's head when
the briefs were first written, kept as the base for every seat's build (the coordinator's ruling on
the critic's item 22). It holds 25 commits over `main` at `ae969c89e` (`git log --oneline
ae969c89e..ae5e2cedd | wc -l`): panel 246's R3 in spec § 13 (`1c4c6f9ae`), its R4 and R6 in the
platform layer and defect 2499 (lane b34-layer, merged at `f74f5ca54`), defects 2491, 2495 and 2489
(b34-fsl), 2496 and 2492 (b34-signals, merged at `a833b7a17` and `351b6fcf4`), task 2400, and
design.md 4.19's amendment for 2489 (`ae5e2cedd` itself).

**The round has moved since**, read at 04:38 when these briefs were repaired: its head is
**`2a3a37fc3be275bfa03062b59a1185f6161f8ab8`** (`git -C .claude/worktrees/b34-round rev-parse HEAD`),
8 commits over the base by `git log --first-parent` and 16 by `git log`, 144 files (`git diff --stat
ae5e2cedd HEAD`). Under `runtime/` (`git diff --stat ae5e2cedd HEAD -- runtime`) seven files moved,
516 lines added and 16 removed, each named by the commit that moved it (`git log --no-merges
ae5e2cedd..HEAD -- <file>`): `parts/alloc.c` (141 lines), D at `932876f95`; `hero_compiler.h` (29),
`parts/lock.c` (295, new) and `runtime.c` (6, the line that includes `parts/lock.c`), defect 2453's
lock at `a8a20a6c5`; `parts/fs.c` (32) and `parts/dir.c` (5), defect 2500's removal at `90db57d7a`;
and `parts/spawn.c` (24), a comment on what crosses into a thread at `10bb6f378`. A seat whose route
touches one of those files says so and diffs against the round's head too. (An earlier version of
this paragraph read `3c0920a3b` as *"three commits over the base"*: 3 by `--first-parent`, 6 by `git
log`.) A gate may run in
`.claude/worktrees/b34-round`: **read it only through git, never build, write or run inside it.**
Every seat copies the commit and builds its own compiler from its seed in two stages:

```
mkdir -p .claude/worktrees/p247-scratch/247-<seat>/tree .claude/worktrees/p247-scratch/247-<seat>/tmp
git -C .claude/worktrees/b34-round archive ae5e2cedddee93e0d53715a5b5ae6d7833c569df | tar -x -C .claude/worktrees/p247-scratch/247-<seat>/tree
cd .claude/worktrees/p247-scratch/247-<seat>/tree
export TMPDIR=$PWD/../tmp
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes       # the seed
./heroes build selfhost/main.hero -o heroes-next                 # stage 2
./heroes-next --version                                          # second line: sources 7cc70d136b590d46
```

Measured by the coordinator at 03:16 to 03:20 in `.claude/worktrees/p247-scratch/base/tree`, made
that way: `heroes-next --version` printed `heroes 0.6.0` and `sources 7cc70d136b590d46`, and
`heroes-next doctor` named the copy's own `runtime` (`ok runtime`); the durations, taken on a loaded
machine, are not measurements. A seat whose stage 2 prints another `sources` line is not on the base
and says so first. **No duration a seat writes is a measurement while other sessions run** (CLAUDE.md
§ Verification, CL-025): count instructions (`/usr/bin/time -l`, *instructions retired*), never
seconds.

## Where each seat works

Every seat's folder is **`.claude/worktrees/p247-scratch/247-<seat>/`**, ignored by git (`git -C <the
repository's root> check-ignore -v .claude/worktrees/p247-scratch/base`: `.gitignore:34:.claude/worktrees/`),
never the session scratchpad: CLAUDE.md § Hard stops writes no file outside this repository's root,
and panel 246 worked the same way for the same reason (`docs/panel/246-briefs/00-shared.md` § Where
each seat works). The shared tools are in `.claude/worktrees/p247-scratch/tools/`, read-only to the
seats: copy them into your folder.

## The rules every seat obeys

- **Your own folder, and nothing else**: the tree in `tree/`, every probe beside it, `TMPDIR` set to
  its `tmp/` before any build or tool, so nothing lands under `/tmp`. Never use the trunk's `./heroes`,
  a compiler of `.claude/worktrees/p247`, of `p247-scratch/base/` or `base-d/`, of a lane or of another
  seat; never read, build or run inside another seat's folder, inside `.claude/worktrees/b34-round`,
  or inside a lane's worktree. **The lane's prototypes and logs under
  `.claude/worktrees/b34-scratch/b34-signals/` are read-only to you**: copy what you need.
- **Your report**: `docs/panel/247-reports/<seat>.md` in `.claude/worktrees/p247`, written as you go,
  in your definition's output structure. Write nothing else in `.claude/worktrees/p247`.
- **Every number with the command that produced it, every negative sentence run or posed as a
  question**, the words *only*, *never*, *provably* with what was searched and where (`/panel` step 2;
  CL-077). Where this brief is wrong, say so first.
- **No paid run your brief does not name**: no `claude -p`, no API call, no `heroes measure
  --refresh`, no cloud run. One you find worth running goes in your report with its size and cost.
- **Never print a credential**: `gh auth status` only, never `gh auth token`. No GitHub write, no
  issue, no commit, no push, no tag.
- **A program that may not end is bounded with SIGKILL**: built with `heroes build`, the BINARY run
  under `timeout -s KILL 60`, never a plain `timeout`, which sends SIGTERM, and a program that catches
  SIGTERM outlives it (panel 246 graded so, `docs/panel/246-*.md:205-207`); its output to `/dev/null`
  or through `head -c 65536` into a file. After a timeout, `pgrep -f <your folder>` is read and
  anything left is stopped before a verdict (`.claude/rules/verification.md` § A run that may not
  end). Send signals only to processes you started.
- **A storm is run with the shared driver**, `python3 -I storm.py <binary> <SIGNAME> <seconds>
  <arm-delay>` (`p247-scratch/tools/storm.py`, copied): the lane's driver with the signal's name as an
  argument, stdout and stderr read by two threads and kept to 65,536 bytes each as `t3x1/t3run.py`
  does, SIGKILL to the binary's group after a 30 s wait and again at the end whatever happened. **Run
  it unwrapped**: it starts the binary in a session of its own, so an outer `timeout` that ended the
  driver would leave the binary running. **Compare routes by each run's outcome, never by the count of
  signals sent**: several storms beside a round gate change those counts.
- **Every Darwin storm reads its crash reports**: a death on this Mac leaves a report in
  `~/Library/Logs/DiagnosticReports/<binary>-<date>-<time>.ips`, read-only to you, naming the
  exception, the signal, the abort cause (`asi`) and the faulting thread's frames.
  `python3 -I ips.py ~/Library/Logs/DiagnosticReports <binary-name>...` (`p247-scratch/tools/ips.py`,
  copied) tallies them by signal and cause; `SHOW=1` prints one stack per kind. **They are a sample,
  not a count**: the lane's runs of D's case before its reshaping ended `exit 137` in 5 and 12 runs of
  200 (`c2494/runs200-O0.log`, `runs200-O0-v2.log`), 17 runs, which left 4 SIGKILL reports of `withD`.
  A death read only from stderr is not read: three of today's storm deaths said nothing there.
- **One heavy command at a time** (a stage 2 build, a suite); a round gate and two lanes share this
  machine. Whole suites only where your route changes what the checker refuses, one at a time, named
  in your report with why.

## The defect, and what it is

**Defect 2494** (`gh issue view 2494 --repo heroes-lang/heroes`: `kind:defect,class:blocking,
area:runtime`, filed 2026-11-12 from panel 246's critic, item 12, no comments): `storm.hero` binds
`signal` from `signal.h` and hands it `on_signal`, a Heroes function that builds `f"caught {sig} while
main was allocating"`, while `main` builds and drops 100,000 strings `f"word {r} {i}"` 200 times; sent
`SIGUSR1` as fast as `kill` returns, it dies. **The issue's title names the allocator; the deaths name
more** (the coordinator's ruling on the critic's items 1 and 3), each read today with `ips.py`:

- **Before any repair** (`b34-scratch/b34-signals/r2494/before-storm.log`, 5 runs, three `exit -9`,
  two `exit -5`, after 17,676 to 21,720 signals), the five reports of 01:28:57 to 58: **3 `SIGKILL`**
  (pids 94041, 94060, 94066), each with `asi` *"BUG IN CLIENT OF LIBPLATFORM: Trying to recursively
  lock an os_unfair_lock", "Abort Cause 259"* and the stack `_os_unfair_lock_recursive_abort <
  _os_unfair_lock_lock_slow < localeconv_l < __vfprintf < _vsnprintf < __snprintf_chk <
  hero_int_to_str < h_storm_on_signal < h_0cb_storm_on_signal < _sigtramp < localeconv_l < __vfprintf
  < _vsnprintf < __snprintf_chk < hero_int_to_str < h_storm_round < h_storm_main`: **`main` was
  formatting an integer, `hero_int_to_str` calling `snprintf` (`runtime/parts/f64.c:267-273`, one of
  that file's 11 calls of `snprintf(`, `grep -c 'snprintf(' runtime/parts/f64.c`), holding the C library's locale lock; the
  handler formatted `{sig}` and took the lock again; Darwin's libplatform killed the process, nothing
  said on stderr.** And **2 `SIGTRAP`** (pids 94063, 94069): `_xzm_xzone_malloc_freelist_outlined <
  hero_malloc_raw < hero_alloc_block < hero_str_alloc < hero_int_to_str < h_storm_on_signal <
  _sigtramp < _xzm_xzone_malloc_freelist_outlined < ... < h_storm_round`, the allocator's own
  freelist check. The driver did not send those SIGKILLs: `r2494/storm.py` kills only after its
  30 s wait and then prints `NOT ended 30 s after the storm`, which no line of the log does.
- **Every report of these binaries today** (`ips.py ~/Library/Logs/DiagnosticReports storm quiet noD
  withD withD-O2 withD-san case`): `storm` 3 SIGKILL (the lock), 2 SIGTRAP (the freelist), 8 SIGABRT;
  `quiet` 1 SIGKILL (the lock), 2 SIGABRT; `noD` (D's case without D) 10 SIGKILL (the lock), 3 SIGTRAP
  (the freelist); `withD` 4 SIGKILL (the lock), 39 SIGABRT; `withD-O2` 30 SIGABRT; `withD-san` 25
  SIGABRT; `case` 2 SIGTRAP (below). **All 18 SIGKILL reports are the recursive-lock death.**
- **So the class is wider than the allocator: Heroes code inside a signal reaching C that is not safe
  there** (POSIX's async-signal-safe list): `snprintf` through `hero_int_to_str` and every number an
  f-string or `print` formats, stdio through `print`, C's `exit` through `hero_exit`
  (`runtime/parts/os.c:1005-1011`: `hero_lend_audit_given(...)` then `exit((int)code)`), and `malloc`
  and `free` through the allocator. D closes one member, the runtime's allocator; every route is
  judged against the whole class.
- **The refusals themselves reach it.** protoC's refusal formats its words with `snprintf` inside the
  handler: of its three `quiet` runs (`protoC/quiet.log`, two `exit -6`, one `exit -9`), the third's
  report is `SIGKILL`, pid 75776, `... __snprintf_chk < hero_thread_guard < h_quiet_on_signal <
  h_0cb_quiet_on_signal < _sigtramp < localeconv_l < ... < hero_int_to_str < h_quiet_round`: the
  refusal died of the same lock and said nothing (the lane's report read it as *"the third outlived 30 s"*,
  `report-signals.md:59`; the crash report says otherwise). The base's own thread refusal formats with
  `snprintf` (`runtime/parts/thread.c:105-114`) and ends through `hero_panic` (thread.c:115), which
  calls `fflush(stdout)`, `fprintf(stderr, ...)` and `hero_abort()` (`runtime/parts/panic.c:59-61`). And `abort()` inside a
  handler on an exiting thread did not end by SIGABRT: two `case` reports of 02:00 (the lane's 2496
  work, pids 35254 and 35532) end `__abort < __abort < abort < hero_abort < hero_panic <
  hero_thread_guard < ..._on_signal < _sigtramp <` frames of `_pthread_tsd_cleanup` (the second inside
  the allocator there), `SIGTRAP`, `asi` *"abort() called"*.
- **What Linux does where Darwin kills is unrun**: the storm died 5 of 5 there (`r2494/linux-storm.log`,
  exit -6, after 307 to 3,053 signals), but D's case read 0 of 50 and 0 of 20 sanitized off the stop
  in every version (`c2494/linux-2494*.log`), and glibc's `snprintf` takes no such lock (D's own
  comment): whether a handler's `snprintf` inside `main`'s is harmless there or corrupts in silence
  is a question.
- `quiet.hero`, the same program whose handler allocates nothing (`_ = getpid() + sig`): 3 of 3
  exit 0, `built 20000000 words`, after 1,040,160, 1,075,392 and 1,168,273 signals
  (`r2494/before-quiet.log`); in Linux 3 of 3 exit 0 after about 1.47 to 1.49 million.

**What the spec says**: § 13, `spec/heroes-spec.md:403-404` at the base, *"A callback is a
**parameter**, never a result; its parameters follow the same rule and `()` is `void`:
`atexit(f: (function() -> ()))`."* The spec names no signal and no handler (`grep -c -i
'signal\|handler' spec/heroes-spec.md`: 0). So a Heroes function handed to `signal` is a program the
spec allows, and R4 a rule the spec does not state.

## What the runtime does at the base, each from the tree

- **The callback guard**: `runtime/parts/thread.c` (116 lines, `wc -l`), `hero_thread_guard` at 102,
  called at the entry of every function whose address a program that declares a callback takes
  (`selfhost/emit/callback_guard.hero`, 207 lines; its `entry` writes `hero_thread_guard("<module>.<fn>");`).
  It asks whether the thread is the program's own (`hero_thread_is_home`, a `_Thread_local` at 74) and
  never whether it runs inside a signal handler; its refusal names the two doors since defect 2492
  (thread.c:102-116), formatted with `snprintf` and ended through `hero_panic` (above).
- **The thunk**: `selfhost/emit/callback_thunk.hero` (296 lines): C is handed a thunk's address, a
  second C function that clears the arguments' marks, calls, and checks the result, *"A Heroes call
  by name never reaches it"* (its lines 16-24). A check that only C's calls should pay goes there.
- **The runtime's own handlers**, which no route may break (`grep -n -E 'sigaction\(|signal\(SIG'
  runtime/parts/*.c runtime/platform/*.c`): the stack guard's SIGSEGV and SIGBUS
  (`runtime/parts/stack.c:1054-1055`, `SA_ONSTACK` at 1052 on the alternate stack `sigaltstack`
  installs at 1009, the thread's bounds `hero_stack_lo` and `hero_stack_hi` at 335-336); the lease
  handler's SIGTRAP, SIGABRT and SIGILL (`runtime/parts/os.c:471`, `SA_ONSTACK` at 488; Windows'
  `signal(SIGABRT, ...)` at 260 and 320); `runtime/parts/panic.c:54`, SIGABRT back to its default
  before `abort()` (defect 2257); `runtime/parts/run.c`, SIGTERM, SIGINT, SIGHUP and SIGQUIT caught
  while a child is waited on (the comment at 518, the installs at 681 to 702); and the platform
  layer's stop, SIGINT and SIGTERM, armed by `hero_net_catch_stop` (`runtime/hero_net.h:130-162`, the
  installs in `runtime/platform/hero_net.c` at 573 to 650): its handler is the layer's own C, which
  writes one byte down a pipe (`hero_net.h:145-147`).
- **Windows**: thread.c's comment (lines 82-101 at the base) says UCRT runs a SIGINT handler on a
  thread of its own, so `hero_thread_guard` refuses a Heroes handler there for every Ctrl-C, *"read
  by panel 246's ffi-pragmatist, not run on the box"*. Still unrun: the Windows box is offline
  (`tailscale status` at 03:23 read `apponfly-vps` *offline, last seen 2h ago*), so every Windows fact
  here is a compile or a reading, never a run, and its run is the CI's Windows leg after a push.

**The programs it touches.** In the tree (`grep -rl -E 'function signal\(' tests/golden examples
--include='*.hero'`, each read, and the critic's wider search of every tracked `.hero` and header
finding no other, `docs/panel/247-reports/critic-briefs.md` item 5):

- `tests/golden/run/fixedbugs-2496-a-signal-sent-as-a-thread-starts-waits-until-the-thread-is-the-programs-own.hero`:
  its handler runs on a `hero_thread_spawn` worker the signal is sent to; its `.expected` reads
  *"200 rounds, 200 signals handled on the worker they were sent to"*; `./heroes-next run
  tests/harness/main.hero -- ./heroes-next run fixedbugs-2496` with the base's stage 2: `run (only
  fixedbugs-2496): 1 passed, 0 failed`. **Every refusing and every deferring route changes what that
  sentence claims**, not only whether the case runs.
- The substring `fixedbugs-2257` selects three cases (`... run fixedbugs-2257`: `run (only
  fixedbugs-2257): 3 passed, 0 failed`): two hand `signal` a Heroes function for SIGABRT that never
  runs after a stop, one of which calls `exit(code: 0)` (`hero_exit`, C's `exit`, not safe in a
  handler were it to run), the other returning; the third,
  `fixedbugs-2257-a-sigabrt-handler-of-the-programs-own-c-never-runs-after-a-stop`, installs a C
  handler from its own header.
- Outside the tree: the lane's `storm.hero` and `quiet.hero` (`r2494/`), `t3x1/t3x1.hero` (the one
  `right` answer panel 246's blind seat wrote for a clean stop, whose Heroes handler calls `close` on
  a descriptor the loop then probes with `dup`), and route A's hole below.

## The routes

Every lane path below is under `.claude/worktrees/b34-scratch/b34-signals/`, every count read from
the log named. The prototypes are copies of the lane's runtime at `62ce17563` (defect 2496's commit)
with one change each (`diff -rq` against `git archive 62ce17563 runtime`). **Only A, C and D were
built, by the lane; the coordinator built none.**

- **A, a refusal at build, by the callee's name** (`signal`, `sigset`, `bsd_signal`): enforces R4
  where it sees; a new diagnostic. **Not built.** The lane's report says clang reads the handler
  parameter as `void (*)(int)` on both platforms, so only the name marks it (`report-signals.md:45`), and a function-typed field of a group record is refused `error[ffi_field_type]`
  (`sigaction/sa.hero`, `sigaction/check.txt`). **Its hole, run**: a program's own C wrapper taking
  the handler as a PARAMETER and calling `sigaction` (`p247-scratch/coord/wrap/wrap.h` and
  `wrap.hero`, the critic's, copied): built by the base's stage 2 at exit 0 and run under `timeout -s
  KILL 30`, it printed `0`, `caught 30 through the program's own sigaction`, `0`, exit 0, nothing on
  stderr: a Heroes function formatting `{sig}` ran inside the signal and no name A knows appears.
- **C, a refusal at run time when the system's signal delivery calls a Heroes function**: enforces
  R4. `protoC/runtime/parts/thread.c`, 56 lines over `62ce17563`'s (`diff | grep -c '^[<>]'`): at
  start, a SIGURG raised to two probes learns the return address the delivery leaves for a plain and an
  `SA_SIGINFO` handler (`hero_signal_learn`, called from `protoC/runtime/parts/os.c`, one line);
  `hero_thread_guard`, first of all, walks at most six frame records within the thread's stack bounds
  and, finding that address, formats a refusal with `snprintf`, writes it with `write(2)`, restores
  SIGABRT's default and calls `abort()` (`protoC/runtime/parts/thread.c:135-143`).
  - `protoC/storm.log`: 5 of 5 exit -6, *"panic: storm.on_signal ran as a signal handler: the system
    called it inside a signal ..."*. `protoC/quiet.log`: 2 refused (exit -6) and 1 dead of the lock in
    its own refusal (above). `r2494/linux-protoC.log`: storm 3 of 3 and quiet 3 of 3 refused (exit
    -6), and `t3x1` refused after `tick 2` (`grep -c 'ran as a signal handler'`: 7).
  - Cost, as prototyped in the guard every call passes: `cost/guarded.hero` (a guarded `add` called
    50,000,000 times through its value) read 4,970,360,692 instructions on the lane's runtime and
    19,976,640,456 on protoC's (`cost/gtime-lane.txt`, `cost/gtime-protoC.txt`), the same output
    `1249999975000000`. **Its production form, in the thunk (only C's calls pay), is unbuilt and
    unmeasured**, and its refusal must reach no `snprintf`, stdio or `abort()` inside the handler.
- **D, the allocator's re-entrance guard**: landed, the floor (above). It leaves the handler's Heroes
  code running inside the signal and stops it only where it re-enters the runtime's allocator.
- **E, the Heroes function run later on a thread the runtime owns, never inside the signal**:
  honours R4. Go's `os/signal` (a channel) is the lane's precedent for it (`report-signals.md:65`
  names *"Python's and Go's approach"*; Python's is S, below). **Not built.** If E chooses the
  deferring thunk at build by the callee's name, it inherits A's hole (`wrap` above).
- **S, the Heroes function run later at a safe point of the program's own thread, the home thread or
  the one interrupted, never inside the signal**: honours R4; the route nobody listed until the
  critic (its item 7). Python's shape, by the page panel 246's historian marked `verified`
  (`docs/panel/246-reports/historian.md:342-350`): *"Python signal handlers are always executed in the
  main Python thread of the main interpreter"*, *"the low-level signal handler sets a flag which tells
  the virtual machine to execute the corresponding Python signal handler at a later point"*, with its
  caveat *"A long-running calculation implemented purely in C ... may run uninterrupted for an
  arbitrary amount of time"*; Ruby's `Signal.trap` and OCaml's polling points are candidates the
  historian checks. For Heroes: the runtime's C handler records the signal, and compiled code asks at
  its safe points (loop heads, function entries, the runtime's return from the allocator) and runs the
  Heroes function there. **Not built.** Its questions: where the poll stands, its instructions on
  `cost/guarded.hero` and on a loop, the latency while a thread is blocked in C, and what it makes of
  the 2496 case (on the interrupted thread it keeps *"handled on the worker they were sent to"*).
- **F, C's detection with E's or S's delivery**: honours R4; a route, not a restatement (the critic's
  item 8): it chooses at run time, so it sees `wrap`'s handler where E by name does not, and it
  inherits C's holes (the alternate stack, frames with no record, Windows, where protoC's probe is
  `#if !defined(_WIN32)`) and C's cost on every call C makes. **The case only F meets**: a Heroes
  function called from a handler the program's own C installed, which expects the call done, or a
  result, when it returns; F's deferral must refuse there, and how it knows is a question. What a
  deferred call does with `SA_SIGINFO`'s two pointers, which live on the interrupted stack, is
  another.

**Three more routes nobody built, listed by the critic (its item 9) for the seats to judge**, each to
build or price, or to say why not; the coordinator built none:

- **G, a door the runtime owns for signals other than SIGINT and SIGTERM**: R4's own mechanism, a C
  handler writing one byte down a pipe, offered for any signal and counted, read by the program
  between its steps. Honours R4. R4's veto refuses a *stop* door (`docs/panel/246-*.md:351-355`: *"no
  stop sentence in the specification and no stop door in `hero_os.h` or among the built-ins"*); a door
  for other signals is unruled and owes Principle 0's ticket. It is what counting SIGUSR1 needs (the
  blind seat's t2) and what D's words now send to a C handler in the program's own header.
- **H, a refusal by the handler's body, not the callee's name**: a function given as a handler is
  refused only where its body allocates, formats, prints, or calls C outside an async-signal-safe
  list; `quiet` (`getpid`), `t3x1` (`close`) and the 2496 case (its own C) stay legal. It keeps A's
  question of how the checker knows a function becomes a handler, and **for the bodies it admits it
  leaves Heroes code inside the signal: it changes R4, the author's call.**
- **I, the handler "made safe" in place**: a handler-time arena for allocation, an integer formatter
  without `snprintf`, `print` by `write(2)` inside a handler. **It leaves Heroes code inside the
  signal: it changes R4, the author's call.** The deaths above are in C's locale lock as well as the
  allocator, so it is wide.

The lane's recommendation, its report's words: *"land D in this round, and take C, in its thunk form,
to a sitting"* (`report-signals.md:189`); the coordinator's question is wider: which route, or which
combination, enforces or honours R4 most robustly and completely.

## The questions

1. **What 2494 is**: the class above, Heroes code inside a signal reaching C that is not safe there.
   Judge each route against the whole class, on POSIX and on Windows, for every handler a program can
   install: by `signal`, by `sigset`, through its own C (`wrap`), from a thread C made.
2. **How R4 is enforced or honoured**: refusal (A, C), deferral (E, S, F), a door (G), or a
   combination, and what each refuses or changes that works today: the three tracked programs (the
   2496 case's *"handled on the worker they were sent to"* above all), `quiet`, `t3x1`, `wrap`. A
   route that leaves Heroes code inside the signal (D alone, H, I) is named as changing R4.
3. **Every refusal async-signal-safe end to end**: protoC's `snprintf` died of the lock it refused
   (1 of 3 on `quiet`); the base's 2492 refusal formats with `snprintf` and ends through `hero_panic`
   (`fflush`, `fprintf`, `abort()`); `abort()` in a handler on an exiting thread ended by Darwin's
   `__abort` trap, SIGTRAP. What exit status the harness then reads, and whether a case's `.expected`
   holds.
4. **C's detection, and its exceptions**: the alternate stack (`sigaltstack` with `SA_ONSTACK`), frames
   with no frame record, a handler the program's own C installs, a signal that lands while the thread
   is in C, x86-64 against arm64, Linux's and Darwin's delivery, and the SIGURG probe at every start
   (what a debugger sees).
5. **E, S and F**: what a deferred handler means for a program: the order of a handler's effect and
   the interrupted code, a fault signal that cannot be deferred (SIGSEGV, SIGBUS, SIGFPE, SIGILL,
   SIGABRT), a handler that must run before the program ends, `SA_SIGINFO`'s two pointers, S's poll
   and its latency, and how a deferred call meets `hero_thread_guard`, the platform layer's stop and
   `run.c`'s forwarding.
6. **D's words**: whether they stay true under the adopted route, who amends them if not, and whether
   their remedy (a C handler in the program's own header) is one the language should give, beside
   panel 246's two-module trap.
7. **What glibc does where Darwin kills**: Linux read 0 of 50 off the stop for D's case; harmless, or
   silent corruption?
8. **The spec**: whether § 13 owes a sentence for the route adopted, priced (`spec-warden.md`), and
   what a reader of the spec writes with it and without it (`llm-ergonomist.md`).
9. **Windows**: what each route does where UCRT runs SIGINT on a thread of its own and SIGABRT,
   SIGFPE, SIGILL and SIGSEGV on the faulting thread (Microsoft's `signal` page, to read), compiled
   with `zig cc -target x86_64-windows-gnu`, the caches inside your folder, never run.
10. **D's place**: whether the adopted route keeps D, makes it moot (and its cost, +0.9% on the
    compiler's check and +1.1% on its warm build by D's commit, removable), or needs it as the
    backstop for what it cannot see.

## What the coordinator measured (each with its command)

- **The spec at the base**: 492 lines and 28,770 bytes (`wc -l -c spec/heroes-spec.md`), sha256
  `41ad6690571bd27b...` (`shasum -a 256`, equal to `git show ae5e2cedd:spec/heroes-spec.md |
  shasum -a 256`); `heroes-next measure spec/heroes-spec.md` from the base's root: claude-legacy 8340,
  cl100k_base 8490, maximum 8490, **real 11108** (*"claude-opus-5, 2026-11-13 — the binding number"*,
  no `STALE`), headroom 5276 against 16384, 11168 judged with the FFI floor's 60.
- **The candidate sentences**, each appended with one leading space after the one occurrence of
  `` `atexit(f: (function() -> ()))`. `` (spec line 404), made by
  `.claude/worktrees/p247-scratch/price/drafts.py` and priced by the base's `heroes-next measure`
  (vendored maximum, the delta a lower bound): `spec-warden.md` holds the table, six drafts.
- **Route A's hole** (`p247-scratch/coord/wrap/`), **the tracked cases** (`p247-scratch/base/run-2496.log`,
  `run-2257.log`), **the crash reports' tally** (`p247-scratch/tools/ips.py`), and **the blind seat's
  reference t2 answer** (`p247-scratch/coord/t2ref/`, `llm-ergonomist.md`), each above or in its brief.
- **Line counts** (`wc -l`): `runtime/parts/thread.c` 116, `alloc.c` 1419, `spawn.c` 630, `os.c` 1019,
  `panic.c` 148, `stack.c` 1213, `run.c` 1962; `runtime/hero_os.h` 494, `heroes_runtime.h` 986,
  `hero_net.h` 176; `runtime/platform/hero_net.c` 1233; `selfhost/emit/callback_guard.hero` 207,
  `callback_thunk.hero` 296, `body.hero` 397; `selfhost/check/ffi.hero` 412; `selfhost/ffi_errors.hero`
  380.

## Questions no one has run, posed as questions

- On which thread Windows' UCRT runs a SIGINT handler a program installs with `signal`, and whether
  `hero_thread_guard` then refuses a Heroes function there (thread.c's comment says so from a reading).
- Whether C's six-frame walk finds the delivery's return address through a C library frame built
  without a frame pointer, on Darwin arm64 and on Linux arm64 and x86-64.
- Whether a deferred handler (E, S, F) can keep the meaning of `SA_SIGINFO`'s two pointers.
- What glibc does with a handler's `snprintf` inside `main`'s (question 7).
- Whether `claude -p` can pass its `--max-budget-usd` cap by one request (the blind seat's bound
  rests on it only past 1.00 USD a request; `llm-ergonomist.md`).
