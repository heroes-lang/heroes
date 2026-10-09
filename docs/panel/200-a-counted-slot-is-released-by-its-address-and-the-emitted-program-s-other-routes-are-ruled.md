# Panel 200: a counted slot is released by its address, and the emitted program's other routes are ruled

Convened 2026-10-09 by the coordinator on the author's instruction of about
19:35, meant as: *launch the single panels, then merge their solutions into
one lane; goal, every defect closed within four hours*, for five improvements
of the emitted program and its runner: defects 453, 465, 470, 472 and 523.
**The soundness lane**: the compiler-engineer and the ffi-pragmatist, the
completeness critic before the seats and after them; no surface, no
diagnostic class and no spec token at stake (the first pass's lane check),
what it gave up said below. The tree frozen at **`36be56d0`**, worktree
`lane-panel-200`. Briefs written from 19:46; the critic's first pass between
19:50 and 19:55, its fourteen repairs to both sittings' briefs applied before
any seat started, **one of them falsifying the premise of the author's
decision on 470, a premise the coordinator had written** (the emitted C is
already one exit per function); seats from about 19:56, the ffi-pragmatist's
reply at 20:32 and the compiler-engineer's at 20:36; the critic's second pass
from 20:37 to 20:52; this synthesis from 20:55, every time read from
`date`. Briefs in `200-briefs/`, reports in `200-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | Q1 **approve route B** (`--emit-c` compiles the fused unit), object to a second `-fsyntax-only` pass; Q2 approve the sentinel; Q3 **approve route C**, release by address, the repair; Q4 approve the prologue carrying the function's line; Q5 **object to reversing panel 182**, 523 a known cost, and a defect found beside it; no veto | four compilers built in its copy (B, B+C, B+C+D), lldb on this Mac, a C model of Q5 |
| ffi-pragmatist | Q2 **approve the sentinel** (route A), the watcher not enough alone; Q1 approve a check, its refusal not an exit 2 | three runtime routes built on this Mac, compiled on Linux arm64 and the Windows box; a real two-header program |
| critic, second pass | Q1: route B would tell a false message, today `heroes test`'s, the header blamed chosen by the order of the `use` lines; Q3: route C with ABI 29 breaks the stamp's one job, +0.8 to +1.1% on the compiler; Q4: route D grows the seed 28%; Q2: killing the sentinel kills its runner; Q5: the deep record's exit 2 reproduced and bounded | its own copy, the seats' prototypes read, the compiler built from the B+C source both ways, depths 33 to 1,000 |

## What the sitting measured

- **Q1, 453**: a program of two modules binding `a.h` and `b.h`, each
  defining a `static inline twice` of another type, runs (6 and 8, exit 0),
  and its `--emit-c` artifact is refused by clang `-fsyntax-only` (the
  ffi-pragmatist): an artifact nobody compiled carried header checks nobody
  evaluated, and one is false. A second pass costs 0.204e9 instructions on a
  small program (+31% on every `--emit-c`) and 26.64e9 on the compiler
  (+6.0%). **Route B**, compiling the whole-program rendering as the build's
  one unit for `--emit-c` (`selfhost/cli/assemble.hero`, one code line),
  writes a byte-identical artifact of `selfhost/main.hero` and drops warm
  `--emit-c` of the compiler from 442.29e9 to 254.88e9 instructions; the
  two-header program is then refused at exit 1, `ffi_header_refused` on `b.h`,
  as `heroes test` already refuses it. That message says `b.h` *does not
  compile*, and `b.h` compiles alone: a false message today in `heroes test`.
- **Q2, 465**: before any change 0 of 5 programs ended when their `heroes` was
  killed with SIGKILL, each adopted by launchd. The sentinel, one process per
  runner holding a pipe, ended 10 of 10, a chain 4 of 4, and a non-Heroes
  child (`/usr/bin/yes`) 3 of 3, for about 0.95M instructions once per runner,
  nothing measurable per launch, one 1 MB process while the runner lives and
  none after; the kqueue watcher inside every program ended 4 of 10 at 0.5 s
  under load and no non-Heroes child, and adds a thread and 32 KB to every
  program; `exec` in `heroes run` ended 10 of 10 and covers `heroes run` alone.
- **Q3, 470**: the memory is the promotion of every counted slot to a value
  merged at the one exit, slots times returns (two controls: releases taking
  the slot's address, 800 returns from 2,482.8e9 instructions and 3.58 GB to
  37.5e9 and 98 MB; a `volatile` read, unchanged). **Route C**, a string,
  array or map slot released through `hero_{str,array,map}_release_at(&slot)`:
  200, 400 and 800 returns from 101.5e9 / 257 MB, 505.9e9 / 934 MB and
  2,482.8e9 / 3.58 GB to 7.38e9 / 37 MB, 17.88e9 / 56 MB and 37.61e9 / 113
  MB, growth per doubling from about 3.7 to about 2.2; at run time +2.2 to
  +3.2% on a release-heavy function; 287 blessed emissions and the seed move;
  record slots, released by an inlined local function, keep the growth until
  that release is called by address too (measured by hand, `noinline`: 9.5e9
  and 22.5e9 at 200 and 400).
- **Q4, 472**: with the prologue's `#line` the function's own
  (`selfhost/emit/body.hero`, one code line), `step` into a function and a
  breakpoint on it land on its `function` line, not the generated C; the
  `lines` suite reads 422 and 0; the exit and cleanup stay under the generated
  file (defect 335's ruling); every emission moves by its prologue's `#line`.
- **Q5, 523**: the walk is mostly a call result's assignment (defect 522's
  walk), the missing initialiser costing about 2.2e9 of 12.5e9; the seed's
  deepest chain is 8, the corpus's 5, and only four stress cases pass 32. **A
  defect beside it**: a 1,000-deep record whose bottom holds a `str` fails
  `heroes build` at exit 2, clang dying (Segmentation fault: 11) on the
  emitter's `= {0}`; the same C zeroed with `__builtin_memset` compiles, links
  and prints.
- **The critic's second pass** (`completeness-critic-pass2.md`): `heroes test` tells the
  two-header program that `b.h` *does not compile*, and with the `use` lines swapped
  that `a.h` does not, while each header compiles alone and `check` and `build` say
  nothing: a false message today, which route B would make `--emit-c` share. Route
  C's emission against the frozen ABI-29 header: clang refuses the call to the
  undeclared `hero_str_release_at`, past a stamp that passes; the compiler built
  from the B+C source and emitted by route C checks `selfhost/main.hero` at +1.0 to
  +1.1% instructions at `-O0` and +0.8% at `-O2`. Route D alone emits the compiler
  as 59,014,593 bytes against the seed's 46,097,621, one `#line` per prologue line.
  `kill -9` of the sentinel ends its runner at 141, SIGPIPE, no file of `runtime/`
  handling it. The deep record: depths 33 to 750 build and print, 875 and 1,000
  die in clang; an `i64` at the bottom builds at 7,000; one `str` drops the depth
  to under 875.

## Disagreements, stated plainly

- **Q1's refusal.** The ffi-pragmatist reads the two-header refusal as a new
  class (naming both `extern` groups), a sitting's; the compiler-engineer
  routes it through the existing classes at exit 1. The critic's run settles it on the ffi-pragmatist's side: the one diagnostic `--emit-c` would print under route B is false and chosen by `use` order, so route B waits for a true message, and what the checker should say of two groups binding one C name with different types is a full sitting's (a diagnostic class).
- **Q3's ABI.** The compiler-engineer left `HERO_RUNTIME_ABI` at 29, the
  additions only adding. The critic measured the stamp passing and clang refusing the new call: the ABI moves to 30 with route C, as it moved 12 to 13 for one function and 28 to 29 for three (`4cafbb05`, `bcdbf694`), every blessed emission's `_Static_assert` line moving with it.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040), and where the robust route is ruled but not yet built whole, the
question stays open with its route written, never a cheaper one adopted in its
place. What conservative would have been is below the list.

1. **R1, defect 453: route B is the route, its landing waits.** `--emit-c`
   compiles the whole-program rendering as the build's one unit, so the file
   written is the file clang read. It lands once the two-group program is told
   truly: **filed now as a `blocking` defect**, `heroes test` blaming one
   header for two groups binding one C name with different types, the header
   chosen by `use` order; what `check`, `build` and `test` should say of such a
   program is a diagnostic class and a full sitting's. 453 stays open behind
   it. Refused: a second `-fsyntax-only` pass (+31% on a small program).
2. **R2, defect 465: the sentinel is the route, its landing waits.** One
   process per runner on macOS, holding a pipe, which ended 10 of 10 and every
   non-Heroes child; owed before it lands, each built and measured: SIGPIPE
   (the write end `F_SETNOSIGPIPE`, and what the runner does when its sentinel
   is gone, a restart or a told end), FREE written before the reap past 1,024
   live children, the guard `defined(__APPLE__)`, and a raylib-binding program
   starting its sentinel. 465 stays open with this route. Refused: the kqueue
   watcher in every program (4 of 10 under load, no non-Heroes child), `exec`
   in `heroes run` alone (covers `run` only).
3. **R3, defect 470: route C, a counted slot released by its address**, with
   `HERO_RUNTIME_ABI` moved to 30, a record slot's release called out of line
   so the escape survives inlining, `selfhost/emit/inst.hero` split under its
   ceiling, `guarded_names` taking the three names, and the premise written
   beside it (the runtime is a separate unit; LTO or a unity build would undo
   it). Measured: 800 returns from 3.58 GB to 113 MB at `-O2`, the compiler
   checking itself +0.8 to +1.1%. It lands in batch 17 if the landing lane
   reaches it whole; otherwise the next batch, 470 open until then.
4. **R4, defect 472: the prologue carries the function's line, in one
   physical line.** Route D's effect, a `step` landing on the `function` line,
   is adopted; its shape, one `#line` per prologue line (+28% on the seed), is
   not: the prologue written on one line under one `#line` (the critic's
   route, about 2 lines per function), with design.md §3.1's sentence amended
   and a `lines` case pinning the new mapping. Unbuilt, so 472 stays open with
   this route.
5. **R5, defect 523: a known cost, closed with its measurement**; panel 182
   stands (no real program past depth 16 but four stress cases; the seed's
   deepest is 8). **Filed now as `blocking`**: a record 875 or more deep
   holding a `str` makes clang die on the emitter's `= {0}` (exit 2, a note
   blaming the C compiler's limit), where `__builtin_memset` zeroing builds at
   1,000; its repair the zeroing's spelling, the slot still zeroed as panel
   182 rules.
6. **R6, the cards**: 523 closes now; 470 closes when R3 lands; 453, 465 and
   472 stay open, each with its route; filed: the two-group false message
   (`blocking`), the deep record's exit 2 (`blocking`), a release inside a
   line stopping under the generated file (`improvement`, the
   compiler-engineer's Q4).
7. **R7, refused**: a second `-fsyntax-only` pass for 453; the kqueue watcher
   and `exec` alone for 465; slot coalescing for 470 (nothing to merge on the
   shape, an inference the sitting did not build); route D's per-line `#line`
   for 472; reversing panel 182 for 523.

**The conservative alternative, the author's to choose instead**: close 453,
465, 470 and 472 as known costs with these measurements, and land nothing of
this sitting but the two `blocking` repairs.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | Q1: `emission` 0 failed, the seed's artifact unchanged by `cmp`, warm `--emit-c` of `selfhost/main.hero` at most 260e9 instructions; Q3: the 800-return string shape at `-O2` at most 0.15 GB and 50e9, `bench.hero` at most +4%; Q4: `lines` 0 failed, emissions moving by prologue `#line`s only | the landing's gate |
| ffi-pragmatist | Q2: a program binding raylib starts its sentinel without the fork-safety abort and its child ends on SIGKILL | unrun, a later sitting's |

## Author's verdict

**RATIFIED, 2026-10-09**, R1 to R7 as written above, the author answering
through the question widget between 20:55 and 20:57 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R7* over
the conservative alternative and *I want to read it first*, on the coordinator's summary of each route.
Recorded as a reading (CLAUDE.md § 4). The author may overturn it.
