# Panel 200, the shared brief: the emitted program and its runner (453, 465, 470, 472, 523)

Written by the coordinator on 2026-10-09 from 19:46 (`date`), on the tree
frozen at `36be56d0` (worktree `lane-panel-200`: the trunk as pushed after
batch 16). Convened on the author's instruction of about 19:35, meant as:
*launch the single panels, then merge their solutions into one lane; goal,
every defect closed within four hours*. Every fact below names the command or
file it comes from; a number marked **carried** is a card's, not re-run by the
coordinator, and is a question for the seat, never a premise.

**Lane: soundness** (`.claude/skills/panel/SKILL.md`): the compiler-engineer
and the ffi-pragmatist, the completeness critic before the seats and after
them. None of the five changes a surface, a diagnostic class or a spec token
(453's refusal, if adopted, is the existing *clang refused the generated C*,
exit 2, `.claude/rules/generated-c.md` § Flags). What the lane gives up: the
spec-warden, the historian and the blind seat; say in your report if a
question here has a reader-facing half that would need them.

**The author's decision on 470, 2026-10-09 about 19:40** (the question widget):
*the sitting measures it*: the compiler-engineer builds *one exit point per
function* (a false premise, the coordinator's: see Q3's reframing); if it holds and costs little it is the repair, otherwise 470 closes
as a known cost with its measurement.

## The five questions

**Q1, defect 453**: `--emit-c` writes C that no clang run reads.
`selfhost/cli/artifact.hero:79` (`write_c`) and its comment `:108-117`: the
program builds module by module, and *no clang run of this build read these
bytes*; measured by hand on 2026-10-07, clang `-fsyntax-only` over the
artifacts of the 372 `tests/golden/run/` programs said nothing. The card's
cost of a check before writing (carried): about 0.21 billion instructions on a
small program and 20.9 billion on the seed. **Should `--emit-c` ask clang
`-fsyntax-only` on the artifact before writing it**, and at what cost, on
what flags (the build's own), and what does it tell when clang refuses?

**Q2, defect 465**: on macOS a program started by `heroes run` outlives a
`heroes` killed with SIGKILL. Linux asks `prctl(PR_SET_PDEATHSIG, SIGKILL)`
between fork and exec (`runtime/parts/run.c:639`, its comment `:630-638`);
Windows ties the child to a job; macOS has no such call. Defect 425 is why it
matters: an orphan wrote 146 GB and filled the disk
(`.claude/rules/verification.md` § A run that may not end). Lane b14-runtime
built a sentinel process per runner holding a pipe, which killed the child 3 of
3 for about 0.85 million instructions a fork (carried; its source lived in a
scratchpad a reboot emptied on 2026-10-08, so it is gone). **What is the most
robust way to end the child on macOS**: the sentinel, a watcher inside every
program the runtime starts (a thread on `kqueue` `EVFILT_PROC`/`NOTE_EXIT` on
its parent, since every program `heroes run` starts carries this runtime),
or another route; its cost, and what it adds to every run.

**Q3, defect 470** (**reframed at 19:55 on the critic's first pass: the
premise of the author's decision was the coordinator's and it was false**;
the emitted C is already one exit per function, `selfhost/ir/exits.hero:1-13`,
wired at `selfhost/ir/lower.hero:56`, landed `6e616898` on 2026-10-04, panel
190's route A-star, defect 231, and panel 197 measured the growth on that
C, `docs/panel/197-reports/compiler-engineer.md:21`. The decision's spirit
stands: the sitting measures **where clang's memory goes in a one-exit
function of many returns** (the C text against the number of slots across
200, 400 and 800 returns, clang's per-pass memory), builds the cheapest
route it finds, panel 182's critic's coalescing of the slots of mutually
exclusive arms among them (`docs/panel/182-*.md:200`), and adopts it only
if it holds and costs little; otherwise 470 closes as a known cost with its
measurement. The paragraph below is the brief as first written.) clang's memory on a function of many returns grows 3.7 to
4.0 times per doubling at `-O2`, under `-g` and line tables alike (panel 197's
compiler-engineer, 400 and 800 returns; carried). The author's decision above:
build *one exit point per function* (every `return` a `goto` to one exit
block holding the cleanup), measure clang's peak memory and instructions at
three sizes before and after, the emissions it moves, `check` and `run` on
the compiler itself; adopt it only if it holds and costs little.

**Q4, defect 472**: `step` into a Heroes function lands on the generated C
(`dbg.c:34:5`, panel 197's compiler-engineer; carried). `docs/design.md:682-688`
(§3.1): `#line` is restored to the generated file around synthetic prologue
and cleanup code *so lldb never blames user lines for housekeeping*. **Should
the prologue carry the function's own line**, so a `step` lands on the
`function` line, and what does that cost the rule design.md states (a
breakpoint or a sanitiser frame in housekeeping then naming a user line)?

**Q5, defect 523**: 2,000 uninitialised declarations of a deep struct cost
+0.57, +1.10 and +2.16 billion instructions at depth 1,000, 2,000 and 4,000
over the same declarations with `= {0}` (lane b15-parse; carried). Panel
182 ruled *a value is never zeroed, a slot is, and every definition is
written whole* (`docs/panel/182-a-value-is-never-zeroed-a-slot-is-and-a-definition-is-whole.md`),
and the `wholes` suite holds the emitter to it. **Is there a shape that
avoids clang's walk without zeroing a value**, or is the cost the ruling's
price, written down? Name what a reversal would give up (`wholes`, the
sanitiser's view of an uninitialised read, panel 182 `:19-20`).

## The rules every seat works under

- **Your own copy, inside the repository's root**: `rsync -a --exclude
  .claude/worktrees --exclude .git
  /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-200/
  /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/200-<seat>/tree/`,
  then your compiler from its seed (`clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`). **No file is ever written outside the
  repository's root** (CLAUDE.md § Hard stops, 2026-10-08); TMPDIR inside your
  folder. Never build, run or write in the frozen tree, the trunk or any
  `lane-*` worktree, or another seat's folder.
- **Running notes** in your folder's `notes.txt` as you go; your final reply
  is your report, and the coordinator copies it into
  `docs/panel/200-reports/<seat>.md`.
- **A run that may not end is bounded** (`timeout`, output to a file or
  `/dev/null`, `pgrep` after). Times only from `date`; counts, never
  durations, unless the machine is still (`/usr/bin/time -l`'s instructions
  retired and peak memory are counts).
- Docker `heroes-linux-arm64:latest` is free (`--rm --pull never`); the
  Windows box (`ssh win`) is being powered on by the author, shared with lane
  b17-fix: one clang at a time, your folder `/c/w/p200-<seat>-<pid>`.
- **No paid run.** A lane, `lane-b17-fix`, works beside you on the harness,
  the hooks and the parser: never touch it.
- **Time box**: the author's goal is every defect closed by about 23:40; report
  by 21:00 with what you have, the unmeasured said plainly.

## Corrections and additions from the critic's first pass

Read 2026-10-09 between 19:50 and 19:55 by the clocks read around it (`completeness-critic-pass1.md`
beside the reports), every one from a command it ran; applied here before
any seat starts, and binding over the text above where they disagree.

- **Q1**: the comment is `artifact.hero:102-117`, the quoted sentence
  `:112-113`; *372* is that comment's count of 2026-10-07 (carried), and
  `ls tests/golden/run/*.hero` gives 422 today. A clang refusal is not
  automatically exit 2: the message (`selfhost/cli/produce.hero:239`) goes
  through `whose.of_refusal` (`:229-240`), which can blame the author's
  `extern` (`.claude/rules/generated-c.md`): say which class an
  `-fsyntax-only` refusal belongs to. **Cost sites**: `--emit-c` is called at
  12 places in 5 harness suites (emission, determinism, lines, surface,
  golden), and the seed is regenerated with `--emit-c` (`seed/README.md:93`),
  where the gate already compiles `seed/heroes.c` with clang. **Routes not
  listed**: for `--emit-c`, compile the whole-program rendering as the one
  unit, so the file written is what clang read and no second pass is needed;
  or the check in the net only (`emission` runs `-fsyntax-only` on its blessed
  artifacts). **Question**: does `.claude/rules/cli-surface.md`'s exit-code
  contract let `--emit-c` start refusing?
- **Q2**: the comment is `run.c:623-638` (`prctl` at `:639`). A watcher thread
  meets: `docs/design.md:1665` (reference counts stay non-atomic only while no
  counted value crosses a thread), the stack guard's SIGSEGV and SIGBUS
  handlers (`runtime/parts/stack.c:796-797`), and a fork from a program that
  now has two threads, inside `hero_run_go` itself. **Scope**: a watcher in the
  child covers only children that are Heroes programs; must clang under
  `heroes build`, and the children of a Heroes program's own `hero_run_go`
  (the harness, `selfhost/mutate/child.hero:70`), be covered too? The
  sentinel covers them. **Routes not listed**: `heroes run` `exec`s the built
  program instead of forking it (no parent left to die; `heroes run` only);
  the child inherits the read end of a pipe the runner holds, the runner's
  end close-on-exec, and a thread reads it to EOF (any POSIX, no kqueue); the
  kqueue watcher must close its race by asking `getppid()` after registering;
  a loop polling `getppid()`. **Question**: how does the child know a runner
  started it (an environment variable is inherited by grandchildren)?
- **Q4**: the mirror case is defect 335 (closed, `9d8b07ae`): a merged exit
  carrying the function's own line made lldb land on the function's head, and
  its repair moved it back under the generated file (`selfhost/emit/term.hero:78-86`).
  The code: `selfhost/emit/writer.hero:223` (`at_generated`) and
  `selfhost/emit/body.hero:132-161`; the `lines` suite judges it. **Route not
  listed**: a prologue that emits no stores, since lldb's step-in lands on the
  first row of the line table, which may be a `= {0}` store.
- **Q5**: panel 182's `:19-20` say a lowering bug reaches the author as
  `internal error:` with clang's words and that `--emit-c` and a debugger show
  uninitialised temporaries; the sanitiser point is `:31-32` (ASan cannot see
  an uninitialised read, MemorySanitizer can, on Linux only) and `:198`. What
  a reversal gives up first: `-Werror=uninitialized`, about 71,602 values
  returned to clang's check (`:70-77`), and the prediction of zero
  uninitialised warnings on the seed (`:229`). **Route not listed**: measure
  with `-Wno-uninitialized -Wno-sometimes-uninitialized
  -Wno-conditional-uninitialized`; if the cost disappears it is the price of
  the very check panel 182 kept. **Question**: does any real program reach a
  nesting depth of 1,000? Measure the deepest struct in the seed and corpus.
