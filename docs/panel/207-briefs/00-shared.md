# Panel 207, the shared brief: a constant's body whose step aborts only on a loop's later turn (580)

Written by the coordinator on 2026-10-10 from 11:35 (`date`), on the tree
frozen at `259de604` (worktree `lane-panel-207`: batch 18's round with lanes
b18-close, b18-infer and b18-ffi's committed repairs, panel 206's landing
among them; not yet the trunk). Convened under the author's goal of about
02:30, *at most five open, all improvement*, and panel 206's ratification
(by 10:22), whose landing filed this. Every fact names its command or file;
a number marked **carried** is a report's.

**Lane: soundness, with the spec-warden and the historian, no blind seat.**
The question is what `check` refuses in a constant's body and whether spec
§ 4's new sentence states it; the readers' expectation of a constant
computed when the program is compiled was measured at panel 206 (2 of 2,
`docs/panel/206-reports/llm-ergonomist-scoring.md`), and no new reader-facing
form is proposed, so the blind seat is not convened; the completeness critic
reads the brief before the seats and the reports after them.

## The question

Panel 206 R1 (ratified) made a step of a constant's written body that cannot
be computed a compile error, read or not, and spec § 4 now reads *a written
body computes over literals and other constants, and a step of it that aborts
is a compile error* (C4b, landed in lane b18-close's `eb5fef1b`, 9,944 real by
its `--refresh`). Its walk (`selfhost/check/const_steps.hero` and
`const_values.hero` in this tree) computes each step at the width the checker
recorded, a `=` binding holding its value and an `@` cell its last value on
the straight path, **forgotten after any branch or loop that writes it**.
Run by the coordinator at 11:35 with the round's compiler (built from this
tree's `selfhost/`), the cases under `.claude/worktrees/scratch-b15/p207/`,
read only:

| program | build | run |
|---|---|---|
| `loop.hero`: `constant V: u8` whose body runs `while i < 3` doing `v @ v + 100` | 0 | aborts 134, *panic: integer overflow* |
| `loop2.hero`: the same, `while i < 2` | 0 | prints 200 |
| `flat.hero`: `constant V: u8` of `200 + 100` | refused `int_out_of_range`, *`200 + 100` computes 300, which does not fit a `u8`* | |

So C4b is false on `loop.hero` (CLAUDE.md § 12: the compiler has the bug, or
the sentence does). A constant's body holds no call (panel 039's
`selfhost/resolve/constant_body.hero`: *may only read literals and other
constants*), so its computation is closed: no input, no call. Panel 039's C1
row (`docs/panel/039-*.md:324`) refused *fold a constant expression ... no
evaluator* as *an optimisation with no measured need*; design.md's
compile-time evaluation paragraph (grep *Compile-time evaluation*) says the
same of features. **Should the checker evaluate a constant's body exactly,
with a bound on its steps, so every step that aborts is refused; or should
C4b's sentence say what the walk decides; and what happens at the bound** (a
refusal *cannot be computed within N steps*, or the run-time abort kept for
what the bound leaves)? Widen the list.

## The rules every seat works under

As panel 206's (`docs/panel/206-briefs/00-shared.md` on the trunk, § The
rules every seat works under): your folder
`.claude/worktrees/scratch-b15/207-<seat>/`, a copy of
`.claude/worktrees/lane-panel-207/` rsynced there excluding
`.claude/worktrees`, its `.git` removed at once and no git run in it; your
compiler from the seed then `./heroes build selfhost/main.hero -o heroes`;
lanes b18-ffi, b18-guard and b18-close work beside you: never touch them; at
most three processes; never kill a process you did not start; no paid run;
no file outside the repository's root, `/tmp` included, not for a moment.
**Time box**: report within 35 minutes; notes in your folder as you go.

## Corrections and additions from the critic's first pass, binding

Read at 11:46 (`date`): `docs/panel/207-reports/completeness-critic-pass1.md`,
every one from a command it ran, the coordinator re-reading design.md
`:3160-3170` at 11:46; applied before any seat starts and binding over the
text above where they disagree. Read it whole; its routes A to H and its
questions are part of your brief. In short:
- **C4b is false on straight-line bodies too**, not only on loops: an array
  index out of range (`xs[5]`, `xs[0 - 1]`, a write `xs[7] @ 4`), a string
  index, a failing `assert`, a nan in `<`, all building and aborting 134
  (the walk judges arithmetic only, `const_steps.hero:114`, `:181-183`,
  `:205`); a computed `if` or `match` branch; inside a loop every cell is
  forgotten, written or not (`s31`); a group's constant (`INT64_MAX + 1`,
  defect 579) is an input from C, so a body is not closed.
- **design.md `:3165-3170` rules on the brief's lead route**: compile-time
  evaluation returns only on three conditions jointly, among them *a
  structural termination argument ..., never a quota* (Rust removed its
  `const_eval_limit` in 1.72); panel 039's agreement 3 (`:81-90`, five
  seats): *A quota is the wrong answer to termination*; its C2 row (*run a
  function at compile time*, `:325`) is the row a body with loops may fall
  under, beside C1. A step bound is a quota.
- **R1 refuses steps in dead code** (`while false`, `if false`, pinned by the
  577 goldens): an evaluator that replaces the walk reverses that ratified
  clause; one beside it keeps it.
- The forms a body admits: no record construction (a call), but variant
  cases with payloads, `assert`, `return`, `break`, `continue`, `f"..."`
  holes, floats, chars, index and field reads; an array cannot grow (`+` and
  `.push` refused), so every `for` is bounded by the source and only `while`
  is unbounded; `while true` with no `break` is refused `no_value`, a computed
  endless loop is not (`s24` runs to the timeout).
- **The census**: 1,063 written constants in 3,348 tracked files; 0 of
  `selfhost/`'s 333 and 0 of `examples/`'s 188 have a loop or a branch, 27
  test goldens do, every one about constants.
- **The lane**: the bound-refusal route and any C4b rewording are
  reader-facing; the ffi-pragmatist joins for group constants and route C
  (run the emitted accessors at build time, exact, the shape of panel 206's
  probe); the blind seat is not convened (no new form proposed by the brief;
  panel 206's readers measured arithmetic only, said here).
- The base for the spec-warden: `heroes measure` reads 9,944 real, 7,565
  cl100k, 7,430 legacy; 236 spendable net of the FFI floor. The round's head
  has moved to `be2da1be` (lane b18-ffi merged), touching no file under
  `selfhost/check`, `selfhost/resolve` or `spec/`.
