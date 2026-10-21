# 050: the trial asked again with every model named: level by turn three through the API and through `claude -p`, and without the specification no model wrote a program that ran right

Measured 2026-10-21 from 06:49 by the session holding M-thesis-harness, its
step 11, the trial of issue 2155 asked again after measurement 049 was found
to hold Haiku 5.5's answers alone at turn 1 (defect 2176). Three folders of
`tests/harness/thesis.hero`'s runs: `spec` and `none` from commit `10e27a55b`
in the lane `thesis`, the `harness` rows from `384014c63` in the worktree
`thesis-fix`, defects 2178 to 2180 repaired in it, their folder moved into the
lane's `build/thesis-runs/` beside the others once done; on the author's budget of
$80 (2026-10-21: $40, then $40 more), each capped by `--max-dollars`: `spec`
at $25, `none` at $10, then $26, then $36 (§ 4), and the `harness` rows
through `claude -p` 2.1.304 at $20. Every number is a command's, counted
from the run folders and the reports the `report` word wrote over them with
the instrument at `384014c63`. **The ten tasks are the assistant's and enter
no published number** (`docs/metrics/tasks.md` § Two suites).

## 1. What ran, and what answered

| run | folder | answers kept | requests sent, by the tallies printed | retries | lost | verdicts | answers from another model | spent |
|---|---|---|---|---|---|---|---|---|
| `spec`, 5 turns | `00fb571c-v3` | 186 | 186 | 0 | 0 | 270 | 0 | $4.24 |
| `none`, 5 turns | `00fb571c-v3-none` | 994 | 634 in three of four invocations | 0 | 0 | 270 | 0 | $34.90 |
| `harness`, 5 turns | `00fb571c-v4-claude` | 212 | 181 in the second of two | 0 | 0 | 270 | 0 | $4.40 |

**The retry rate**, unmeasured in 049: no request of any tally was sent
twice. An invocation ended by the exit of the Claude Code process that had
started it printed no tally (one of `none`'s, the first of the `harness`
rows'), so its requests are counted by the answers it kept, and a request in
flight at that moment, which may have been paid, by nothing. Every one of the
1,392 answers kept stopped on `end_turn`, and every one's `answered_by` is
the model its request named, which defect 2173's rule holds every run to and
defect 2179's reading now reads right through the harness.

## 2. Under the specification

| model | green@1 strict / brief / control | green@2 | green@3, every arm | green@5 | mean turns to green |
|---|---|---|---|---|---|
| Haiku 5.5 | 16 / 16 / 16 | 27 | 28 | 28 | 1.5 |
| Sonnet 5.5 | 18 / 18 / 20 | 29 | 29 | 29 | 1.4 strict, 1.3 control |
| Opus 5.5 | 23 / 23 / 24 | 29 strict, 30 control | 30 | 30 | 1.3 strict, 1.2 control |

**The primary, green@3 strict minus control on Sonnet 5.5, reads +0.0pp: 29
and 29, no discordant pair.** The strict arm lost 3 pairs at turn 1 (Sonnet
5.5 2, Opus 5.5 1), 1 at turn 2 (Opus 5.5), none at turn 3, and won none at
any turn. Silent@3 is 1 against 1 for Haiku 5.5 and for Sonnet 5.5 and 0 for
Opus 5.5: the same mistakes in every arm, which no rule names.

**What refused the first tries**, the 90 turn-1 answers graded again in the
strict arm: Haiku 5.5 13 of 30, 9 a `slice` with nothing to slice and nothing
else, 2 `repeat`'s count; Sonnet 5.5 11 of 30, 6 `repeat`'s count and nothing
else, none `slice`; Opus 5.5 7 of 30, 5 `repeat`'s count and nothing else, none
`slice` (issue 2174). Codes once per verdict, strict arm: `type_mismatch` 13,
`builtin_shape` 9, `wrong_label` 9, `expected_arm_arrow` 3, `bad_operand` 3,
`builtin_name_taken` 2, `expected_args_close` 1, `needs_label` 1,
`cannot_infer` 1; the control arm the same less `wrong_label`,
`builtin_name_taken` and `needs_label`.

## 3. What the protocol learns

- **Output tokens per answer at turn 1**: Haiku 5.5 median 8,192, max 12,447;
  Sonnet 5.5 median 3,063, max 4,008; Opus 5.5 median 2,577, max 3,183. Later
  turns: medians 1,357, 970 and 767, max 4,847.
- **Difficulty**: green@1 strict per task, median task: Haiku 5.5 0.5, Sonnet
  5.5 0.67 (the target's 0.3 to 0.7, at its top), Opus 5.5 1.0.
- **Correlation within a task**, green@1 strict, one-way ICC: Haiku 5.5
  -0.180, Sonnet 5.5 0.614, Opus 5.5 0.477, pooled 0.295.
- **Extraction**: every turn-1 answer under `spec` held one fenced block.
- **The cache**: one prompt of 10,875 tokens at the median for each of the
  three models; Haiku 5.5 wrote it once and read it 29 times, Sonnet 5.5 wrote
  it twice, Opus 5.5 four times.
- **Cost under `spec`**: turn 1 $2.91 (Haiku 5.5 $0.12, Sonnet 5.5 $1.02, Opus
  5.5 $1.77), later turns $1.33.

## 4. Without the specification

The same thirty conversations a model, without the specification: the
template's opening that names no language detail and its sentence *No
specification of the language is given; write the program from what you
know of it.* 994 answers kept, 90 at turn 1 and 904 later, every one the
model its request named and every one stopping on `end_turn`. Asked from
`10e27a55b` in four invocations: from 07:44 to its $10 cap at 08:37 (104
verdicts); resumed at 08:44 and ended at 10:33 by the exit of the Claude Code
process that had started it (202), printing no tally; resumed at 10:34, in a
session of its own, to the next cap of $26 at 10:45 (213); resumed at 10:46
under a cap of $36, priced from the folder's own answers, and done at 11:33
(270). The three tallies printed read 384, 41 and 209 requests sent with no
attempt beside them (defect 2178's count corrected for the first two).

**No model wrote a program that ran right: 0 green of 270, every model and
every arm**, Wilson's interval on each cell of thirty 0.0 to 11.4%.

| model | turn 1: no `function main` / refused | last turn: refused / `extern` / ran wrong | turn-1 answers saying the language is unknown | turn-1 output, median |
|---|---|---|---|---|
| Haiku 5.5 | 29 / 1 | 71 / 19 / 0 | 0 | 1,143 |
| Sonnet 5.5 | 30 / 0 | 87 / 2 / 1 | 0 | 988 |
| Opus 5.5 | 26 / 4 | 86 / 4 / 0 | 5 | 1,154 |

- **The `format_slip` at turn 1 is the language unknown, not a format
  slipped.** The extraction rule asks for a fenced block holding `function
  main`, Heroes' spelling, and the answers spelled it otherwise: Sonnet 5.5
  `def main` 26 times of 30; Opus 5.5 `fn` 11, `func` 8, `def` 3, `proc` 1,
  no `main` 3; Haiku 5.5 `func` 10, `fn` 6, `def` 1, `fun` 1, no `main` 11.
  Of the five that wrote `function main`, every one reached the compiler and
  was refused. From turn 2 the template's sentence names `function main`;
  after it Haiku 5.5's later turns still missed it 10 times and held no
  program 3 times, counted once per arm, and Sonnet 5.5's and Opus 5.5's never.
- **Five of Opus 5.5's thirty turn-1 answers say in their prose that the
  language is unknown to them**, before a guess they offer as such; no answer
  of Haiku 5.5's or Sonnet 5.5's says so.
- **What the last turns met**: the parser's and the names' first refusals
  (`expected_end_of_line`, `reserved_word`, `missing_body` and `unknown_name`
  among the five codes most frequent in every model's strict arm), and `extern`
  groups the models wrote to reach C, 25 conversations ending on
  `refused_extern`, 19 of them Haiku 5.5's.
- **One program ran wrong, in the control arm alone.** Sonnet 5.5's second
  sample of task `18e95dc7` compiled at turn 5 under `--permissive` and left
  exit 0 with output other than the case's; the strict and brief arms of the
  same generation were still refused at turn 5 (`not_mutable`,
  `expected_end_of_line`). It is the one silent failure of the 270, past
  silent@3's window, and one sample is an anecdote, not a rate.
- **Cost**: $34.90, against `spec`'s $4.24 for the same cells: five turns of
  growing history in every conversation, where under the specification most
  were green at the first or second.

## 5. Through `claude -p`

Through `claude` 2.1.304 (`claude -p --bare --tools ""`, panel 226's route C,
a table of its own and never pooled with the API's), from `384014c63`, the
repairs of defects 2178 to 2180 in it: 212 answers kept, 90 at turn 1 and
122 later, every one `claude-haiku-5-5`, `claude-sonnet-5-5` or
`claude-opus-5-5` as its request named (defect 2179's reading, the session's
own title request turned off), every one stopping on `end_turn`, none lost.
Asked in two invocations: the first, from 10:23, ended at 10:33 with 33
verdicts when the Claude Code process that had started it exited, and printed
no tally; the second, resumed into the folder under its protocol (defect
2180's `resumed.txt`), sent 181 requests in 181 attempts.

| model | green@1 strict / brief / control | green@3, every arm | green@cap | silent | loud | mean turns to green, strict |
|---|---|---|---|---|---|---|
| Haiku 5.5 | 15 / 15 / 16 | 30 | 30 | 0 | 0 | 1.6 |
| Sonnet 5.5 | 12 / 12 / 17 | 30 | 30 | 0 | 0 | 1.7 |
| Opus 5.5 | 24 / 24 / 27 | 30 | 30 | 0 | 0 | 1.2 |

**Every one of the 270 samples was green by turn 3**, in every arm, so the
primary reads +0.0pp on every model with no discordant pair, as under the
API. Set beside the API's rows pair by pair, the same task and sample: green
by turn 3 in the harness and not through the API, Haiku 5.5 2 and Sonnet 5.5
1 in each arm; the other way, none.

**Sonnet 5.5's first tries are the difference, and they are not the same
model's at the same effort.** Its green@1 strict is 12 through the harness
against 18 through the API, its median task 0.33 against 0.67, and its
turn-1 answers a median of 1,339 tokens out against 3,063. The harness's
sessions record what it sent (`api-request` entries): `output_config`
`{"effort": "medium"}`, `max_tokens` 128,000 and adaptive thinking on every
request, Haiku 5.5, Sonnet 5.5 and Opus 5.5 alike; the API rows sent no
effort and ran at the API's default, `medium` on Haiku 5.5 and Opus 5.5 and
`high` on Sonnet 5.5 (the models overview as panel 226's historian read it).
So Haiku 5.5's and Opus 5.5's rows differ by transport alone and Sonnet
5.5's by transport and effort; both folders' provenance said `"effort": ""`,
which is defect 2181, repaired at `8e9793d9d`: every request now carries its
effort written out and the two transports run at the same one.

- **Prompt**: 10,987 tokens at the median against the API's 10,875, 112
  more; what the harness adds is its own system text, which its sessions'
  `api-request-shape` entries hold whole: one sentence naming the Agent SDK,
  the working folder and the date, and no tool.
- **Turn-1 output**: Haiku 5.5 median 9,217 (max 16,316), Sonnet 5.5 1,339
  (2,619), Opus 5.5 2,345 (3,540).
- **Correlation within a task**, green@1 strict: Haiku 5.5 0.104, Sonnet 5.5
  -0.080, Opus 5.5 0.614, pooled 0.243.
- **Cost**: $4.40, turn 1 $2.90 (Haiku 5.5 $0.15, Sonnet 5.5 $0.75, Opus 5.5
  $2.00), against the API's $4.24 on the same tasks.

## 6. What it means

- **The primary is level, and level is what ten tasks can say.** Green by
  turn 3, strict minus control, reads +0.0pp on Sonnet 5.5 through the API
  and through the harness, with no discordant pair in either, and so does
  every other model's. The strict arm cost first tries (through the API,
  Sonnet 5.5's green@1 18 against the control's 20 and Opus 5.5's 23 against
  24) and had them back by turn 3. Ten tasks of the assistant's own, trapless
  and labelled, are the trial the protocol asked for, not the thesis: they
  say the instrument measures and what a run costs, and the held-out suite,
  every task a trap (decision 2172), is where a difference could show.
- **Nothing failed silently through the harness; through the API one task
  did, in every arm alike.** Silent at turn 3 was 1 against 1 for Haiku 5.5
  and for Sonnet 5.5 through the API, 0 for Opus 5.5, and 0 everywhere
  through the harness: no arm caught what another let through, on these
  tasks.
- **Without the specification no model wrote Heroes.** Through the API, 0 of 270 conversations reached a program that passed its cases, with five turns of the compiler's messages each; the first answers wrote another language's `main` and five of Opus 5.5's said the language was unknown to them. The repository has been public since 2026-09-08, after these models' June 2026 cutoff, and no answer showed Heroes known; so the `spec` rows measure what the specification itself teaches, from nothing a model brought.
- **The transports agree once the effort is the same.** Haiku 5.5 and Opus
  5.5, at one effort through both, reached green by turn 3 on as many samples
  or more through the harness; Sonnet 5.5's lower first tries through the
  harness came with `medium` against the API's `high`, which defect 2181 now
  writes out and holds equal.
- **What the trial cost**: $48.74 for every answer kept since the trial began on 2026-10-20 (this measurement's three folders $43.54, the folders of 049 and before $5.20), and about $0.002 for four probes of `claude -p` 2.1.304, inside the author's $80.
