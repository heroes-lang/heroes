# Panel 230, the llm-ergonomist's report: the blind seat's experiment, as run and scored

Written by the coordinator from the scores, as `blind/README.md` says; no seat wrote it. The
design, the rules and the instruments' digests are `docs/panel/230-briefs/blind/README.md`, committed
in `811e84d39` before the first session, with the two changes written beneath it the day they were
made (`run_one.sh`'s `< /dev/null`, and the second run's rule).

## The runs

- **First run**, 2026-10-26 12:29 to 12:36: 84 sessions, 32 answered (family P 21, family M 11).
  42 were refused by the API from 12:33:38 on, *You have reached your specified API usage limits*, a
  cap on the account and not the author's 12 USD; 10 family M sessions (Sonnet 5.5) were refused by
  its safeguards, category `reasoning_extraction`, on the brief's `choice_points` section. The author
  raised the cap the same day and asked for the lost sessions again.
- **Second run**, after `brief-m.md` lost `choice_points` (its digest `be1935de...`): all 36 family M
  sessions again in their folders, and family P's 27 lost ones; the first run's 11 family M answers
  set aside in `build/b230-out/first/` and not scored. **All 63 answered**: the classifier over every
  `build/b230-out/<n>.jsonl` result event read 48 family P and 36 family M sessions `ok`, none
  refused and none retried.
- **Cost**: the 84 scored transcripts 1.9314 USD, the set-aside first-run ones 1.0160, **2.9475 USD
  in all** of the author's 12 (the result events' `total_cost_usd`, summed); by cell, family P T
  0.0841 and P 0.0899 over 24 sessions each, family M T 0.8863 and P 0.8711 over 18 each.
- **The context**: **84 of 84 sessions** made no call but `Read` of a path inside their own folder
  (`score.py`'s `outside_calls`, 0 for each); every `context` section denies a project rule, a
  contract, a memory, a file list and a commit history (a scan for `CLAUDE`, `memory`, `git`,
  `commit`, `repositor`, `contract`, `worktree`, `heroes-lang`, `panel`, `coordinator` over the 84,
  every one of its 73 matching lines a denial). **Disclosed**: the folder's path, which the system
  prompt names, holds `heroes-lang` and the lane's name `panic-location`, and some sessions said so;
  it stood alike in both variants, and by the rule written before the run a `context` naming the
  system prompt is not a failure. No session is void.

## Scored before the key was read

`python3 -I score.py` (sha256 `4d3fa8b0...`, the digest of the pre-registration), over the 84
folders, wrote `scores.tsv`; the key was joined after the context check, by a reader that asserts
each folder's family and task agree with the key.

### Family P, *fix what stopped* (Haiku 5.5), three sessions a cell

| item | T fixed | T located | P fixed | P located |
|---|---|---|---|---|
| `report` | 3/3 | 3/3 | 3/3 | 3/3 |
| `basket` | 3/3 | 3/3 | 3/3 | 3/3 |
| `rects` | 3/3 | 3/3 | 3/3 | 3/3 |
| `checksum` | 3/3 | 3/3 | 3/3 | 3/3 |
| `histogram` | 3/3 | 3/3 | 3/3 | 3/3 |
| `shipping` | 3/3 | 3/3 | 3/3 | 3/3 |
| `roster` | 3/3 | 3/3 | 3/3 | 3/3 |
| `excerpt` | 3/3 | 3/3 | 3/3 | 3/3 |
| **total** | **24/24** | **24/24** | **24/24** | **24/24** |

**Both variants at the ceiling, on both scores.** By the rule written before the run (*4 or more of
24*), **no difference this sample can show**, and no veto. Read with finding 18 of the critic's
first pass: the smallest pinned model corrected every program from the program itself, whatever
its stderr said, on items whose abort class named several sites; and `shipping`'s header comment
gave its mistake away in both variants (`blind/README.md`, the flaw recorded after session 4).

### Family M, *say that it failed* (Sonnet 5.5), three sessions a cell

A T session read on the lane's `heroes-next`, a P session on the prototype `heroes-m1`, as the
rules say.

| task | T passed | P passed | T silent | P silent | `main -> ()?` in T | in P |
|---|---|---|---|---|---|---|
| `sum` (status stated) | 2/3 | 1/3 | 0 | 0 | 0 | 3 |
| `capital` (default) | 3/3 | 3/3 | 0 | 0 | 0 | 3 |
| `port` (default) | 3/3 | 3/3 | 0 | 0 | 0 | 3 |
| `words` (default) | 3/3 | 3/3 | 0 | 0 | 0 | 3 |
| `date` (default) | 1/3 | 3/3 | 0 | 0 | 0 | 3 |
| `tally` (1 and 2 stated) | 1/3 | 1/3 | 0 | 0 | 0 | 0 |
| **total** | **13/18** | **14/18** | **0 of 30 default, 0 of 24 stated** | **0 and 0** | **0/18** | **15/18** |

- **SILENT: 0 against 0** over the 30 default error cases a variant (and 0 against 0 over the 24
  stated ones). Every program Sonnet 5.5 wrote under today's specification that met an error it was
  told to report ended with a status that was not 0, and so did every one under the proposal's.
  **Issue 1001's silent success did not occur once in 36 programs.**
- **passed: 13 against 14 of 18**, a difference of one, *no difference this sample can show*.
- **`main -> ()?` under today's specification: 0 of 18.** **Panel 159's llm-ergonomist's
  prediction**, *≥1 in 3 first attempts at a fallible `main` writes `-> ()?`*, **is falsified**; the
  spec-warden's W2, *at most 1 of the 18*, holds. Under the proposal's, **15 of 18** declared it,
  every session of the five tasks whose statuses fit M1's one status, and none of `tally`'s, which
  asks 1 and 2 (finding 27's direction, met by the writers rather than missed).
- **Hang: 0.** No timeout in either variant.
- **The failures, each read** (the cases' lines in `work/<n>/<variant>-cases.txt`): three `sum`
  programs (one T, two P) printed a wrong sum on cases 1 and 7 at status 0; one `date` T and one
  `tally` T printed a wrong line on a case that asked status 0; four were refused by the compiler on
  a mistake unrelated to `main`, `needs_label` twice (two `i64` parameters called positionally, one T
  and one P) and `bad_operand` twice (`to_i64` handed a `str`, one T and one P). **None of the
  failures is a status.**

## What the seat says, in its shape

- **verdict**: **object** to M1 on its measurement (it changes nothing the experiment can see:
  silent 0 against 0, passed 13 against 14, and no writer under today's text reached for it);
  **no veto** on any route (nothing proposed is non-local); on the place and the sides, **no
  objection and no support**: family P reads 24 of 24 in both variants.
- **experiment**: the two families above, 84 sessions, 2.9475 USD.
- **argument**: The ceiling is the finding. A model that corrects every stopped program from the
  program itself gains nothing measurable from being told where it stopped, on programs of this
  size; what the place and the sides are owed is §1.12's truth (a report that names the place and
  never forges a line), not a turn saved. And the default issue 1001 fears is not the default this
  model writes: under today's text it ends every reported error at a status that is not 0.
- **prediction**: registered for the next held-out run under a compiler that prints places: the
  share of correction turns that follow an `abort` or a `crash` outcome stays within 2 points of
  052's (the base the spec-warden named and nobody has counted: the author's to count, the run being
  sealed).
- **condition**: a family P run off the ceiling (a weaker model, or programs whose mistake the text
  hides) showing P 4 or more of 24 above T would make the place a measured thesis effect; a family M
  run showing T's silent count 5 or more above P's would pay for M1.

## Corrected 2026-10-26, on the completeness critic's second pass (`critic-reports.md`, findings 1 to 3 and 15)

Written beneath rather than over, so the reading that went past its measurement stays legible.

- **This report is the coordinator's reading of the coordinator's experiment** (finding 15): the
  tables above are the experiment's; the `verdict`, `argument`, `prediction` and `condition` are the
  coordinator's, and the synthesis labels them so. The prediction rests on a base nobody has counted
  (052's `abort` and `crash` outcomes, sealed, the author's to count): it is registered and cannot be
  scored until that count exists.
- **Family M did not measure issue 1001's default** (finding 1). Every task of `blind/tasks-m/` opens
  *The program is run as a command, with the arguments given, by somebody who reads what it prints
  and the status it exits with* (`grep -c 'the status it exits with'`: 1 in each of the six), and
  each "default" task calls the failing case *an error, and the program reports it*. So *0 silent of
  30* reads **"Sonnet 5.5 sets a failing status when told the status is read"**, never "the default
  leads nowhere silent". The sentence above, *Issue 1001's silent success did not occur once in 36
  programs*, is true of these 36 and says nothing of a writer told neither; **the default is
  unmeasured**. M1's rejection stands on Principle 0, not on this table.
- **Panel 159's prediction is unscored, not falsified** (finding 2). It was registered *under the
  current document* of 2026-09-16, which read *the file you compile holds `function main()`.* and
  nothing more; the T variant read panel 159's own R3 sentence, *which takes nothing and produces
  nothing* (landed in `f20f03dce`). So 0 of 18 measures R3's sentence working as a guard, and the
  prediction's condition was never run. What the two arms do carry: **when the shape is offered it
  is taken, 15 of 18**, with no measured effect on correctness (passed 13 against 14).
- **Family P's place was read against another numbering** (finding 3). The program stood inside
  `task.md` under 28 to 31 lines of the task's text, and the Read tool numbers `task.md`'s lines; the
  place named the program's own. **15 of 24 P answers cite the place** (a `line:column` or `at
  <item>.hero` in the answer, recounted); the critic counted **9** that reconcile it against
  `task.md`'s numbering (sessions 5, 7, 15, 19, 21, 50, 62, 68, 69), and **3 conclude it is wrong**:
  session 19, *the reported line does not match the file I was given*; 68 and 69, *line 39 is the
  `function by_age` header and the assert is on line 70* (read in their answers here). So family P's
  ceiling cannot carry *gains nothing from being told where it stopped*: the place was read, and in
  this frame often doubted. A rerun puts each program in a file of its own, numbered from 1.
- **The `choice_points` sections are owed** (finding 3): `blind/README.md` said they would be
  quoted whole here. They stand in `.claude/worktrees/panic-location-scratch/blind230/answers/` (the
  48 family P answers carry them; family M's second run asked none), and go into the sitting's
  evidence whole rather than into this file.
