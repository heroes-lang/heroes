# 040: thirty first answers, and none left the mistake the compiler did not tell

Measured 2026-10-07 from 14:30 to 15:48 by lane b14-m212 of batch 14, for
defect 212 (*class (b), a mistake told only once another is fixed, has no
measurement at the project's sizes*). Two paid runs, both named in the lane's
brief and approved by the author at about 14:25 that day (the question put to
them: *the spec-warden's run of about 10 sessions and 4 to 5 USD, and the
300-line form*; their answer: *yes, both parts*): **part 1**, panel 187's
spec-warden's prediction 2 (`docs/panel/187-reports/spec-warden.md` § 8),
capped at 6 USD; **part 2**, the 300-line form panel 183's completeness critic
left owed (`docs/panel/183-reports/completeness-critic.md` § 6, *the same
script over the (b)-class mutants of files of 300 lines and more*), capped at
20 USD. What the two decide is whether panel 187's D1 stronger promise enters
(design.md §4.17's last sentence: *where it measures a hidden mistake of the
parse stage costing one, telling that mistake in the same run joins the
promise*); that decision is a sitting's, and § 6 says only what the numbers
show for it.

`<scratchpad>` below is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`,
and `<m212>` is `<scratchpad>/batch14/m212`, where every session's folder,
`run.json`, answer and report stays until the scratchpad is emptied.

## 1. Provenance

| what | value |
|---|---|
| today's compiler | built from `affbd872`'s seed (`git rev-parse affbd872:seed/heroes.c` `466b3102...`) by `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in the lane's worktree; binary sha1 `34d1b3bc763c22c3` |
| the sitting's compiler | `07ccb72a`, the commit panel 187's seats sat on, its `seed` and `runtime` taken by `git archive 07ccb72a seed runtime` into `<m212>/sitting-07ccb72a/` and built the same way; binary sha1 `3924b00d105dbaf7` |
| the specification each session read | `spec/heroes-spec.md` at `affbd872`, 456 lines, copied as `spec.md` (sha1 `8f118a9af193b5f4`) |
| the command | `claude` 2.1.285, `claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." --restricted --safe-mode --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd <B> --output-format json > run.json 2> run.err`, run under `timeout 900` from each session's own folder (`.claude/skills/panel/SKILL.md`, the `llm-ergonomist` bullet) |
| the folders | one per session, holding `spec.md`, `brief.md` and (part 2) `p.hero`; outside any git tree (`git rev-parse` fails there) and with no `CLAUDE.md` in the folder or any ancestor (each ancestor listed) |
| the model that answered | **`claude-opus-5-5`** in all 30 sessions (each `run.json`'s `modelUsage`); each session's `modelUsage` also lists one `claude-haiku-4-5-20251001` call, 0.000984 to 0.001009 USD, the CLI's own |
| what reached the sessions | each `report.md`'s `context` heading: the folder's files and the CLI's environment note (working directory, platform, date, an account email); no session names a project rule, contract or memory. One part 2 session (`selfhost_measure_bpe-s1`) says a read of a mistyped path outside its folder was refused and returned nothing |

No session failed, none was cut by its budget, and none was run twice. One
launch of part 2's sessions at 15:11 ran none: the shell kept the folder list
as one word, every `cd` failed before `claude` was called, and no `run.json`
was written (its logs are kept as `<m212>/p2/stream?-failed-zsh-split.log`);
the relaunch at 15:12 is the run.

## 2. What moved before anything was paid: both rows of prediction 2 are told today

The two rows were in `<scratchpad>/audit-130-133/cases/` and
`<scratchpad>/lane-recovery-b8/p1/`, which no longer exist; `git log --all
-S f5_do_while` and `-S r12_function_moved_to_top` find one commit each, panel
187's own `5784b1e0`, whose text names them. **131-32** is
rebuilt verbatim from `docs/panel/187-briefs/00-facts.md` (its case
`f5_do_while.hero`, printed whole there); **`g2/r12`** from the same file's
description (*`variant V` over `    red` and `    function f()` over
`        print(1 +)`*) with `function main()` over `    print(1)` below, the
shape of its pin `tests/golden/check/panel-187-a-function-among-a-variants-cases-is-dropped.hero`:

```
function main() {
    do {
        y = 3 4
    } while (1 == 1)
}
```

```
variant V
    red
    function f()
        print(1 +)

function main()
    print(1)
```

**The sitting's compiler reproduces every position the record gives**
(`check`, and `check --brief` on the record's follow-ups): 131-32 gives
`missing_body@1:17`, `expected_end_of_line@2:8` and `@3:15`, line 4 untold;
`g2/r12` gives `expected_case@3:5` alone, its words the record's (*a variant
case is a name on its own line, with its fields indented below it*); with the
function moved to the top level, `expected_expression@5:14`; 131-32 without
its braces, `unexpected_block@3:1` and `missing_body@5:1`, the record's two.

**Today's compiler tells both hidden mistakes.** `g2/r12`: `expected_case@3:5`
and `expected_expression@4:18` on `print(1 +)` (defect 200, repaired
2026-10-04). 131-32: `missing_body@1:17`, `@3:15`, and at `2:8` *expected the
end of the line, found `{` — C's `do { … } while (c)`, which runs its body
before asking `c`, is no loop here: write the body under `while true`, ending
it with `if !(c)` over `break`* (defect 201, repaired 2026-10-04). So on the
compiler the brief named, neither row has a mistake on a line no message
shows, and a reading of today's output could not score a prediction about a
hidden one. **Part 1 was therefore run on the sitting's output**, the output
prediction 2 was made on; today's output on each is in `<m212>/p1/src/*/today.txt`.
That is a deviation from the brief, taken because the alternative measured
nothing the prediction names, and the coordinator is told it.

**The frozen plan on today's compiler.** Panel 187's R2 plan (round10's,
13,594 singles and 16,041 pairs over the corpus at `c85bccb8`) replayed by
today's compiler, `python3.14 -I <scratchpad>/inst-r2/replay.py --compiler
<m212>/heroes-today --tree <m212>/tree --plan <scratchpad>/inst-187/round10
--out <m212>/r2-today --jobs 3` over a clone of the frozen tree, 14:49 to
15:48, exit 0, every check exiting 0 or 1. A pair counts in an arm where its
second mistake, alone, draws a code on its own lines (the instrument's rule).
In the normal arm, **seconds of the parse stage hidden: 1 of 14,776**, the
pair `arm-comma` then `arm-comma` in `tests/golden/run/fallible.hero` (38
lines), **and 0 of 4,957 in files of 300 lines and more**; seconds of a later
stage hidden: 1,068 of 1,158, and 369 of 397 at 300 lines and more, which
design.md §4.17 calls waiting *by design*. The control arm: parse stage 5 of
14,556 (1 of 4,881 at 300 and more), later 884 of 971. Against batch 13's
replay of the same plan (`<scratchpad>/inst-187/r2-b13-round`, 11:19 that
day), no pair's told status moved in either arm.

## 3. Part 1: the spec-warden's prediction 2

Ten sessions, five per row, `--max-budget-usd 0.6` each, from 14:52 to 14:57.
The brief (`<m212>/p1/src/*/brief.md`): the folder's `spec.md`, the program and
the sitting's `check` output inline, and the task *make this program compile
and do what it evidently means*, written in ONE turn to `c.hero`, then a
`report.md` with three headings, `reading`, `confidence` and `context`. It
names no project. **Scored** on each first answer, `c.hero`: does it repair or
remove the hidden mistake (the do-while's tail; the operand left out of
`1 +`), read in its text, and does it check clean (`check --brief`, today's
compiler and the sitting's).

| session | USD | `c.hero` | checks clean, today / sitting | hidden mistake |
|---|---|---|---|---|
| 131-32 s1 | 0.171161 | `while 1 == 1` over `y = 34`, `_ = y` | yes / yes | repaired |
| 131-32 s2 | 0.163320 | `while true` over `y = 34`, `_ = y` | yes / yes | repaired |
| 131-32 s3 | 0.165156 | the same as s2 | yes / yes | repaired |
| 131-32 s4 | 0.169602 | the same as s2 | yes / yes | repaired |
| 131-32 s5 | 0.167448 | the same as s2 | yes / yes | repaired |
| `g2/r12` s1 | 0.149424 | `f` moved to the top level, `print(1 + 1)` | yes / yes | repaired |
| `g2/r12` s2 | 0.152643 | `f` moved to the top level, `print(1)` | yes / yes | removed |
| `g2/r12` s3 | 0.156280 | as s1 | yes / yes | repaired |
| `g2/r12` s4 | 0.153030 | as s1 | yes / yes | repaired |
| `g2/r12` s5 | 0.153598 | as s1 | yes / yes | repaired |

**10 of 10 first answers repair or remove the hidden mistake, and 10 of 10
check clean: prediction 2 holds** (it asked for 8 or more, and is falsified at
3 or more leaving it). Every 131-32 answer reads `3 4` as `34`, a guess at the
told mistake that the prediction does not score. **Part 1 cost 1.601662 USD**,
the sum of the ten `total_cost_usd`.

## 4. Part 2: the 300-line form

**The selection.** The plan is round10's (`<scratchpad>/inst-187/round10/plan-*.jsonl`,
lane recovery-b6's gate plan over the corpus at `c85bccb8`), replayed by
`<scratchpad>/inst-r2/replay.py`. A class (b) row is a pair whose second
mistake, alone, is told on its own lines in the normal arm, and in the pair is
told on none of them (the replay's `hidden` with a non-empty `alone` set). In
files of 300 lines and more, batch 13's replay of the plan
(`<scratchpad>/inst-187/r2-b13-round`, 2026-10-07 11:19) holds **369 such
pairs, every one of a later stage** (the resolver's or the checker's message,
waiting behind a parse message by design), **and none of the parse stage**;
51 are in files with no `use` line, which can be checked alone as panel 183's
critic required, and those 51 are in exactly five files. In each, the pair
whose two sites stand farthest apart (ties to the smaller key). Each pair was
then re-run on today's compiler before any session: the original checks clean,
the mutant draws ONE message on the first mistake, and the second alone draws
its own code (`<m212>/p2/alone/`). Today's replay, read after the sessions,
selects the same 369, 51 and five, and the same pair in each file
(`<m212>/p2/select-b13.txt` and `select-today.txt`, made by
`<m212>/tools/select2.py`).

| file, at `affbd872` (blob unchanged since `c85bccb8`) | lines | first mistake, told | second mistake, hidden; alone told |
|---|---|---|---|
| `examples/calculator/whole.hero` | 433 | line 43, `constant ERR_UNCLOSED_PAREN: str = "unclosed_paren"` (`constant-inline`): `missing_body@43:34` | line 401, `.product _  => False` (`python-bool`): `unknown_name` |
| `examples/ledger/db/sqlite.hero` | 404 | line 145, `text_cell(value: str)` (`case-parens`): `expected_end_of_line@145:14` | line 341, `value: double` (`double-type`): `unknown_type` |
| `examples/nbody/main.hero` | 405 | line 151, `jupiter() saturn()` (`missing-comma`): `expected_separator@151:30` | line 400, `-> boolean` (`boolean-type`): `unknown_type` |
| `selfhost/measure/bpe.hero` | 315 | line 35, `class Ranks` (`class`): `reserved_word@35:1` | line 259, `r.of["IQ=="] ?? -1 == 0` (`nullish`): `try_in_infallible`, `not_fallible` |
| `tests/harness/strings.hero` | 361 | line 65, `if at >=` (`missing-operand`): `expected_expression@65:13` | line 149, `hit @ False` (`python-bool`): `unknown_name` |

Line numbers are the original's; a session's input was the mutant as
`p.hero` and today's `check p.hero` on it, one message each, in its brief
(`<m212>/p2/src/*/today.txt`). **The task and the brief are part 1's**, the
program in a file rather than inline. **The scoring is panel 183's critic's**:
does `c.hero` check clean (`check --brief`, today's compiler); does it restore
both damaged lines (each original line the two mutations replaced stands again
byte for byte, aligned against the original by `difflib`); does it give back
the original byte for byte; and every other change, read by hand for a silent
wrong edit (a change of what a correct line means).

**The price, first**: one session on the largest file in bytes,
`examples/ledger/db/sqlite.hero` (21,940 bytes), at `--max-budget-usd 2`, read
**0.594018 USD**; twenty at most that is 11.9, under the 20 cap, so the other
nineteen ran at `--max-budget-usd 1.5`, in two streams, each stopping before
a session if the part's spend plus 1.5 passed 18.5.

| file | session | USD | checks clean | both lines restored (the hidden one included) | byte for byte | other changes, read by hand |
|---|---|---|---|---|---|---|
| calculator | s1 | 0.352279 | yes | yes | yes | |
| calculator | s2 | 0.363312 | yes | yes | no | moved the misplaced doc comment of `apply` and its heading (lines 27 to 31) above `apply`; the `.num _ => []` arm returns a `none: [str] = []` bound before the `match`: meaning kept |
| calculator | s3 | 0.361011 | yes | yes | yes | |
| calculator | s4 | 0.365777 | yes | yes | no | the same comment move, nothing else |
| sqlite | s1 | 0.594018 | yes | yes | no | appended a `main` and a `print_row` (29 lines) using the binding: an addition |
| sqlite | s2 | 0.501062 | yes | yes | yes | |
| sqlite | s3 | 0.495611 | yes | yes | yes | |
| sqlite | s4 | 0.511431 | yes | yes | yes | |
| nbody | s1 | 0.407627 | yes | yes | yes | |
| nbody | s2 | 0.404340 | yes | yes | no | line 301, `text: str @ ...` as `text = ...`, a cell never written again: meaning kept |
| nbody | s3 | 0.395611 | yes | yes | yes | |
| nbody | s4 | 0.394576 | yes | yes | no | the same line 301 |
| bpe | s1 | 0.412573 | yes | yes | yes | |
| bpe | s2 | 0.418254 | yes | yes | yes | |
| bpe | s3 | 0.399593 | yes | yes | no | appended a four-line `main` printing `size(empty())`: an addition |
| bpe | s4 | 0.373007 | yes | yes | yes | |
| strings | s1 | 0.378062 | yes | yes | yes | |
| strings | s2 | 0.396800 | yes | yes | yes | |
| strings | s3 | 0.413674 | yes | yes | yes | |
| strings | s4 | 0.384545 | yes | yes | yes | |

**20 of 20 check clean, 20 of 20 restore both damaged lines, the hidden one
included, 14 of 20 give back the original byte for byte, and no silent wrong
edit.** Every report says the hidden line was found by reading the program
against the specification, after the compiler stopped at the first (for
instance `examples_nbody_main-s2`: *No message led to this edit; I found it by
reading*). The two added `main`s answer the specification's *the file you
compile holds `function main()`* on three files that are modules (`sqlite`,
`bpe`, `strings`), which `check` accepts without one. **Part 2 cost 8.323163
USD**, the pricing session included. **Both parts: 9.924825 USD** of the 26
approved.

## 5. What this does not say

- One model, `claude-opus-5-5`, the CLI's default on this day; panel 183
  recorded `claude-opus-5`. Thirty sessions: no failure in 30 puts the
  rate of a first answer leaving the hidden mistake under about 1 in 10 at 95
  per cent (the rule of three, arithmetic and not a run), and part 2's 20
  alone under about 1 in 7.
- Part 1 scored the sitting's output, not today's (§ 2).
- Part 2's hidden mistakes are a later stage's, not the parse stage's, because
  the plan holds no parse-stage (b) row at that size (§ 2, § 4); each is a
  habit another language writes (`False`, `double`, `boolean`, `??`), which a
  reader of the whole file meets on its own line.
- Each mutant holds two mistakes; panel 183's critic's form, not a program
  with many.
- Part 11's metric 4 is still unrun; this is not it.

## 6. What the numbers show for D1's stronger promise

D1's last clause enters the promise *where it [metric 4] measures a hidden
mistake of the parse stage costing one* [exchange]. Nothing here measured one
costing an exchange: where a parse-stage mistake was hidden (the sitting's
output, 5 and 7 lines), 10 of 10 first answers repaired it; where a mistake
was hidden at the project's sizes (315 to 433 lines, a later stage's), 20 of
20 did. And the class the clause would bind is nearly empty on today's
compiler: both of prediction 2's rows are told since defects 200 and 201, and
the frozen plan's normal arm hides 1 parse-stage second of 14,776, none in a
file of 300 lines or more. So the numbers do not supply the condition D1
names for the stronger promise; they would make it cheap to state, since
today's compiler nearly meets it on the plan, and they give it no measured
turn to pay for. Whether it enters is the sitting's.
