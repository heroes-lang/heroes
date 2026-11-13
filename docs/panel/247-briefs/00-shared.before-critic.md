# Panel 247, the brief every seat reads first

Convened 2026-11-13 by the coordinator, in M-core-packages (`gh issue view 2494 --repo
heroes-lang/heroes --json milestone`: `M-core-packages`). **The question is defect 2494's complete
repair**: what happens when a program hands C one of its own functions as a signal handler, which
spec § 13 allows today. **Why a sitting**: lane b34-signals of batch 34 measured four routes and
found that every complete one refuses a program that works today, which is a ruling no rule reaches
(its brief, `.claude/worktrees/b34-scratch/brief-common.md` step 4, and the coordinator's line in its
prompt: *"A NEW diagnostic code or a refusal of a program correct today is a sitting's ruling"*).
**The mandate**: CLAUDE.md § 4, a sitting asked for once per milestone and convened without asking
again; this milestone's sittings run from 234 to 246 (`ls docs/panel | grep -E '^2(3[4-9]|4[0-6])-'
| sed 's/-.*//' | sort -u`). **The full panel**, all five seats: route A is a new diagnostic class and
every refusing route changes what a program the spec allows does (`.claude/skills/panel/SKILL.md`,
*"When in doubt take the full panel"*). **The one paid run is the blind seat's, awaiting the
author's yes** (a paid run is the author's every time, `.claude/rules/verification.md` § The batch);
its ceiling, if given, is **5 USD**, and without the yes the synthesis records the seat as unrun.

**What is already decided, and by whom**: the coordinator, under the batch's standing default (the
author's *5a*, `.claude/rules/verification.md` § The batch), decided on 2026-11-13 that route D below
lands in batch 34 as a guard (CLAUDE.md § Precedence: *"a guard that closes a corruption class lands,
with its cost measured and reported rather than argued"*), and that 2494 stays open, `blocking`, for
this sitting. D is therefore an input to every route, not a rival: each seat says whether its route
keeps D, makes it moot, or needs it as a backstop.

## The base

**`ae5e2cedddee93e0d53715a5b5ae6d7833c569df`**, its tree **`3e22d53467b606946a70a6f9dfdea6f46ea47612`**
(`git -C .claude/worktrees/b34-round rev-parse ae5e2cedd 'ae5e2cedd^{tree}'`), the head of batch 34's
round on branch `b34-round` when this brief was written. It holds 25 commits over `main` at
`ae969c89e` (`git log --oneline ae969c89e..ae5e2cedd | wc -l`): panel 246's R3 in spec § 13
(`1c4c6f9ae`), its R4 and R6 in the platform layer and defect 2499 (lane b34-layer, merged at
`f74f5ca54`), defects 2491, 2495 and 2489 (b34-fsl), 2496 and 2492 (b34-signals, merged at `a833b7a17`
and `351b6fcf4`), task 2400, and design.md 4.19's amendment for 2489 (`ae5e2cedd` itself). **Not
in it**: route D's commit, which lane b34-signals is making as this is written; once made it stands
on branch `b34-signals` (`git -C .claude/worktrees/b34-round log --oneline b34-signals -3`), and until
then D is its prototype, `protoD` below.

**The round has moved since**: at 03:47 its head was `3c0920a3b` (`git -C
.claude/worktrees/b34-round log --oneline -4`), three commits over the base, 78 files (`git diff
--stat ae5e2cedd 3c0920a3b`), of which the runtime's are `hero_compiler.h`, `parts/lock.c` (new,
defect 2453's lock), `parts/spawn.c` (a comment on what crosses into a thread) and `runtime.c` (its
parts list); none is a file this brief names a route changing, and the base stays `ae5e2cedd` so every
seat measures one tree. A seat that adopts a route in `spawn.c` says so and diffs against the round's
head too. A gate may run in `.claude/worktrees/b34-round`: **read it only through git, never build,
write or run inside it.** Every seat copies the commit and builds its own compiler from its seed in
two stages:

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
that way, under `/usr/bin/time -p` on a machine three lanes were loading (`uptime` at the start:
33.05, 11.57, 7.35): the seed `real 15.88` (`user 15.16`), stage 2 `real 213.51` (`user 155.30`,
`sys 36.74`, a waiting run, not a duration), `heroes-next --version` printing `heroes 0.6.0` and
`sources 7cc70d136b590d46`, and `heroes-next doctor` naming the copy's own `runtime` (`ok runtime`).
A seat whose stage 2 prints another `sources` line is not on the base and says so first. **No
duration a seat writes is a measurement while other sessions run** (CLAUDE.md § Verification,
CL-025): count instructions (`/usr/bin/time -l`, *instructions retired*), never seconds.

## Where each seat works

Every seat's folder is **`.claude/worktrees/p247-scratch/247-<seat>/`**, ignored by git
(`git -C <the repository's root> check-ignore -v .claude/worktrees/p247-scratch/base`:
`.gitignore:34:.claude/worktrees/`), never the session scratchpad: CLAUDE.md § Hard stops writes no
file outside this repository's root, and panel 246 worked the same way for the same reason
(`docs/panel/246-briefs/00-shared.md` § Where each seat works).

## The rules every seat obeys

- **Your own folder, and nothing else**: the tree in `tree/`, every probe beside it, `TMPDIR` set to
  its `tmp/` before any build or tool, so nothing lands under `/tmp`. Never use the trunk's `./heroes`,
  a compiler of `.claude/worktrees/p247`, of `p247-scratch/base/`, of a lane or of another seat; never
  read, build or run inside another seat's folder, inside `.claude/worktrees/b34-round`, or inside a
  lane's worktree. **The lane's prototypes and logs under `.claude/worktrees/b34-scratch/b34-signals/`
  are read-only to you**: copy what you need into your folder.
- **Your report**: `docs/panel/247-reports/<seat>.md` in `.claude/worktrees/p247`, written as you go,
  in your definition's output structure. Write nothing else in `.claude/worktrees/p247`.
- **Every number with the command that produced it, every negative sentence run or posed as a
  question**, the words *only*, *never*, *provably* with what was searched and where (`/panel` step 2;
  CL-077). Where this brief is wrong, say so first.
- **No paid run your brief does not name**: no `claude -p`, no API call, no `heroes measure
  --refresh`, no cloud run. One you find worth running goes in your report with its size and cost.
- **Never print a credential**: `gh auth status` only, never `gh auth token`. No GitHub write, no
  issue, no commit, no push, no tag.
- **A program that may not end, and a signal storm, are bounded**: built with `heroes build`, the
  BINARY run under `timeout 60`, its output to `/dev/null` or through `head -c 65536` into a file;
  a storm driver (the lane's `storm.py`, below) caps its own time. After a timeout, `pgrep -f <your
  folder>` is read and anything left is stopped before a verdict (`.claude/rules/verification.md` § A
  run that may not end). Send signals only to processes you started.
- **One heavy command at a time** (a stage 2 build, a suite); a round gate and two lanes share this
  machine. Whole suites only where your route changes what the checker refuses, one at a time, named
  in your report with why.

## The defect, as filed and as re-run

**Defect 2494** (`gh issue view 2494 --repo heroes-lang/heroes`: `kind:defect,class:blocking,
area:runtime`, filed 2026-11-12 from panel 246's critic, item 12): `storm.hero` binds `signal` from
`signal.h` and hands it `on_signal`, a Heroes function that builds an `f"..."` string, while `main`
builds and drops 100,000 strings 200 times; sent `SIGUSR1` as fast as `kill` returns, it dies. Lane
b34-signals re-ran it at its own base, with the critic's driver (its logs, read by the coordinator
today):

- `b34-scratch/b34-signals/r2494/before-storm.log`: 5 runs, every one dead: exit -9 three times
  (SIGKILL; the driver prints `NOT ended 30 s after the storm` when the SIGKILL is its own, and these
  lines do not, so whose it was is a question) and -5 twice, those two printing *"panic: the process
  is dying of SIGTRAP, under storm.on_signal, and this runtime did not raise it ... memory given back
  twice among them"*, after 17,676 to 21,720 signals.
- `r2494/before-quiet.log`: `quiet.hero`, the same program whose handler allocates nothing
  (`_ = getpid() + sig`), 3 runs, exit 0, `built 20000000 words`, after 1,040,160, 1,075,392 and
  1,168,273 signals.
- `r2494/linux-storm.log` (Linux arm64, the lane's `linux-storm.sh` in `heroes-linux-arm64:latest`):
  the storm 5 of 5 at exit -6 after 307 to 3,053 signals; `quiet` 3 of 3 at exit 0 after about 1.47
  to 1.49 million each.

**What the spec says**: § 13, `spec/heroes-spec.md:403-404` at the base, *"A callback is a
**parameter**, never a result; its parameters follow the same rule and `()` is `void`:
`atexit(f: (function() -> ()))`."* The spec names no signal and no handler (`grep -c -i
'signal\|handler' spec/heroes-spec.md`: 0). So a Heroes function handed to `signal` is a program the
spec allows.

**What panel 246 ruled, and what it did not**: R4, ratified 2026-11-12
(`docs/panel/246-*.md:301-326`), puts a server's stop in the platform layer, whose handler is C, and
says at line 318 *"**No Heroes code runs in a handler**: Windows runs a console control handler on a
thread it creates for it ... and a Heroes handler that allocates kills its program (the critic's item
12)"*. It describes the layer's mechanism; whether a program's OWN Heroes handler is refused, made
safe, or left is not ruled there as far as the coordinator read: `grep -n -i 'own handler\|program.s
own' docs/panel/246-*.md` prints lines 102, 152 and 305, and 305 is R4's own reading that t3-x-1's
Heroes handler is the shape that killed its program, not a ruling on such a handler; read the file
and say if you find one.

## What the runtime does at the base, each from the tree

- **The callback guard**: `runtime/parts/thread.c` (116 lines, `wc -l`), `hero_thread_guard` at 102,
  called at the entry of every function whose address a program that declares a callback takes
  (`selfhost/emit/callback_guard.hero`, 207 lines; its `entry` writes `hero_thread_guard("<module>.<fn>");`).
  It asks whether the thread is the program's own (`hero_thread_is_home`, a `_Thread_local` at 74) and
  never whether it runs inside a signal handler; its refusal names the two doors since defect 2492
  (thread.c:102-116).
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
  by panel 246's ffi-pragmatist, not run on the box"*. Still unrun: the Windows box is offline (`tailscale
  status` at 03:23 read `apponfly-vps` *offline, last seen 2h ago*), so every Windows fact here is a
  compile or a reading, never a run, and its run is the CI's Windows leg after a push.

**The programs it touches.** In the tree, three `.hero` files hand C a Heroes function as a handler
(`grep -rl -E 'function signal\(' tests/golden examples --include='*.hero'`, each read):
`tests/golden/run/fixedbugs-2496-a-signal-sent-as-a-thread-starts-waits-until-the-thread-is-the-programs-own.hero`
(its handler runs), and the two `fixedbugs-2257-*` cases that bind `signal` (their handler never
runs after a stop). Outside the tree: the lane's `storm.hero` and `quiet.hero` (`r2494/`), and
`t3x1/t3x1.hero`, the one `right` answer panel 246's blind seat wrote for a clean stop, whose Heroes
handler calls `close` on a descriptor the loop then probes with `dup`.

## The routes, as lane b34-signals built and measured them

Every path below is under `.claude/worktrees/b34-scratch/b34-signals/`; every count is read from the
log named, by the coordinator, today. The prototypes are copies of the lane's runtime at `62ce17563`
(defect 2496's commit) with one change each (`diff -rq` against `git archive 62ce17563 runtime`):

- **A, a refusal at build, by the callee's name** (`signal`, `sigset`, `bsd_signal`): a new
  diagnostic. **Not built.** The lane's report says clang reads the handler parameter as
  `void (*)(int)` on both platforms, so the type marks nothing and only the name does, and that
  `sigaction` is reachable from Heroes only through the program's own C: `sigaction/sa.hero`, a
  function-typed field of a group record, is refused `error[ffi_field_type]` (`sigaction/check.txt`,
  run by the lane); a program's own C wrapper that takes the handler as a parameter
  (`sigaction/glue.h`'s shape with a parameter instead of a field) would reach it, a question.
- **C, a refusal at run time when the system's signal delivery calls a Heroes function**:
  `protoC/runtime/parts/thread.c`, 56 lines over `62ce17563`'s (`diff | grep -c '^[<>]'`): at start,
  a SIGURG raised to two probes learns the return address the delivery leaves for a plain and an
  `SA_SIGINFO` handler (`hero_signal_learn`, called from `protoC/runtime/parts/os.c`, one line);
  `hero_thread_guard`, first of all, walks at most six frame records within the thread's stack bounds
  and, finding that address, formats a refusal with `snprintf`, writes it with `write(2)`, restores
  SIGABRT's default and calls `abort()` (`protoC/runtime/parts/thread.c:135-143`).
  - `protoC/storm.log`: 5 of 5 exit -6, *"panic: storm.on_signal ran as a signal handler: the system
    called it inside a signal ..."*. `protoC/quiet.log`: 2 of 3 refused (exit -6), the third `exit -9`
    (the lane's report reads it as outliving 30 s; the log line says only `exit -9`).
    `r2494/linux-protoC.log`: storm 3 of 3 and quiet 3 of 3 refused (exit -6), and `t3x1` refused
    after `tick 2` (`grep -c 'ran as a signal handler'`: 7).
  - Cost, as prototyped in the guard every call passes: `cost/guarded.hero` (a guarded `add` called
    50,000,000 times through its value) read 4,970,360,692 instructions on the lane's runtime and
    19,976,640,456 on protoC's (`cost/gtime-lane.txt`, `cost/gtime-protoC.txt`), the same output
    `1249999975000000`. **Its production form, in the thunk (only C's calls pay), is unbuilt and
    unmeasured.**
- **D, a re-entrance watch on the runtime's allocator** (being landed now, above):
  `protoD/runtime/parts/alloc.c`, 31 lines over `62ce17563`'s: a `_Thread_local` depth around the
  allocator's eight sites; entering it again on the same thread stops the program with a fixed
  message by `write(2)` and `abort()`.
  - `protoD/storm.log`: 5 of 5 exit -6 with the named stop; `protoD/quiet.log`: 3 of 3 exit 0 after
    758,041, 718,167 and 770,519 signals. `r2494/linux-protoD.log`: storm 3 of 3 exit -6, quiet 3 of
    3 exit 0, `t3x1` right (`tick 1` to `tick 3`, `stopped after 3 ticks`, exit 0).
  - Cost: the storm's loop with no signal, one run each, 105,522,880,949 instructions on the lane's
    runtime against 112,830,170,645 on protoD's (`protoD/time-storm-lane.txt`,
    `protoD/time-storm.txt`), +6.9% (`python3 -I -c 'print(112830170645/105522880949)'`: 1.0692).
  - What it does not see, by the lane's reading: a signal landing inside C's own allocator or stdio
    during a C call, a handler's `print` while `main` prints, the drop lists, the frame room.
- **E, the Heroes function run later, on a runtime thread, never inside the signal** (the shape the
  lane names as Go's `os/signal` and Python's `signal` module): **not built.** It changes what
  `signal` does for a program that hands it a function.
- **F, a route nobody built, added by the coordinator for the seats to weigh**: C's detection with
  E's delivery, the thunk that finds itself entered by the signal delivery hands the call to a runtime
  thread instead of refusing it. Whether it can be built (what a deferred call does with
  `siginfo_t *` and the context pointer of an `SA_SIGINFO` handler, which live on the interrupted
  stack) is a question.

The lane's recommendation, its report's words: *"land D in this round, and take C, in its thunk form,
to a sitting"*; the coordinator's question is wider: which route, or which combination.

## The questions

1. **Which route, or combination, is the most robust and complete repair of 2494**: closing the
   memory corruption for every handler a program can install (by `signal`, by `sigset`, through its
   own C), on POSIX and on Windows, and what each refuses that works today: the three tracked
   programs, `quiet`, `t3x1`.
2. **C's detection, and its exceptions**: a handler on an alternate stack (`sigaltstack` with
   `SA_ONSTACK`), frames with no frame record (C code built without frame pointers between the
   delivery and the thunk), a handler the program's own C installs that calls a Heroes function, a
   signal that lands while the thread is in C, x86-64 against arm64, Linux's and Darwin's delivery,
   and the probe SIGURG at every start (what a debugger sees).
3. **E and F**: what a deferred handler means for a program: the order of a handler's effect and the
   interrupted code, a fault signal that cannot be deferred (SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT),
   a handler that must run before the program ends, and how the runtime's thread meets
   `hero_thread_guard`, the platform layer's stop and `run.c`'s forwarding.
4. **A**: what a name-based refusal catches and misses, what it refuses at build that runs safely
   today, and its diagnostic's words and `Fix`.
5. **The spec**: whether § 13 owes a sentence for the route adopted, priced (`spec-warden.md`), and
   what a reader of the spec writes with it and without it (`llm-ergonomist.md`).
6. **Windows**: what each route does where UCRT runs SIGINT on a thread of its own and SIGABRT,
   SIGFPE, SIGILL and SIGSEGV on the faulting thread (Microsoft's `signal` page, to read), compiled
   with `zig cc -target x86_64-windows-gnu`, the caches inside your folder, never run.
7. **D's place**: whether the adopted route keeps D, makes it moot (and so its +6.9% removable), or
   needs it as the backstop for what it cannot see.

## What the coordinator measured at the base (each with its command)

- **The spec**: 492 lines and 28,770 bytes (`wc -l -c spec/heroes-spec.md`), sha256
  `41ad6690571bd27b...` (`shasum -a 256`, equal to `git show ae5e2cedd:spec/heroes-spec.md |
  shasum -a 256`); `heroes-next measure spec/heroes-spec.md` from the base's root: claude-legacy 8340,
  cl100k_base 8490, maximum 8490, **real 11108** (*"claude-opus-5, 2026-11-13 — the binding number"*,
  no `STALE`), headroom 5276 against 16384, 11168 judged with the FFI floor's 60.
- **The candidate sentences**, each appended with one leading space after the one occurrence of
  `` `atexit(f: (function() -> ()))`. `` (spec line 404), made by
  `.claude/worktrees/p247-scratch/price/drafts.py` and priced by the base's `heroes-next measure`
  (vendored maximum, the delta a lower bound): `spec-warden.md` holds the table.
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
- Whether a deferred handler (E, F) can keep the meaning of `SA_SIGINFO`'s two pointers.
- Whether `claude -p` can pass its `--max-budget-usd` cap by one request (the blind seat's bound does
  not rest on it; `llm-ergonomist.md`).
