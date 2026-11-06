# Panel 234, the blind seat: the reader test

The coordinator's experiment, run as `docs/panel/234-briefs/blind/README.md`
registered it (committed at c935ff008, f550c896a, a22d08938 and b36cd92e5,
every one before the first session), and read here by the coordinator: the
seat has no agent of its own, its sessions are its evidence. It is not the
llm-ergonomist's reading of locality (`00-shared.md`, frame 6). Written
2026-11-06.

## The run

**227 sessions**: 180 first turns (36 cells of one arm, one task and one model,
five samples each) and 47 second turns, one for each first turn whose program
did not build, given its program and `heroes build`'s messages. `claude`
2.1.311, `--bare --restricted --safe-mode --strict-mcp-config`, the `Read` tool
alone, `--max-budget-usd 0.20` a session, run by
`.claude/worktrees/234-coordinator/blind234.py` (kept in the evidence). The
first session of each model ran alone and its cost was read before the others:
Haiku 0.0064 USD, Sonnet 0.0530 USD, a projection near 7 USD, so no cell was
cut. Every session exited 0 with no error. **Spent: 6.2100 USD** of the 25
registered (Haiku 0.0046 USD a session on average, Sonnet 0.0477), against the
spec-warden's registered 8 USD, band 4 to 15.

**The context**: every one of the 227 `context` sections reads `none`, and no
session called a tool but `Read` of a path inside its own folder. **One
correction of the instrument, recorded**: the grader's first check compared
each path with the folder plus a `/`, and so voided the 20 sessions whose
first call was a `Read` of their folder itself, a directory read inside the
folder the README admits; the check was corrected to admit the folder itself
and recomputed from each session's recorded calls, nothing rebuilt: **0 void**.
**And one addition before the run**, at the spec-warden's request: the README
states a case is right only at exit 0 (b36cd92e5), which the grader already
kept, and the grader also keeps the output-only reading the warden registered
its predictions under; **the two readings agree in every cell** (no program
printed the right lines and exited other than 0).

## The cells

`built@1` is a first turn that built; `right@1` built and right on all three
cases at exit 0; `right@2` right after at most one second turn; `silent` a
final program that built and answered a case wrong at exit 0. Out of 5 each.

| experiment | arm | task | model | n | built@1 | right@1 | right@2 | equal@1 | equal@2 | silent | void | first errors |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Q | thick | Q1 | claude-haiku-5-5 | 5 | 4 | 4 | 5 | 4 | 5 | 0 | 0 | needs_qualifying 1 |
| Q | thick | Q1 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| Q | thick | Q2 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| Q | thick | Q2 | claude-sonnet-5-5 | 5 | 4 | 4 | 5 | 4 | 5 | 0 | 0 | needs_qualifying 1 |
| Q | thick | Q3 | claude-haiku-5-5 | 5 | 4 | 4 | 5 | 4 | 5 | 0 | 0 | needs_qualifying 1 |
| Q | thick | Q3 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| Q | thin | Q1 | claude-haiku-5-5 | 5 | 4 | 4 | 5 | 4 | 5 | 0 | 0 | cell_redeclared 1 |
| Q | thin | Q1 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| Q | thin | Q2 | claude-haiku-5-5 | 5 | 4 | 4 | 5 | 4 | 5 | 0 | 0 | expected_args_close 1 |
| Q | thin | Q2 | claude-sonnet-5-5 | 5 | 3 | 3 | 5 | 3 | 5 | 0 | 0 | bad_operand 2 |
| Q | thin | Q3 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| Q | thin | Q3 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | bare-G | S1 | claude-haiku-5-5 | 5 | 3 | 3 | 5 | 3 | 5 | 0 | 0 | unknown_in_module 2 |
| S | bare-G | S1 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | bare-G | S2 | claude-haiku-5-5 | 5 | 1 | 1 | 5 | 1 | 5 | 0 | 0 | unknown_in_module 4 |
| S | bare-G | S2 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | bare-G | S3 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | bare-G | S3 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | bare-H | S1 | claude-haiku-5-5 | 5 | 2 | 2 | 5 | 2 | 5 | 0 | 0 | unknown_in_module 3 |
| S | bare-H | S1 | claude-sonnet-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | bare-H | S2 | claude-haiku-5-5 | 5 | 2 | 2 | 4 | 2 | 4 | 0 | 0 | expected_end_of_line 1, unknown_in_module 2 |
| S | bare-H | S2 | claude-sonnet-5-5 | 5 | 3 | 3 | 4 | 3 | 4 | 0 | 0 | needs_label 1, unknown_in_module 1 |
| S | bare-H | S3 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | bare-H | S3 | claude-sonnet-5-5 | 5 | 3 | 3 | 5 | 3 | 5 | 0 | 0 | needs_label 2 |
| S | card-G | S1 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | card-G | S1 | claude-sonnet-5-5 | 5 | 1 | 1 | 5 | 1 | 5 | 0 | 0 | needs_label 4 |
| S | card-G | S2 | claude-haiku-5-5 | 5 | 5 | 4 | 4 | 4 | 4 | 1 | 0 |  |
| S | card-G | S2 | claude-sonnet-5-5 | 5 | 2 | 2 | 5 | 2 | 5 | 0 | 0 | needs_label 3 |
| S | card-G | S3 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | card-G | S3 | claude-sonnet-5-5 | 5 | 1 | 1 | 5 | 1 | 5 | 0 | 0 | needs_label 4 |
| S | card-H | S1 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | card-H | S1 | claude-sonnet-5-5 | 5 | 2 | 2 | 5 | 2 | 5 | 0 | 0 | needs_label 3 |
| S | card-H | S2 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | card-H | S2 | claude-sonnet-5-5 | 5 | 0 | 0 | 4 | 0 | 4 | 0 | 0 | builtin_name_taken 1, needs_label 4 |
| S | card-H | S3 | claude-haiku-5-5 | 5 | 5 | 5 | 5 | 5 | 5 | 0 | 0 |  |
| S | card-H | S3 | claude-sonnet-5-5 | 5 | 0 | 0 | 5 | 0 | 5 | 0 | 0 | needs_label 5 |

spent: 6.2100 USD over 227 sessions

## What the sessions did, beyond the counts

**Whether an answer used the package at all** (`use core/...` and at least one
call into it, read from each first turn's program):

| experiment | arm | model | answers | use the package | right@1 | right@2 |
|---|---|---|---|---|---|---|
| Q | thick | Haiku | 15 | 14 | 13 | 15 |
| Q | thick | Sonnet | 15 | 15 | 14 | 15 |
| Q | thin | Haiku | 15 | 14 | 13 | 15 |
| Q | thin | Sonnet | 15 | 15 | 13 | 15 |
| S | bare-G | Haiku | 15 | 14 | 9 | 15 |
| S | bare-G | Sonnet | 15 | **4** | 15 | 15 |
| S | bare-H | Haiku | 15 | 13 | 9 | 14 |
| S | bare-H | Sonnet | 15 | **9** | 11 | 14 |
| S | card-G | Haiku | 15 | 15 | 14 | 14 |
| S | card-G | Sonnet | 15 | 15 | **4** | 15 |
| S | card-H | Haiku | 15 | 15 | 15 | 15 |
| S | card-H | Sonnet | 15 | 15 | **2** | 14 |

**Without a card, Sonnet mostly did not use the package**: 11 of its 15 bare-G
answers and 6 of its 15 bare-H answers hold no `use core/strings` and write
their own helper functions, so its high `right@1` there is the hand-written
copy the milestone exists to remove. **With a card, Sonnet used the package in
every answer, and called it as Go does**: counted over its first turns, 24 of
its 30 card answers call a two-`str` function positionally
(`strings.has_prefix(t, "#")`, `strings.count(text, needle)`), refused
`needs_label`, against 6 that label; Haiku labelled its calls in all 30 of its
card answers and in 27 of its 30 bare ones (the other 3 calling no
two-parameter function). The second turn, given the message, repaired them:
Sonnet's card arms reach 15 and 14 of 15.

**What a reader without a card reaches for that the package lacks**, the names
of the bare arms' `unknown_in_module` (each the compiler's *declares nothing
called*): Go's `trim` (its `Trim`, a cutset; the package has `trim_space`,
`trim_prefix`, `trim_suffix`) 12 times in each bare arm, Go's `split_n` (its
`SplitN`) 2 in bare-G and 1 in bare-H, and Go's `index` 4 times in bare-H, the
one name the author's rule took away that a reader wrote.

**The one silent answer of 227** (folder 178, card-G, S2, Haiku, first turn):
a logic slip the package did not touch: it calls `strings.split` and
`strings.cut` correctly, counts the lines with no `=` and never prints `bad: N`,
so cases 1 and 3 print one line too few at exit 0.

## The registered predictions, scored by their own rule

The spec-warden registered these before the first session
(`docs/panel/234-reports/spec-warden.md` § 1); each cell is a share pooled over
the two models, a hit when it lies inside its band, ends included. The table is
`.claude/worktrees/234-coordinator/score_predictions.py`'s, kept in the
evidence.

| cell | metric | registered | measured | |
|---|---|---|---|---|
| S S1 card-G | r1 | 0.6 [0.4, 0.8] | 6/10 = 0.60 | hit |
| S S1 card-G | r2 | 0.85 [0.65, 1.0] | 10/10 = 1.00 | hit |
| S S1 card-H | r1 | 0.65 [0.45, 0.85] | 7/10 = 0.70 | hit |
| S S1 card-H | r2 | 0.85 [0.65, 1.0] | 10/10 = 1.00 | hit |
| S S1 bare-G | r1 | 0.55 [0.35, 0.75] | 8/10 = 0.80 | MISS |
| S S1 bare-G | r2 | 0.8 [0.6, 1.0] | 10/10 = 1.00 | hit |
| S S1 bare-H | r1 | 0.3 [0.1, 0.5] | 7/10 = 0.70 | MISS |
| S S1 bare-H | r2 | 0.65 [0.45, 0.85] | 10/10 = 1.00 | MISS |
| S S2 card-G | r1 | 0.5 [0.3, 0.7] | 6/10 = 0.60 | hit |
| S S2 card-G | r2 | 0.75 [0.55, 0.95] | 9/10 = 0.90 | hit |
| S S2 card-H | r1 | 0.5 [0.3, 0.7] | 5/10 = 0.50 | hit |
| S S2 card-H | r2 | 0.75 [0.55, 0.95] | 9/10 = 0.90 | hit |
| S S2 bare-G | r1 | 0.3 [0.1, 0.5] | 6/10 = 0.60 | MISS |
| S S2 bare-G | r2 | 0.65 [0.45, 0.85] | 10/10 = 1.00 | MISS |
| S S2 bare-H | r1 | 0.2 [0.0, 0.4] | 5/10 = 0.50 | MISS |
| S S2 bare-H | r2 | 0.5 [0.3, 0.7] | 8/10 = 0.80 | MISS |
| S S3 card-G | r1 | 0.8 [0.6, 1.0] | 6/10 = 0.60 | hit |
| S S3 card-G | r2 | 0.9 [0.7, 1.0] | 10/10 = 1.00 | hit |
| S S3 card-H | r1 | 0.8 [0.6, 1.0] | 5/10 = 0.50 | MISS |
| S S3 card-H | r2 | 0.9 [0.7, 1.0] | 10/10 = 1.00 | hit |
| S S3 bare-G | r1 | 0.7 [0.5, 0.9] | 10/10 = 1.00 | MISS |
| S S3 bare-G | r2 | 0.9 [0.7, 1.0] | 10/10 = 1.00 | hit |
| S S3 bare-H | r1 | 0.7 [0.5, 0.9] | 8/10 = 0.80 | hit |
| S S3 bare-H | r2 | 0.9 [0.7, 1.0] | 10/10 = 1.00 | hit |
| Q Q1 thin | r1 | 0.45 [0.25, 0.65] | 9/10 = 0.90 | MISS |
| Q Q1 thin | r2 | 0.7 [0.5, 0.9] | 10/10 = 1.00 | MISS |
| Q Q1 thick | r1 | 0.4 [0.2, 0.6] | 9/10 = 0.90 | MISS |
| Q Q1 thick | r2 | 0.6 [0.4, 0.8] | 10/10 = 1.00 | MISS |
| Q Q2 thin | r1 | 0.35 [0.15, 0.55] | 7/10 = 0.70 | MISS |
| Q Q2 thin | r2 | 0.6 [0.4, 0.8] | 10/10 = 1.00 | MISS |
| Q Q2 thick | r1 | 0.55 [0.35, 0.75] | 9/10 = 0.90 | MISS |
| Q Q2 thick | r2 | 0.8 [0.6, 1.0] | 10/10 = 1.00 | hit |
| Q Q3 thin | r1 | 0.4 [0.2, 0.6] | 10/10 = 1.00 | MISS |
| Q Q3 thin | r2 | 0.6 [0.4, 0.8] | 10/10 = 1.00 | MISS |
| Q Q3 thick | r1 | 0.6 [0.4, 0.8] | 9/10 = 0.90 | MISS |
| Q Q3 thick | r2 | 0.9 [0.7, 1.0] | 10/10 = 1.00 | hit |

**Cell bands: 17 of 36 hit** (the warden expected about two in three; fewer than half, by its own rule, says the failure model is wrong, not unlucky).

| comparison | metric | registered | measured | |
|---|---|---|---|---|
| C names, card: card-H − card-G, S1 S2 | r1 | +0.03 [-0.15, 0.15] | 12/20 − 12/20 = +0.00 | hit |
| C names, card | r2 | 0.0 [-0.15, 0.15] | 19/20 − 19/20 = +0.00 | hit |
| C names, card | silent | 0.0 [-0.1, 0.1] | 0/20 − 1/20 = -0.05 | hit |
| C names, bare: bare-H − bare-G, S1 S2 | r1 | -0.18 [-0.4, -0.05] | 12/20 − 14/20 = -0.10 | hit |
| C names, bare | r2 | -0.15 [-0.35, 0.05] | 18/20 − 20/20 = -0.10 | hit |
| C names, bare | silent | 0.0 [-0.1, 0.15] | 0/20 − 0/20 = +0.00 | hit |
| C control, card: H − G on S3 | r1 | 0.0 [-0.2, 0.2] | 5/10 − 6/10 = -0.10 | hit |
| C control, bare: H − G on S3 | r1 | 0.0 [-0.2, 0.2] | 8/10 − 10/10 = -0.20 | hit |
| C control, card: H − G on S3 | r2 | 0.0 [-0.2, 0.2] | 10/10 − 10/10 = +0.00 | hit |
| C control, bare: H − G on S3 | r2 | 0.0 [-0.2, 0.2] | 10/10 − 10/10 = +0.00 | hit |
| E card under G: card-G − bare-G | r1 | +0.12 [0.0, 0.3] | 18/30 − 24/30 = -0.20 | MISS |
| E card under G | r2 | +0.05 [-0.1, 0.2] | 29/30 − 30/30 = -0.03 | hit |
| E card under H: card-H − bare-H | r1 | +0.25 [0.1, 0.45] | 17/30 − 20/30 = -0.10 | MISS |
| E card under H | r2 | +0.15 [0.0, 0.3] | 29/30 − 28/30 = +0.03 | hit |
| D thick − thin, Q1 | r1 | -0.05 [-0.3, 0.2] | 9/10 − 9/10 = +0.00 | hit |
| D thick − thin, Q1 | r2 | -0.1 [-0.35, 0.15] | 10/10 − 10/10 = +0.00 | hit |
| D thick − thin, Q2 | r1 | +0.2 [0.0, 0.45] | 9/10 − 7/10 = +0.20 | hit |
| D thick − thin, Q2 | r2 | +0.2 [0.0, 0.45] | 10/10 − 10/10 = +0.00 | hit |
| D thick − thin, Q3 | r1 | +0.2 [0.0, 0.45] | 9/10 − 10/10 = -0.10 | MISS |
| D thick − thin, Q3 | r2 | +0.3 [0.05, 0.5] | 10/10 − 10/10 = +0.00 | MISS |
| D thick − thin, Q1 to Q3 | r1 | +0.12 [0.0, 0.3] | 27/30 − 26/30 = +0.03 | hit |
| D thick − thin, Q1 to Q3 | r2 | +0.13 [0.0, 0.3] | 30/30 − 30/30 = +0.00 | hit |
| E interaction: the card worth more under H than under G | r1 | (card-H − bare-H) > (card-G − bare-G) | -0.10 against -0.20 | hit |
| silent, S card-G | silent | 0.07 [0.0, 0.2] | 1/30 = 0.03 | hit |
| silent, S card-H | silent | 0.07 [0.0, 0.2] | 0/30 = 0.00 | hit |
| silent, S bare-G | silent | 0.07 [0.0, 0.2] | 0/30 = 0.00 | hit |
| silent, S bare-H | silent | 0.07 [0.0, 0.2] | 0/30 = 0.00 | hit |
| silent, Q thin | silent | 0.1 [0.0, 0.25] | 0/30 = 0.00 | hit |
| silent, Q thick | silent | 0.03 [0.0, 0.1] | 0/30 = 0.00 | hit |
| per model: Sonnet's right@1 over the 18 cells at least Haiku's | r1 | Sonnet ≥ Haiku | Sonnet 59/90, Haiku 73/90 | MISS |

**The rest of § 1, scored by hand against the tables above**:

- *In bare-H, S1 and S2, `unknown_in_module` is the first code of at least half
  the first turns that do not build*: 6 of 8, **hit**.
- *In S2, all four arms, `builtin_name_taken` is the first code of at least 10%
  of the first turns*: 1 of 40, **miss**.
- *Q1's empty case: of Q1's final programs that build, thick answers case 2
  wrong at 0.3 [0.1, 0.5], thin at most 0.1*: every final Q1 program of both
  shapes answers case 2 right, thick 0 (**miss**) and thin 0 (**hit**).
- *Ceilings*: S3 `right@2` at its ceiling in all four arms, S1's card arms
  `right@2` and thick Q3 `right@2` on Sonnet at 5 of 5: **hit**, all three.
- *Floors*: bare-G S2 `right@1` on Haiku at 1 of 5, **hit**; bare-H S2 on Haiku
  at 2 of 5 and thin Q2 on Haiku at 4 of 5, **miss** both.
- *The cost*, about 8 USD, band 4 to 15: 6.21 USD, **hit**.

## What it says, read by the coordinator

- **D, thickness: the test cannot separate the shapes.** Both reach their
  ceiling, thick 27 and thin 26 of 30 at the first turn, 30 of 30 after it,
  and no answer of either is silent. The comparison the README registered reads
  +0.03 at the first turn and 0.00 after it: inside the band, and no difference
  a sitting can rest a ruling on. The SQLite seat's 22 mistakes, run on two
  machines (thin silent on five, thick on one), remain the measurement of the
  difference; these sessions show only that a careful reader with a card makes
  none of those five on these three tasks.
- **C, names: the author's rule costs nothing a card does not repay.** With a
  card, H and G read the same, 12 of 20 at the first turn and 19 of 20 after it
  on S1 and S2; without one, H reads 0.10 below G at both turns, inside the
  band the warden registered and as large as the control's own spread on S3
  (0.20 between two arms that differ in nothing S3 calls). The rule's cost a
  reader met was Go's `index` written against `position`, four times, every one
  a compile error.
- **E, the card: it is what makes a reader use the package.** Its effect on the
  first turn is negative, 0.20 under G and 0.10 under H, and the warden's
  registered gain is missed in both; but read beside the use of the package,
  the bare arms' advantage is Sonnet writing its own helpers instead (4 and 9
  of 15 using the package, against 15 of 15 with a card), and the card's
  first-turn cost is one rule of the language, the labels a call with two
  parameters of one type owes, broken by the reader's Go habit and repaired by
  the compiler's message at the second turn (29 of 30 in each card arm). **What
  no session tested is a card that shows a labelled call**: the cards gave each
  function as a signature, and the sitting may weigh whether the card's form
  should write each call as the language requires it.

## Corrected 2026-11-06, beneath the reading above, on the critic's second pass

Three sentences of *What it says* said more than the tables, and the critic's
G8, G9 and G10 (`docs/panel/234-reports/critic-reports.md`) measured where:

- **E: the registered condition fired, and the reading did not say so.** The
  spec-warden registered that *a card-minus-bare lift at or under 0 in both
  variants says a card buys nothing at these tasks, and the budget falls to an
  index line per package*; the pooled lifts are −0.20 under G and −0.10 under
  H, so it fired. **And the pooled figure averages two opposite effects**: on
  Haiku the card raises `right@1` by +0.33 under G and +0.40 under H, on Sonnet
  it lowers it by 0.73 and 0.60, the only differences of the run whose
  intervals exclude zero (the critic's Newcombe intervals).
- **E: the explanation was wrong.** *"The reader's Go habit"* and *"Sonnet
  writing its own helpers instead"* do not account for Sonnet's loss: without a
  card Sonnet labelled 18 of its 22 calls with two parameters of one type, its
  own helpers included, and with one 11 of 53; among the answers that call the
  package, Sonnet's are right at the first turn 9 of 13 without a card and 6 of
  30 with one. What changed is the card, written as declarations. That is why
  the reader test gained a second experiment, E2, on the card's form, registered
  before its sessions (`blind/README.md`, its last section).
- **C: *costs nothing* was wider than the run.** No bare answer of either
  variant called `cut`, `lines`, `split_once` or `split_lines`: the bare
  comparison touched the author's rule at `index` alone, four times, each a
  compile error.
- **D: the 22 mistakes measure severity, the run measured frequency.** The
  SQLite seat's planted mistakes say what each shape does with a mistake once
  made; the sessions say how often readers made them at these tasks, and no
  final Q program of either shape was `silent` (0 of 60). And the SQLite seat's
  registered prediction (2), *Q1 the only task where thick's `right@1` can fall
  below thin's*, is falsified on Q3 (9 against 10 of 10); its prediction (1),
  thick's `silent` no higher than thin's, holds; the historian's D holds and
  its C holds where it was tested.

## Experiment E2: the card's form, run 2026-11-06

Registered before its first session (`blind/README.md`, its last section, at
9a5aeef8b and e95ad4e4a; the spec-warden's predictions at 01:11 and amended at
01:19 for the corrected call card, `docs/panel/234-reports/spec-warden-e2.md`).
**74 sessions**, 60 first turns and 14 second turns, every one exiting 0, none
void, every `context` `none`; **1.972 USD**, the run's total now **8.182 USD**
of the 25 registered, over 301 sessions.

| arm | task | model | n | right@1 | right@2 | silent | labelled calls | positional calls | first errors |
|---|---|---|---|---|---|---|---|---|---|
| call-H | S1 | haiku | 5 | 5 | 5 | 0 | 11 | 0 |  |
| call-H | S1 | sonnet | 5 | 5 | 5 | 0 | 11 | 0 |  |
| call-H | S2 | haiku | 5 | 3 | 5 | 0 | 14 | 0 | missing_body 1, unknown_name 1 |
| call-H | S2 | sonnet | 5 | 4 | 5 | 0 | 9 | 0 | builtin_name_taken 1 |
| call-H | S3 | haiku | 5 | 5 | 5 | 0 | 5 | 0 |  |
| call-H | S3 | sonnet | 5 | 5 | 5 | 0 | 5 | 0 |  |
| decl-H | S1 | haiku | 5 | 5 | 5 | 0 | 11 | 0 |  |
| decl-H | S1 | sonnet | 5 | 3 | 5 | 0 | 8 | 4 | needs_label 2 |
| decl-H | S2 | haiku | 5 | 4 | 5 | 0 | 9 | 2 | needs_label 1 |
| decl-H | S2 | sonnet | 5 | 1 | 5 | 0 | 2 | 9 | needs_label 4 |
| decl-H | S3 | haiku | 5 | 5 | 5 | 0 | 5 | 0 |  |
| decl-H | S3 | sonnet | 5 | 1 | 5 | 0 | 1 | 4 | needs_label 4 |

haiku r1: call-H 13/15 decl-H 14/15 effect -0.07
haiku r2: call-H 15/15 decl-H 15/15 effect +0.00
haiku silent: call-H 0/15 decl-H 0/15 effect +0.00
sonnet r1: call-H 14/15 decl-H 5/15 effect +0.60
sonnet r2: call-H 15/15 decl-H 15/15 effect +0.00
sonnet silent: call-H 0/15 decl-H 0/15 effect +0.00
pooled r1: call-H 27/30 decl-H 19/30 effect +0.27
pooled r2: call-H 30/30 decl-H 30/30 effect +0.00
pooled silent: call-H 0/30 decl-H 0/30 effect +0.00
labelled share, haiku decl-H: 25/27 = 0.93
labelled share, haiku call-H: 30/30 = 1.00
labelled share, sonnet decl-H: 11/28 = 0.39
labelled share, sonnet call-H: 25/25 = 1.00

The package was used in 14 of 15 Haiku and 15 of 15 Sonnet answers with the
call card, 15 of 15 each with the declaration card.

**Scored by the spec-warden's registered rule** (per model, out of 5, band ends
included):

- **Cells: 23 of 24 hit.** `right@1` 11 of 12, the one miss decl-H S1 on
  Sonnet (3 of 5 against 0.2 [0.0, 0.4]); `right@2` 12 of 12.
- **Effects, call-H minus decl-H: 7 of 7 hit.** `right@1` Sonnet **+0.60**
  (registered +0.56, [+0.3, +1.0]), Haiku −0.07 ([−0.2, +0.2]), pooled +0.27
  (+0.28, [+0.1, +0.5]); `right@2` 0.00 for each model and pooled; `silent`
  0.00 pooled.
- **The share of two-parameter calls labelled: 4 of 4 hit.** Haiku decl-H 25 of
  27 (0.93, [0.9, 1.0]), call-H 30 of 30; Sonnet decl-H 11 of 28 (0.39, [0.0,
  0.45]), call-H 25 of 25 (1.00, [0.7, 1.0]).
- **The check of the cause: hit.** Sonnet's answers whose every two-parameter
  call is labelled, both arms together, are right at the first turn 19 of 20
  (0.95, [0.8, 1.0]).
- **What would have said the form is not the cause** (call-H Sonnet labelling
  fewer than half its calls, or decl-H not losing again) did not occur.

**What it says, read by the coordinator**: the card's form decided Sonnet's
first turn. Written as declarations, the same card on the same package left
Sonnet labelling 11 of 28 calls and right 5 of 15; written with each function's
call as a program writes it, 25 of 25 and 14 of 15, Haiku unchanged, and both
arms reaching 30 of 30 after a second turn. The call card measured 805 vendored
tokens against the declaration card's 585. One limit the spec-warden named
stands: the call card keeps the declaration and adds the call beside it, so
this run says nothing of a card without the declaration.
