# Panel 245, the build of Q1's route (d) and Q2's S and D in one compiler: the ffi-pragmatist

Written 2026-11-11 by the coordinator after the critic's second pass
(`docs/panel/245-reports/critic-reports.md`, read whole before you start). `/panel` 3c: a route
that does not build is not adopted, and a seat with Bash builds the adopted route in its own copy
and runs the cases the sitting named before the synthesis is written. Q1's route (d) is built at the
C level only (critic items 3 and 8); Q2's S and D you built in one compiler, and their notes cite a
clause § 13 does not hold (item 13). This brief asks you to build both in your worktree,
`.claude/worktrees/245-ffi-pragmatist`, on top of what it holds now (`git -C` that worktree `status`
names your prototype's two modules), and to run them.

## What to build

**Q1, route (d), a fail-closed region.** In the region a group header's own code is compiled in,
every warning clang gives is a refusal, except an allow-list of advice: the two deprecation words,
`#pragma message` (which clang never upgrades, item 3), and `#warning` relayed as defect 248's
surface fixture pins (`tests/golden/surface-fixtures/warning248/`, `suite_surface.hero:133-146`). The critic's list of what (d) owes, each to be built and shown:

1. `-Wuser-defined-warnings` and `-Wimplicit-int` raised again after the region's `-Wall` ignore
   (item 4; item 10 is the false note that `-Wimplicit-int` inside `-Wall` produces, now filed as a
   defect, and its reproducer is `.claude/worktrees/b30-scratch/repro/loop/`);
2. the probes and order trials compiled under the same words as the unit (item 8:
   `cli/compiling.hero`'s `without_diagnostics` drops every `-W` word from `probe_words`), so that
   no note says a header "compiles" in an order the unit refuses;
3. a code-generating trial for `-Wattribute-warning`, which `-fsyntax-only` does not report (item 7),
   and a reading of the level question (item 9): say what (d) does with the `warning`-marked call
   refused at `-O0` and built at `-O2`, and what each answer costs; the ruling is the sitting's;
4. defect 158's carrier (item 11), so `fixedbugs-158-a-note-in-another-header-between` still says
   what it pins.

**Q2, S and D.** `-Wformat-security` raised in the region, and no function its header declares
variadic bound directly. The notes cite no clause of § 13 that § 13 does not hold: either a note
that cites none, or the clause W0 states, which is a spec change the sitting would have to pay for;
build the first, and say what the second's note would read. Lines 220, 230, 250 and 260 of
`emit/ffi_past_prototype.hero` carry the same false citation today; build their repair too.

## What to run, with the compiler you build

- The critic's 17-shape table under your region, at `-O0` and `-O2` (`p245-scratch/critic/second/q1/`;
  read its `run.sh`, run it from a copy in your own scratch folder, never in the critic's), and
  curl's checker (`p245-scratch/critic/second/q1curl/`): what each shape now does.
- The `unsupported` and `run` forms narrowed to the cases each change moves, by the harness's third
  word (`./heroes-next run tests/harness/main.hero -- ./heroes-next <form> <substring>` from your
  worktree's root), at least: `fixedbugs-158`, `fixedbugs-2036`, `fixedbugs-2421`, `fixedbugs-2102`,
  `fixedbugs-2103`, `fixedbugs-2106`; the `surface` suite once at the end, whole, since it takes no
  filter and holds defect 248's `#warning`; and the two programs of
  `.claude/worktrees/b30-scratch/repro/loop/` (copy them first).
- A census of what (d) refuses that the base builds: every tracked `.hero` under `examples/` and
  `tests/golden/run/` that binds a header, built by the base compiler and by yours, the names of the
  ones that move. Bound it: one build at a time.

## How

- Your worktree, your compiler: rebuild stage 2 from the seed's stage 1 in it after each change
  (`./heroes build selfhost/main.hero -o heroes-next`). Never build or run in another seat's
  worktree or folder.
- Batch 30's Linux leg and its last suites may still be running on this Mac: one heavy command at a
  time, no whole suite or form but `surface` once at the end. A program that may not end is built and its binary run under
  `timeout 20`, its output to a file through `head -c 200000`.
- No paid run, no credential printed, no GitHub write, no file outside the repository's root
  (`/tmp` included: write to `.claude/worktrees/p245-scratch/ffi-pragmatist/build/`).
- Write your report as you go, `docs/panel/245-reports/build-q1q2.md` in the worktree
  `.claude/worktrees/p245` (that file only; nothing else there), each item the fact, the command,
  what it printed. End with the diff of your worktree against `7fb6868b2`, saved beside the report
  as `.claude/worktrees/p245-scratch/ffi-pragmatist/build/route.diff`, and its line counts.
