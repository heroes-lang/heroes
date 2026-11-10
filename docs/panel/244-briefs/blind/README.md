# Panel 244's blind seat: whether the specification states `internal`

Written 2026-11-11 by the coordinator after the author's yes (*Sì, fino a 3 USD*, the question
form, 2026-11-11 at about 00:40), at the sitting's R1: *`internal` (I2, +35 / +37) enters only if
the blind reading pays for it*. Registered here before any session runs.

## What it measures

Whether a reader holding only the specification predicts the `internal` rule's refusal, and uses
the rule to keep a module private, with the sentence I2 and without it. I2, priced by the sitting's
critic in `p244-scratch/critic/second/price/set-S1c2_I2.md`, adds to § 1 after the catalogue's
sentence: *"A path with `internal` as a part after its first is named only by the module the path
before the last such part names and by the modules under it."*

Two arms, label-stripped in the session's folder as `x` and `y`:

- **x** (arm A): the specification as R1 lands it without I2, the critic's
  `set-S1c2_d03_C2_d08d.md`;
- **y** (arm B): the same with I2, the critic's `set-S1c2_I2.md`.

Two tasks:

- **t1, predict and repair** (`task-t1.md`): the program of
  `tests/golden/check/use-a-private-module-from-outside-its-owner.hero`, its paths shortened to
  `geom/`, which the compiler refuses `private_module`; the reader says whether it builds and, if
  not, repairs it so it prints 6;
- **t2, write** (`task-t2.md`): a module `money` whose helper no other module of the program may
  use, a `main.hero` printing 300, and a `probe.hero` naming the helper that the compiler must
  refuse.

## Grading, registered before any session

Each answer's files are copied into a fresh folder and built with batch 30's round compiler
(`.claude/worktrees/b30-fix/heroes-next`, the landing round's):

- **t1 right**: the report predicts that the program as written is refused, and its repair builds at
  exit 0 and prints `6`; **wrong** otherwise;
- **t2 right**: `main.hero` builds at exit 0 and prints `300`, and `probe.hero` is refused at exit 1;
  **wrong** otherwise.

**The decision rule**: I2 enters the specification if arm y gives at least two more `right` answers
than arm x over its four sessions; otherwise the compiler's message carries the rule, as defect 2439
made it state it, and I2 waits.

## The run, and its size

Two sessions per task and arm, **8 sessions**, each in a folder of its own under
`.claude/worktrees/p244-scratch/blind244/<task>-<arm>-<n>/` (inside the repository's root and
ignored by git, as panel 239's were), holding `brief.md`, `heroes-spec.md` (the arm's
specification) and `task.md`; run with git blind (`GIT_CEILING_DIRECTORIES` set to the folder's
parent) and the key loaded from the trunk's `.env`, never printed, by the `/panel` skill's command
with `--max-budget-usd 0.36`. **Bound: 0.36 USD a session, 2.88 USD for the eight**, under the
author's 3 USD; the runner stops once the reported sum passes 3.00. The model: the one the CLI
runs, said in the report.
