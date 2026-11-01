# Panel 233, shared brief: defect 2278, records of records some megabytes deep and clang at -O2

**Lane: soundness** (`.claude/skills/panel/SKILL.md`), the compiler-engineer and the ffi-pragmatist.
Convened by the author's answer to decision 2283, *(A)*, on 2026-11-01
(https://github.com/heroes-lang/heroes/issues/2283), the milestone's one panel gate; the tag
`m-latency-ceiling` waits on this sitting's ruling and its repair. The tree is frozen at
`0ee14c9bec8ca005fe103e4e71fed6dec0b54f57` until the synthesis.

## The rules every seat keeps

- Your worktree is `.claude/worktrees/233-<seat>/`, made for you by the coordinator, a detached
  checkout of the frozen commit. Build your own compiler inside it **in two stages**, `clang -I runtime
  seed/heroes.c runtime/runtime.c -o heroes` then `./heroes build selfhost/main.hero -o heroes-next`:
  the seed alone gives `sources 1831f7e156474293`, six commits behind the frozen tree, and the frozen
  compiler every measurement compares against is the stage 2, `sources 2764fc8525370b80` (the
  critic's first pass, `233-reports/critic-briefs.md`). Never the trunk's `./heroes`, never another
  seat's directory, never a file outside the repository's root, the session's scratchpad included.
- **No wall clock.** Two seats run at once and contend for this Mac, so a duration either reads is
  not a measurement. Measure clang's work in **instructions retired**, which a parallel job does not
  move: `/usr/bin/time -l clang -fintegrated-cc1 <flags> -c <unit>.c -o /dev/null`, reading the lines
  `instructions retired` and `maximum resident set size`. **`-fintegrated-cc1` is required**: without
  it the driver spawns `cc1` as a child and `time -l` counts the driver alone (89,153,154 at depth 16
  without it, 63,059,557,301 with it). **Use the build's own flags**, which include
  `-gline-tables-only` (5.0 % of the instructions at depth 16, the critic) and, on Linux,
  `-fstack-clash-protection` (`selfhost/cli/flags.hero`); read them there. A reading reproduces to
  within about 1 to 3 % (the critic: 0.8 % at depth 16, 3.2 % at depth 18), so a difference under 5 %
  is not a finding. **Memory is a cost too**: clang peaked at 2.78 GB at depth 16 and 12.29 GB at
  depth 18 on this 24 GiB Mac (the critic), so each seat runs one depth-18 compile at a time, and every
  route is judged on memory as well as instructions.
- No paid run (no `claude -p`, no API call, no cloud run), no credential printed (`gh auth status`
  only, never `gh auth token`). A run that may not end goes under `timeout` with its output bounded,
  and any `clang` it leaves is looked for (`pgrep -fl clang`, then `lsof -a -p <pid> -d cwd` to see
  whose it is) and stopped.
- Write your report as you go, `docs/panel/233-reports/<seat>.md`: per item a verdict, the route you
  recommend, its cost in lines of `selfhost/` and `runtime/` (count with `wc -l`), a falsifiable
  prediction with the instrument that scores it, and the condition under which your verdict is wrong.
  Every number with the command that produced it; a negative sentence either run or written as a
  question.

## The defect, as filed

Defect 2278 (https://github.com/heroes-lang/heroes/issues/2278), `adjacent`, `blocking` at its
second tag: a legal program declaring records nested by copies builds at `-O0` and, at `-O2` (the
level of `heroes run`), its unit's clang does not finish. The filing names **four shapes**, none
committed: panel 232's `bigframe18` (clang still running after 9 min 13 s when stopped); a record of
128 MiB of eight-way records built in one function (past `timeout 600`); the same passed by value to
another module's function (past `timeout 120`); the same built level by level in a module of its own,
one call per level or eight (past `timeout 60`). The reproducer below is the first shape only, and it
does NOT reproduce the filing's reading of it: here it finishes, in about 300 billion instructions.
**The other three shapes are the seats' to write and measure**, one depth-18-sized compile at a time.

## What the coordinator measured on 2026-11-01, each with its command

The reproducer, `docs/panel/233-briefs/repro/bigframe{12,14,16,18}.hero`: `R0` holds two `i64`,
`R_k` holds two `R_(k-1)`, so `R_k` is 2^k words of 16 bytes (`R18` 4 MiB), and `make_k(n)` returns
`R_k(left: make_(k-1)(n), right: make_(k-1)(n + 1))`. `repro/bigframe18-unit.c` is the program's unit
as today's compiler writes it (569 lines; `wc -l`).

- Whole builds, `heroes build bigframe<k>.hero -O2`, one reading each on a still machine (`/usr/bin/time
  -p`, `real`): today's compiler (`sources 2764fc8525370b80`) 0.91, 1.70, 5.41, 25.75 s at k = 12, 14,
  16, 18; the compiler before M-compatibility-promise's batch (`afe983265`'s stage 3, `sources
  3e3c95dc246b73b3`, against its own runtime) 0.93, 1.70, 5.56, 24.20 s. So the batch, defect 2253's
  guard among it, did not make this; the coordinator's guess that 2253's `__optnone__` `main` did was
  measured false.
- `heroes build bigframe18.hero -O0`: `real` 0.63 s. **But the program does not run at `-O0`**: at
  k = 16 and k = 18 it stops with `panic: stack exhausted` (exit 134) at `-O0` and prints its answer
  at `-O2` (the critic, at the frozen commit), the frames `-O2` elides being ones `-O0` keeps. So a
  route that makes the `-O2` frames look like `-O0`'s (R2's `optnone`, R1's opaque copies) can turn
  today's slow but correct run into that abort: a robustness question (design.md §1.12), weighed
  before the compile cost.
- `clang -O2 -ftime-report` on the k = 18 unit (flags below): `SROAPass` 24.51 of 25.46 s, 96.3 %;
  `InlinerPass` 3.5 %; the front end 0.4 %.
- Instructions retired, `/usr/bin/time -l clang -fintegrated-cc1 -std=gnu11 -O2 -fno-strict-aliasing
  -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -I runtime -c <unit>.c -o
  /dev/null`: 63,059,557,301 at k = 16 and 306,941,806,654 at k = 18 (x4.87 for a record x4).
- The coordinator searched clang's hidden options for the word *sroa* only (`clang -cc1 -mllvm
  -help-hidden`), and found `--sroa-skip-mem2reg`, which makes depth 16 70 % worse (the critic). The
  critic found `--opt-bisect-limit` and `--inline-threshold` on this clang, and `-opt-disable=` on the
  arm64 image's clang 22.1.8 and not on Apple clang 21. Options on the CI's Ubuntu clang and the
  Windows box's are unrun.

## Prior art inside the compiler, read by the coordinator (verify what you rely on)

- `selfhost/emit/byte_copy.hero` (120 lines): past `IN_ORDER_FROM` (32, `selfhost/emit/typeorder.hero`,
  409 lines) a unit defines `HERO_COPY`, a `memmove` with the type checked, for assignments of its own
  structs (defects 469 and 522, a front-end cost cubic in a chain's DEPTH); `HERO_COPY` does not reach
  a call's result, which stays an assignment, and most of the reproducer's copies are call results.
  `HERO_COPY_HELD` passes both addresses through `hero_slot_escape` (`runtime/heroes_runtime.h:269-272`,
  an identity in `runtime/parts/str.c:226` clang cannot see into); its comment's 139 to 16 s is the
  Windows box's clang 23 on whole compiles of a thousand-deep chain of COUNTED records (defect 566),
  and **the same comment records the escape as a loss on an `i64`-only chain, 28 s plain against 48 s
  through the identity**, which is the reproducer's case. Defect 2278's cost is the WIDTH of one
  record, not a depth: below 32 levels none of that is defined.
- `runtime/heroes_runtime.h:389-408`: the C `main` is `__optnone__, __noinline__` since defect 2253.

## The questions

1. Is this cost compiler-need (CLAUDE.md § 13: *a cost that stops a needed program from running at
   all is compiler-need and goes to the panel*, CL-006), or a refusal the language should name?
2. The most robust route that lets such a program run at `-O2`, with every other program's emitted C
   unchanged where possible. Routes the coordinator can name, every one unrun: (R1) records past a
   size, or past a count of scalar fields, copied, built and returned through memory the optimiser
   cannot split, `HERO_COPY_HELD`'s shape keyed on size instead of depth; (R2) a function whose frame
   holds such a record compiled `__attribute__((optnone))` or at a lower level; (R3) such records kept
   in the heap behind a reference, copied on write; (R4) a refusal with a diagnostic past a size; (R5) a
   compiler flag, where a platform's clang has one (`-fno-inline-functions`, `-inline-threshold`, the
   critic). And the critic's: **(R6) `noinline` on the functions that build or return a large record,
   measured by the critic: depth 18 from 297 billion instructions and 12.29 GB to 1.86 billion and
   10.6 MB, below `-O0`, the return's calling convention unchanged on one case; it does nothing for the
   one-function shape (a 2 MiB eight-way record built in one function, 42.3 billion either way)**; (R7)
   a construction written straight into its destination and a large record returned through the
   caller's slot, unrun, which may address the frame as well as the compile; a bigger stack for a
   program with large frames, paired with another route; a record past the threshold as one byte array
   in C, as the 2253 header's is. A memory-bounded clang with a fallback level is named to be refused
   with reasons. Name any route not on this list. **R3 and R4 are outside this lane**: R3 has spec § 3
   behind it (*a record holds its fields by value*) and R4 is a new diagnostic, so if either wins the
   sitting recommends the full panel rather than adopting it.
3. What threshold, in which unit (bytes, scalar fields, depth times width), and the program in the
   tree nearest to it on each side (`tests/`, `examples/`, `selfhost/`). The critic searched
   `selfhost/` and found no function that gives a record's size in bytes or its count of scalar fields
   (depth is computed in three places), so a threshold in either unit is new code; verify.
