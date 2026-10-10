# Panel 204, compiler-engineer

Copied by the coordinator at 02:14 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 204, compiler-engineer

I worked from 01:30 to 02:14 on 2026-10-10 (by `date`). My copy of the frozen tree `635e8f67` is at `.claude/worktrees/scratch-b15/204-compiler-engineer/tree/`. The prototype compiler is `heroes-p3` and my running notes are in `notes.txt`, both in that folder.

The current compiler reproduces every case as the brief gives it.

## Fields

- **verdict:** approve, for the route I built and ran (below).
  - **Veto** on per-group units.
  - **Object** to the byte-sorted header order and to the comparison of a module over several orders.

- **section:**
  - design.md §1.1, the ceiling.
  - Part 5: the seven core constructs. This route adds none.
  - §4.19, `docs/design.md:2366`: *"the parser flattens it … no later pass ever learns the word 'group'"*. This is the ground of the veto.
  - §4.19, `:2370`: *inline functions and `#define` constants are reachable because the C compiler sees the real header*.

- **implementation_cost:** 4 files and +149 lines in `layout`'s own count (non-blank lines outside `test` blocks, comments included). Nothing lands in the lexer, parser, checker, IR, type descriptors or ownership pass.
  - **`selfhost/emit/externs.hero`, 192 → 212 lines.** `headers()` now collects the indices of the bound declarations and of the record groups, and walks them in sorted order. So a group holding only records keeps the place it is written in; until now it was included after every other group. One test added.
  - **`selfhost/cli/flags.hero`, 286 → 287 lines.** One flag: `-Werror=macro-redefined`. Redefining a macro differently violates a constraint of C11 §6.10.3p2. That is my reading of the standard, not something I ran. The two pinned counts in its test move from 17 to 18 and from 20 to 21.
  - **`selfhost/cli/whose.hero`, 108 → 117 lines.** `told_header` now receives the compile settings (`under`).
  - **`selfhost/cli/header_order.hero`, new, 119 lines.** It runs only after clang has refused a header. It tries each other group of the same module on the other side of the refused one, at most n−1 runs of `clang -fsyntax-only` on the real unit's opening. It reports the first order that compiles. It costs nothing on a build that compiles.

  What it costs, measured:
  - **Warm build of `selfhost/main.hero`:** 400.19 G instructions retired on the current compiler, 400.24 G on the prototype (41.20 s against 41.23 s). Run-to-run noise is about 0.4%.
  - **My first draft cost +3.8%** (415.98 G), from a `fail()` built for every declaration. I measured it and rewrote it.
  - **`jpeg/ab.hero` warm build:** user+sys 0.24 s on both compilers.
  - **The refused `ba.hero`:** about 0.03 s more user time on the prototype, for the trial compile. The machine was at load 23, so these timings are indicative only.

  What it moves:
  - **The 107 tracked files with two or more headers:** 0 moved in exit code, stderr (paths normalised) or the output of the 44 that run.
  - **`--emit-c` over the 309 extern-using files** of `tests/golden/{emit,run,ir,fixedbugs}` and `examples/`: base and prototype output byte-identical in all 309.
  - **`heroes test selfhost/main.hero`:** 1544 tests, all passed.
  - **Re-running panel 202's Mac header-pair census with the new flag:** 4 of the 24,148 ordered pairs that compile today are now refused, all for a redefined macro.
    - `expect.h` with `jmorecfg.h` fails in both orders (`EXTERN`).
    - `tcldbg.h` before `jmorecfg.h` fails (`EXTERN`).
    - `tcldbg.h` before `turbojpeg.h` fails (`DLLEXPORT`).

  The routes I did not build:
  - **Byte-sorted order:** about 5 lines, and it is refused by `jpeg`.
  - **Comparison over orders:** one extra `clang -E` per extra order, per module with two or more groups, on every build. My shell experiment measured about 0.05 s user per order on jpeg's pair. Being complete needs n! orders.
  - **Per-group units:** they would cut through `emit/unit.hero` (234 lines), `emit/members.hero` (343), `cli/units.hero` (525) and `cli/assemble.hero` (324). They need a group identity past the parser, plus a wrapper for every bound function, because a module's own C calls the C name directly (`cfgone`'s `main.c`: `t2 = half(t1)`).

- **needed_for_self_hosting:** no. The compiler's three modules that name two headers build unchanged. This is a repair of a `blocking` defect: robustness and truth, CLAUDE.md § Precedence 3 and § 12.

- **argument:** Every route but the sentence buys a construct or refuses correct C. Per-group units need the IR to learn the word group, which §4.19 forbids (`design.md:2366`). A module's own C calls `half(t1)` and builds the header's structs, so its unit needs every header anyway. A comparison refuses `cfgone/main` and `dual`, both correct C, and misses `mpq`. A byte sort refuses `jpeg`. What is true and cheap: C reads a module's headers in the order its groups are written. Make that true (record-only groups came last). Make the one order-dependence C11 calls a constraint violation an error. Tell the move that compiles, only on failure: +149 lines, and within noise on a build that compiles.

- **prediction:** At the gate of the batch that lands defect 563 by this route:
  - `selfhost/` grows by at most 160 lines in `layout`'s count, over at most four files;
  - `emission`, `emit`, `run` and `warnings` read 0 failed with nothing re-blessed.

  It is falsified by any re-blessing or a 161st line.

- **condition:** Any one of these changes my verdict.
  - **The flag:** the Linux census, re-run with it, shows more than 1% of today's compiling pairs failing in both orders. Then I would drop the flag and tell the warning as the author's diagnostic instead.
  - **The written order:** a tracked program turns out to depend on record-only groups coming last.
  - **The comparison:** a measured, non-trivial count of installed header pairs that compile in both orders and preprocess a bound header differently (the critic's question 4).
  - **Per-group units:** a §4.19 amendment that lets a later pass know a group, together with a program only per-group units can build.

## What the cases do on the prototype

- **`cfgone/main`:** builds and prints `3 50`.
- **`cfgone/main2`:** exit 1, no warning, with this note: *these compile when `a.h` is included before `b.h`, which the group at line 1 names: a module's headers are included in the order its groups are written, so write this group above that one*. `run` and `test` agree with `build`.
- **`jpeg/ba`:** exit 1, with this note: *`jpeglib.h` compiles when `stdio.h`, which the group at line 4 names, is included before it: … so write that group above this one*. A single clang run cannot name that header; n−1 runs on the failure path can.
- **`recfirst/rec`:** now builds and prints `1`.
- **`mpq` m-first order:** refused, with the move to make.
- **`one`, `one2`, `alone`:** no move note.
  - For two groups, both orders are tried, so *whichever comes first* becomes true.
  - With three or more groups it still overstates; defect 561 removes that clause.
  - `alone` has no group to move and needs a § 13 ruling, since a group with no member is refused.
- **`dual` (new, `cases/dual/`):** two `#ifndef K` defaults. Both orders build silently, printing `5 105` against `9 109`, on the current compiler and the prototype alike. Only a sentence covers this shape. A draft, not priced by `heroes measure`:
  - § 4: *Declaration order never matters, but between `extern` groups (§ 13)*.
  - § 13: *a module includes its groups' headers in the order they are written, after the compiler's own; a macro two headers define differently is refused*.

## Open task and panel 202's condition

- **The open task.** This sitting can close only the first of the task's five repairs, the §4.19 `#include` sentence, and the prototype makes that sentence true. The task should stay open for the other four.
- **Panel 202's R4 condition** (its R3 comparison instrument) does not bind this route, because nothing in it compares dumps.

## What I did not run

- The full net, the seed and its fixpoint, the net's own tests, and the `check` census.
- Linux and Windows, including the flag against glibc and `_GNU_SOURCE`.
- The pair census on the real unit. My re-run used panel 202's unit: two headers, no prefix, no guard.
- The comparison route inside the compiler. I only ran a python and `clang -E` experiment (`cmp/seg.py`).
- Per-group units and the byte sort: not built.
- The token cost of the spec and design.md sentences.
- Programs with several modules: a record-only group of another module's unit, and whether `test`'s single unit orders several modules like `build` does. For one module I measured that they agree.
- Instructions retired counts the `heroes` process only, not its clang children.

## A mistake I made

At about 01:35 I ran `git -C tree diff --stat` in my copy. The copied `.git` file points at `.git/worktrees/lane-panel-204`, and that lane's index now reads mtime 01:35.

I checked with `--no-optional-locks`:
- nothing is staged;
- HEAD is still `635e8f67`;
- the only untracked entries are the coordinator's `204-briefs/` and `204-reports/`.

At most a stat refresh happened. I then moved the pointer out of my copy (`lane-git-pointer.txt`), so no git command in it can reach the lane again.

Everything is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/204-compiler-engineer/`:
- `notes.txt`
- `heroes-p3`
- `tree/selfhost/emit/externs.hero`
- `tree/selfhost/cli/header_order.hero`
- `tree/selfhost/cli/whose.hero`
- `tree/selfhost/cli/flags.hero`
- `census/pairsw.tsv`
- `cases/dual/`
- `runs/`
- `emitcmp/`
