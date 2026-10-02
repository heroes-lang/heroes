# Panel 185, the compiler-engineer's brief

Read `00-shared.md` in this directory first; it holds the five questions,
their routes and every measured fact, and the rules of your directory
(`<scratchpad>/185-compiler-engineer/`, a copy of `03e70520`, your compiler
built inside it from the seed). Write your report as you go to
`docs/panel/185-reports/compiler-engineer.md`.

**What only you can give the sitting**: for each route of each question, what
it costs in the compiler (modules, lines, and the frames of the recursive
walks it widens: *recorded*, lane flow's note of 2026-10-01,
`docs/panel/185-briefs/probes/q3/lane-flow-first-pass-100-112.md`, *a first
draft lost 6 and 7 levels* until the arm rule moved out of `arms`), what it
moves (a census of `check --brief` and its exit over the 1,530 tracked `.hero`
files, both arms, the frozen tree's compiler against your prototype's; **for
Q1 the census is `build --emit-c`**, since `check` never runs the `extern`
probe: over the 412 files with an `extern` line, 206 emit today and five are
`ffi_unknown_name`, none a macro, `probes/q1/extern-emit.tsv`), and
**the route the sitting adopts built in your copy and run on the cases this
sitting names**, before the synthesis is written (`.claude/skills/panel/SKILL.md`
§ 3c: a route that does not build is not adopted).

Pointers, each read by the coordinator on `03e70520` (`grep -n`):
- Q1: `selfhost/emit/extern_probe.hero:171` (the probe line);
  `selfhost/emit/probe_reply.hero` (how clang's answer is read);
  `selfhost/emit/ffi_declared.hero:159` (the message); the second round
  that exists, `selfhost/cli/assemble.hero:4-24` (a handle's tag, panel 152
  R1), and `selfhost/cli/produce.hero`; defect 145's note,
  `selfhost/emit/ffi_incomplete.hero`.
- Q2: `selfhost/grammar_expr.hero:1165-1209` (`arm`, which reads a whole
  `Statement` on the arm's line) and `selfhost/parse/arm_body.hero:25`
  (`reject_declaration`); spec § 8's `Inline` at `spec/heroes-spec.md:242`.
- Q3: `selfhost/check/walk.hero` and `selfhost/check/join.hero`
  (`join.Branch.jumps` and `every_branch_jumps`, defect 139's repair,
  `201b99af` and `3a25b640`); the shapes the adopted route is run on are
  `probes/q3/a55`, `a69`, `a73`, `b1` to `b8` and `c1` to `c6`.
- Q4: `selfhost/parse/arm_line.hero` (the spaced sign's fix).
- Q5: `selfhost/lex_interp.hero` (the `f` literal), `selfhost/literals.hero`
  (the plain literal); panel 184's prototypes of (1a) and (1b) were built in
  `<scratchpad>/184-compiler-engineer/` on the tree of 2026-09-30: read their
  changes and RE-APPLY them in your own copy of `03e70520`, never run those
  binaries against today's corpus, and never carry its 34.

Two measurements the sitting needs from you and nobody else: **Q5's false
alarms** on the tracked files for each route, (5d) included, with every false
alarm read and named; and **Q3's census**: does any tracked program change
exit under (3a), and does (3a) keep R4 of panel 184 (a statement after a jump
refused, ratified, not yet landed) consistent.
