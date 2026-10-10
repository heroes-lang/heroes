# Panel 206, the shared brief: arithmetic on literals that cannot fit its position (577)

Written by the coordinator on 2026-10-10 from 04:49 (`date`), on the tree
frozen at `e0aeb991` (worktree `lane-panel-206`: batch 18's round, holding
lanes b18-close, b18-infer and b18-ffi's committed repairs, defect 564's
among them; not yet the trunk). Convened on the author's instruction,
*as soon as you have CPU, put the last open ones in together*, their answer
through the question widget, *yes, 3.69 USD* for the blind seat, and their
message raising it, *spend up to 30 dollars* for the night's paid runs, all
between 04:37 and 04:49 by the clocks read before and after. Every
fact names its command or file; a number marked **carried** is a report's.

**Lane: full**: what `check` refuses. The compiler-engineer, the
spec-warden, the historian, the blind seat run by the coordinator as fresh
`claude -p` sessions outside the repository, the critic before the seats and
after them.

## The question

Defect 564's repair (lane b18-infer, `ce9c980b`, ratified by panel 203 R3
and panel 042's ruling 3) gives the operands of an arithmetic operator the
type their position asks for. Run by the coordinator between 04:37 and 04:49 with the
round's compiler (`lane-round-b18/heroes`, built by the trial gate from
`e0aeb991`) and with the trunk's (`86189b2b`'s seed, before 564), the cases
under `.claude/worktrees/scratch-b15/p206/`, read only:

| program | the trunk | the round |
|---|---|---|
| `y: u8 = 200 + 100` (`sum`) | refused `type_mismatch`, *expected `u8`, found `i64`* | builds, aborts 134 *panic: integer overflow* |
| `x: i8 @ 100 + 100` (`i8`) | refused `type_mismatch` | builds, aborts 134 |
| `repeat("-", 0 - 1)` (`rep`) | refused `type_mismatch`, *expected `u64`* | builds, aborts 134 |
| `constant BIG: u8` with body `200 + 100` (`const`) | refused `type_mismatch` | builds, aborts 134 when read |
| `show(v: 255 + 1)` with `show(v: u8)` (`arg`) | refused `type_mismatch` | builds, aborts 134 |
| `y: u8 = 300` (`lit`) | refused `int_out_of_range` | refused `int_out_of_range` |

Spec § 2 (`:46`): *Every base writes a value, so a literal must fit its type.*
§ 7 (`:198`): *Overflow aborts at every width.* § 4 (`:114-115`): *a written
body computes over literals and other constants.* Lane b18-infer's reading
(its final reply): refusing such an expression would be a new refusal, Rust's
`arithmetic_overflow` the precedent, a sitting's. **Should `check` refuse an
arithmetic expression whose value it can compute from literals and constants
alone and which cannot fit its position, and where does that stop** (a
constant's body; an operand that is a constant; a `repeat` count; a division
by a literal zero; a shift past the width; a unary minus; an expression with
one variable operand)? **And is the round's run-time abort, where the trunk
refused (for another reason), a regression to land or a repair to finish
before batch 18 closes?**

## The rules every seat works under

As panel 205's shared brief states them (`docs/panel/205-briefs/00-shared.md`
on the trunk, § The rules every seat works under): your folder
`.claude/worktrees/scratch-b15/206-<seat>/`, your copy rsynced from
`.claude/worktrees/lane-panel-206/` excluding `.claude/worktrees`, its `.git`
removed at once and no git run in it; your compiler from the seed in your
copy (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then
`./heroes build selfhost/main.hero -o heroes`, since the round's `selfhost/`
is ahead of its committed seed); four lanes work beside you
(`lane-b18-close`, `lane-b18-ffi`, `lane-b18-infer`, `lane-b18-guard`) and a
trial gate on `lane-round-b18`: never touch them; at most three processes at
a time; never kill a process you did not start; no paid run; no file outside
the repository's root. **Time box**: report within 40 minutes; notes in your
folder as you go.

## Corrections and additions from the critic's first pass, binding

Read at 05:04 (`date`): `docs/panel/206-reports/completeness-critic-pass1.md`,
every one from a command it ran, the coordinator re-running its `u03` at
05:01 (filed as defect 578); applied before any seat starts and binding over
the text above where they disagree. Read it whole; its routes and questions
are part of your brief. In short:
- **`rep` reverses a ratified sitting's witness**: panel 054 (ratified
  2026-08-14) ruled *a negative count cannot be written*, pinned by
  `tests/golden/check/repeat-count-is-unsigned.hero:22`; defect 564's commit
  `ce9c980b` removed that annotation and wrote a correction. Judge `rep`
  apart from the rest.
- **Panel 203's R3 already ratified a run-time abort** for `m2`, `y: u8 =
  first(a: 200, b: 2) + 100`, pinned by `run/fixedbugs-564-a-sum-past-its-width-aborts`:
  a refusal route leaves an expression with a call an abort, or reopens R3.
  The ruling behind 564 is panel 042's item 3 of its resolution (*full
  adoption*, line 286), and its **ruling 4**, overflow aborting at every
  width, the one this question meets.
- **The class is older than the round**: both compilers accept and abort a
  constant read through another (`s11`), `x: u8 = 255; y: u8 = x + 1`
  (`t03`), `9223372036854775807 + 1` (`s17`, `t04`), `-M` at `i8` (`t12`),
  division by a literal zero at `i64` (`s01`, `s02`), a shift past 63
  (`s05`, `s33`), an extern constant `INT64_MAX + 1` (`v02`); the round adds
  narrow widths over literals (the critic's item 6 lists 17 shapes).
- **What a refusal would refuse that runs today**: an unread constant, a
  function never called, a branch not taken, a `test` block (`t05` to
  `t11`); and **two semantics** to choose between: the final value (`x: u8 =
  2 - 3 + 5` is 4) or each step at the width as the emitted C computes it
  (it aborts today; `y: u8 = 300 - 100` is refused `int_out_of_range`).
- **Integers only**: spec § 11, *nothing fails to fit a float*.
- **No evaluator of a constant's body was found**: a constant is emitted as a
  C function read at each use (the critic's grep, a question for the
  compiler-engineer); panel 039's C1 row refused folding a constant
  expression as an optimisation with no measured need, design.md's
  compile-time evaluation paragraph naming three conditions to return it.
- A spec sentence is cited by section, never by line
  (`.claude/rules/spec-shape.md`); the base for the spec-warden is `measure`'s
  9889 real in this tree, 291 spendable net of the 60-token floor.
- **The ffi-pragmatist joins the sitting** for the FFI shapes (an extern
  constant in literal arithmetic, `ffi_constant_type` a refusal `build`
  alone makes). The blind seat's brief is
  `docs/panel/206-briefs/blind.md`.
