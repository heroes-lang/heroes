# 051: the variant contexts' pilot on the assistant's tasks, level within one sample at turn three, and five predictions scored without a held-out task, panel 217's ten readers ten of ten

Measured 2026-10-24 by the session holding M-thesis-harness, its step 13,
in the lane `thesis` from `b3defeb49`, the compiler of sources
`616f959062622f6a` (the held-out run's pre-registered one). One folder of
`tests/harness/thesis.hero`'s runs, `build/thesis-runs/00fb571c-variants`:
the assistant's ten tasks under three contexts at once, `spec` and the two
variants of `tests/thesis/contexts/` (issues 1139 and 1005), Haiku 5.5 and
Sonnet 5.5, three arms, three samples, five turns, on the author's yes to a
cap of $10 (2026-10-24). **It is the pilot of the scoring run, not that run**:
panel 226's R6 asks the variants at the held-out run on ten of the held-out
tasks, which is the author's, in a shell no model reads; this run says what the
three contexts cost and take, and its rows enter no score
(`tests/thesis/contexts/README.md`). Then the predictions this milestone's close scores
without a held-out task: panel 226's llm-ergonomist and ffi-pragmatist,
panel 227's P4 and panel 007's spec-warden on the trial's answers. Every
number is a command's, run that day. **The ten tasks are the assistant's and
enter no published number** (`docs/metrics/tasks.md` § Two suites).

## 1. What ran, and what answered

| invocation | from | requests, by the tally printed | generations | verdicts | lost | stopped by |
|---|---|---|---|---|---|---|
| first, 04:11 to 05:44, exit 2 | `b3defeb49` | none printed | 166 | 498 | not known | the disk: `sleep 5` could not be started, the operating system's reason 28, `ENOSPC` |
| resumed, 12:28 to 12:38, exit 0 | `f305307aa` | 23 | 14 | 42 | 0 | the end of the plan |

**The disk filled while the first invocation ran** (another project's build
directories, 137 GB, cleared by its own session on the author's word), and the
run stopped printing no tally: its requests are counted by the answers the folder
kept, and a request in flight at that moment, which may have been paid, by
nothing. `report` read every verdict of the folder at exit 0, so no file was left
half written. The resumed invocation asked only what was missing, its provenance
held to the folder's field by field (`resumed.txt` names the day and the
commit). **The whole folder spent $4.48**, the instrument's own sum of the
answers it holds, against the $10 cap. 180 generations, 540 verdicts, 0
answers from another model.

## 2. The specification without its productions (issue 1139, panel 133's third prediction)

Green at turn 1 and turn 3 in the strict arm, of 30 samples a cell (the brief
and control arms in `report.md` beside it):

| model | `spec` g1 / g3 | without productions g1 / g3 | with one worked diagnostic g1 / g3 |
|---|---|---|---|
| Haiku 5.5 | 21 / 30 | 22 / 29 | 21 / 29 |
| Sonnet 5.5 | 24 / 30 | 21 / 29 | 21 / 30 |

Without the productions each model lost one sample of 30 at turn 3, in every
arm alike, a program stopped loudly (`abort`) and no silent one: **on the pilot,
panel 133's third prediction reads as it says**, the grammar worth at most one
sample in thirty and the silent delta 0 (`spec` 0, without productions 0). It is
scored on the held-out ten, never here.

## 3. The specification with one worked diagnostic (issue 1005)

With the worked diagnostic, Haiku 5.5 read 21 and 29, one silent failure
in every arm alike, and Sonnet 5.5 21 and 30, three samples fewer at turn 1 than
under `spec`. **The pilot shows no gain** from the one diagnostic; issue 1005's
number is the held-out ten's.

**And one thing the pilot shows that it was not asked.** Under `spec`, on the
same ten tasks and the same two models, the strict arm's turn-1 codes moved
between measurement 050's run (the specification `21226405`, before panel 227's
sentence) and this one (`4d8e5251`, after it): `wrong_label` in 9 samples of 60
then and 0 now, `builtin_shape` 9 and 0, `type_mismatch` 8 and 0, the three
codes panel 227 found on `slice` and on `repeat`'s count; and green at turn 1
read Haiku 5.5 16 and Sonnet 5.5 18 then, 21 and 24 now. Another day and other
samples, so a sign and not a score: panel 227's P1 and P2 are the held-out
run's.

## 4. Panel 226's llm-ergonomist, at the trial

*First attempts that compile and that pass the hidden cases, 45/30, 50/30,
70/60 per cent for tasks 1 to 3; the three rules broken most, in order;
repair 1 identical under the brief message and repair 2 different*
(`docs/panel/226-metrics-2-and-4-measured-by-one-instrument-three-arms-forked-from-one-generation-the-control-arm-built-and-run-and-the-held-out-tasks-sealed-in-the-tree.md`, its predictions). Its three tasks are the trial's
tasks 01, 02 and 03 (word counts, a ledger's balances, bracket depth), the
seat's blind folder holding the same three before their prose was pinned.
Read from `00fb571c-v3`'s `verdicts.jsonl`, the strict arm at turn 1, the
three models pooled since the prediction names none, nine samples a task;
*compiles* is every outcome but `compile_error`, `no_program` and
`format_slip`, *passes* is `cases_pass`; the interval Wilson's at 95%, the
instrument's formula:

| task | predicted compiles / passes | measured compiles | measured passes | interval | verdict |
|---|---|---|---|---|---|
| 01 | 45% / 30% | 2 of 9 | 2 of 9 | 6% to 55% | both inside |
| 02 | 50% / 30% | 8 of 9 | 8 of 9 | 56% to 98% | **both outside, above** |
| 03 | 70% / 60% | 8 of 9 | 8 of 9 | 56% to 98% | both inside |

**Falsified on task 02**: the models did far better than the seat expected
on the ledger. The rules: on the three tasks the turn-1 codes were
`type_mismatch` in 5 samples and `bad_operand` in 2 (task 01, integer
conversions), `builtin_shape`, `wrong_label` and `expected_arm_arrow` in 1
each; so the first rule predicted, conversions giving a `T?`, is first, the
second, unused bindings and an unbound value, is met **never**, and the third,
mandatory labels, once. **The two repairs are `lapsed`**: they are two
mistakes the seat made by hand, which no answer of the trial holds.

## 5. Panel 227's P4, at its landing

*R3 lands at +75 ± 10 `code_lines` with `check/walk.hero` at or under its row
and no `DECIDED` row raised* (`docs/panel/227-the-built-ins-sentence-shows-what-a-call-takes-and-a-wrong-count-is-told-one-way-whatever-the-callee.md`). R3 landed at
`872dbcaf8` over `7d92b2619` with R4, R5 and defect 2185's repair, gated as one
tree. Counted with `layout`'s own rule (`.claude/hooks/ceiling.py`'s
`code_lines`, the suite's declared mirror) on the six files of `selfhost/` the
commit moved: `check/builtin_args.hero` +63 (a new module), `check/mismatch.hero`
+13, `check/waiting.hero` +8, `check/walk.hero` +1 (1869 to 1870),
`check/width_class.hero` 0, `inventory.hero` +19: **+104 for the four landed
together**. `check/walk.hero` reads 1870 against its row of 1870, and
`tests/harness/suite_layout.hero` is unchanged between the two commits, no
`DECIDED` row raised. **The two conditions held; the number is `lapsed`**, R3's
own share not separable from a commit that carried four changes.

## 6. Panel 226's ffi-pragmatist, at the close, on this Mac

*With the key exported in the grader's process, a C program calling
`sysctl(KERN_PROCARGS2)` on the grader's pid inside `thesis_sandbox.hero`'s
profile prints the key's value; with the archive opened by a separate
invocation, the same program prints `hits=0`.* Run with the seat's own probe,
`procargs.c` and `sleeper.c` from `docs/panel/226-reports/evidence.tar.xz`,
built with clang that day, the profile `thesis_sandbox.hero`'s `profile` writes,
a stand-in process carrying a made-up value in place of the key:

```
with the value in its environment, inside the profile:  procargs bytes=4916 hits=1
without it, inside the profile:                         procargs bytes=4884 hits=0
with the value, outside any sandbox:                    procargs bytes=4916 hits=1
```

**Held, both halves**: the profile does not hide a same-user process's
environment, which is why no process holding a key runs a model's program.
That rule was asked too: `grade` of task 01's reference with
`HEROES_TASKS_KEY` set to a made-up value exits 2 naming the variable and the
route (`unseal`, then grade where the key is not set), and the same command
without it reads `cases_pass 9/9`.

## 7. Panel 007's spec-warden, on the trial's answers

*Zero baseline completions contain a depth-0 illegal break*
(`docs/panel/007-terminator-enders.md`; issue 4). A depth-0 break is told
`continuation_outside_brackets` or `line_end_before_continuation`, at the
first stage `check` speaks, so a completion holding one cannot hide it behind
another refusal. Turn 1, strict arm: **0 of 90** under `spec` (`00fb571c-v3`),
**0 of 90** through `claude -p` with the specification (`00fb571c-v4-claude`),
**1 of 90** with no specification (`00fb571c-v3-none`). In the pilot, **0 of 180**, 0 of 60 in each context, without the productions included; the held-out run's count is measurement 052's.

The ergonomist's half of panel 007, *tasks forcing a depth-0 break lose
≥25pp first-try under a spec that says nothing*, is `lapsed`, as panel 226's
R13 decided: the specification has stated the continuation rule since panel
095, so no run can ask a spec that says nothing about it.

## 8. Panel 217's ten blind readers, the payment of ledger row 7846

*Asked what `keptpt_lent` prints, at least 8 of 10 answer `11`, and asked
whether `read(p, bump(@p))` builds with `read(a: Pt lent, b: i64)`, at least 8 of
10 answer that it is refused* (`docs/measurements/010-spec-budget-ledger.md`,
row 7846), scored by the sitting's own launcher
(`docs/panel/217-reports/llm-ergonomist-work/armR2/run_one.sh`), on the author's
yes to a cap of $3 (2026-10-24). The compiler answered first, with the lane's
stage 2: `keptpt_lent`, the compiler-engineer's program from
`docs/panel/217-reports/evidence.tar.xz` with its `pt.h`, runs at exit 0 and
prints `11` then `6`; the second program, written with the prediction's names
(`bump(@p: Pt) -> i64` raising `p.x`, `read(a: Pt lent, b: i64)`, `print(read(p,
bump(@p)))`), is refused at exit 1 with `aliased_mutable_arguments`.

Ten folders, each a `brief.md` asking for the answers in a fixed shape, a
`task.md` holding the two programs and the header, and the specification at
`4d8e5251`; ten `claude -p` sessions with the launcher's flags, `claude-opus-5-5`,
`--bare --restricted`, Claude Code 2.1.308, five at a time. **Two departures from
the launcher, both said here**: `--max-budget-usd 0.3` a reader where it reads
`0.7`, so that ten could not pass the author's $3; and the answers came back in
each session's final text, `run.json`'s `result`, since the sessions reported
holding no tool to write `answers.md` with.

| reader | Q1, the output | Q2 |
|---|---|---|
| each of the ten | `11 / 6` | refused, the `@p` nested in an argument reaching the `lent` place |

**Held, ten of ten on each question**, against at least eight. $0.94 for the
ten, by `run.json`'s `total_cost_usd`. Every session's `modelUsage` names
`claude-haiku-5-5` beside `claude-opus-5-5`, the session's own side request that
defect 2179 found, the answer being the pinned model's. The row's clause is paid.

