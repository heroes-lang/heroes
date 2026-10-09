# Panel 200, compiler-engineer's brief

Read `00-shared.md` beside this file first. Your verdict is per question, with
the implementation cost in files and lines (the `layout` suite's unit), and
**a route you recommend is built in your copy and run on the cases it names**
before you report (`.claude/skills/panel/SKILL.md` § 3c: a route that does not
build is not adopted). Rebuilding the compiler from `selfhost/` in your copy is
`./heroes build selfhost/main.hero -o heroes` (`real 66.20` and `real 31.29`
in batch 16's two gate A logs, `.claude/worktrees/scratch-b15/gate16t/a/gate-a.log`
and `gate16/a/gate-a.log`).

- **Q1 (453)**: build the `-fsyntax-only` check in `write_c`
  (`selfhost/cli/artifact.hero`), on the build's own flags; count its
  instructions on a small program and on `selfhost/main.hero`; say what it
  prints when clang refuses (the existing exit-2 class) and how a test proves
  it red before the repair (an artifact clang refuses, made by hand).
- **Q3 (470)**, as the shared brief reframes it (the C is already one exit,
  `selfhost/ir/exits.hero`): where clang's memory goes, and the cheapest route
  you can build (slot coalescing among them): clang's
  peak memory (`/usr/bin/time -l`'s maximum resident set) and instructions at
  `-O2` at 200, 400 and 800 returns before and after, the same for `-O0`; the
  emissions it moves (`tests/emission/`, the `emission` suite); `check` and a
  build of `selfhost/main.hero`; adopt or close as known cost by the author's
  rule in the shared brief.
- **Q4 (472)**: where the prologue's `#line` restore is written; build the
  variant that gives the prologue the function's own line; on lldb (this Mac)
  a `step` into a function before and after, a breakpoint on a function, and
  an ASan report whose frame is in housekeeping (a leak or a double release in
  a case of `tests/golden/run/`), each before and after.
- **Q5 (523)**: a shape that avoids clang's walk over an uninitialised deep
  struct without zeroing a value (a declaration at first definition where no
  `goto` crosses it, a union, a byte array with a typed view, anything you
  find); its counts at the three depths; what `wholes` says of it.
- **Q2 (465)**: the ffi-pragmatist builds the runtime routes; read its
  numbers if they land before you report, and judge the cost to every run.

Report per question: verdict (approve, object, veto with the soundness
reason), section of design.md, cost, the cases, the prediction a later run
can falsify, and what you could not run.
