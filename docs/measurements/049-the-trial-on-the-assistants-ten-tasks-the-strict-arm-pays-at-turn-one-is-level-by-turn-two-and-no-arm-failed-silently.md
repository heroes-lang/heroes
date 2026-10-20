# 049: the trial on the assistant's ten tasks: the strict arm pays at turn one, is level by turn two, and no arm failed silently

Measured 2026-10-20 from 20:28 to 22:23 by the session holding M-thesis-harness,
its step 11, the trial `docs/metrics/tasks.md` asks for before the freeze
(issue 2155). Two paid runs of `tests/harness/thesis.hero` against Anthropic's
API, each asked of the author: the first turn on the author's *push ok, cap 20
dollars* (the trial's turn 1 had been recommended to them as *the first turn
alone: 180 requests, a few dollars*); turns 2 to 5 on *raise it by 20 if
needed* and *ok, spend what is needed*, a total cap of $40 over both. The cap
was kept by the instrument (`--max-dollars`, `tests/harness/thesis_price.hero`,
committed for it at `c75561853`), never reached. Every number below is a
command's, counted that evening from the two run folders under the lane's
`build/thesis-runs/` and the reports the `report` word wrote over them. **The
ten tasks are the assistant's, labelled `assistant`: they prove the instrument
and never enter a published number** (`docs/metrics/tasks.md` § Two suites).

## 1. What ran

| | turn 1 | turns 2 to 5 |
|---|---|---|
| out | `build/thesis-runs/00fb571c` | `build/thesis-runs/00fb571c-k5`, seeded with turn 1's 180 generations and none of its verdicts |
| head | `c75561853` | `52441a985` (defects 2168 and 2170 repaired between) |
| models | `claude-haiku-5-5`, `claude-sonnet-5-5`, `claude-opus-5-5`, effort the provider's default | the same |
| contexts | `spec` (digest `212264059c985c2e`), `none` | `spec` alone (defect 2169, § 4) |
| arms, samples | strict, brief, control; 3 | the same |
| requests | 180, 0 lost | 154, 0 lost; the 90 `spec` generations reused, none asked again |
| verdicts | 540 | 270 |
| wall clock | 20:28:45 to 21:33:36 | 22:02:01 to 22:23:12 |
| spent | $8.85 | $4.07, so $12.92 over both, against the cap of $40 |

Both runs name the same template (`888f3b67`), layout (`cached`, `185cc0a2`),
compiler sources (`79fe404274caacd0`, `./heroes-next`), suite (`00fb571c`) and
`max_tokens` (32,000); their `instrument` digests differ (`6029dcf4` and
`d997da41`), the two repairs being in the modules it covers. Every answer of
the 334 stopped on `end_turn`: none was truncated, refused or lost.

## 2. Turn 1, under the specification

Green at turn 1, of 30 samples (10 tasks, 3 samples):

| model | strict | brief | control |
|---|---|---|---|
| Haiku 5.5 | 13 | 13 | 13 |
| Sonnet 5.5 | 15 | 15 | 18 |
| Opus 5.5 | 14 | 14 | 15 |

**The strict arm lost 4 pairs and won none.** The four, task by number: Sonnet
on 07 (sample 3, `needs_label` twice) and on 10 (sample 1, `needs_label` four
times; sample 2, `builtin_name_taken`), Opus on 10 (sample 2,
`builtin_name_taken`): programs whose output was right, refused for their form.
The report's primary row for Sonnet reads -10.0pp, the cluster bootstrap's 95%
interval -26.7pp to +0.0pp, McNemar mid-p 0.125 (exact 0.250).

**Every other refusal was the control arm's too.** The codes, each counted
once per verdict that carried it, of 90 per arm: strict `builtin_shape` 21,
`wrong_label` 21, `type_mismatch` 16, `bad_operand` 11, `needs_label` 4,
`builtin_name_taken` 3, `expected_arm_arrow` 2, `escape_not_needed` 1,
`shadowed_binding` 1 (brief the same); control `builtin_shape` 23,
`type_mismatch` 16, `bad_operand` 12, `expected_arm_arrow` 2,
`escape_not_needed` 1. The control arm's two more `builtin_shape` are two
samples where the strict arm stopped at a rule the control arm drops
(Haiku 5.5 on 03, `builtin_name_taken`; Sonnet 5.5 on 02, `shadowed_binding`)
and the control arm went on to the type stage's codes, `check` speaking one
stage at a time; both arms refused those two.

**No program failed silently.** Not one sample in any arm compiled, ran to exit
0 and printed a wrong output at turn 1; besides refusals and greens, one
Haiku 5.5 sample timed out on a hidden case of task 07, in all three arms.

## 3. Turns 2 to 5, under the specification

| model | arm | green@1 | green@2 | green@3 | green@5 | mean turns to green |
|---|---|---|---|---|---|---|
| Haiku 5.5 | strict | 13 | 25 | 27 | 28 | 1.7 |
| Haiku 5.5 | brief | 13 | 26 | 28 | 28 | 1.6 |
| Haiku 5.5 | control | 13 | 27 | 28 | 28 | 1.6 |
| Sonnet 5.5 | strict | 15 | 29 | 30 | 30 | 1.5 |
| Sonnet 5.5 | brief | 15 | 28 | 30 | 30 | 1.6 |
| Sonnet 5.5 | control | 18 | 29 | 30 | 30 | 1.4 |
| Opus 5.5 | strict | 14 | 29 | 30 | 30 | 1.6 |
| Opus 5.5 | brief | 14 | 29 | 30 | 30 | 1.6 |
| Opus 5.5 | control | 15 | 30 | 30 | 30 | 1.5 |

**The primary, green@3 strict minus control on Sonnet 5.5 under the
specification, reads +0.0pp: 30 and 30, no discordant pair.** The strict arm's
cost was repaid by turn 2: the discordant pairs, strict only and control only,
were 0 and 4 at turn 1, 0 and 3 by turn 2 (Haiku 2, Opus 1), 0 and 1 by turn 3
(Haiku), 0 and 0 at turn 5. Every code the strict arm met at turn 1 on a sample
that went green later was repaired from its message, `needs_label` and
`builtin_name_taken` among them.

**Silent@3 is 0 in every arm for Sonnet 5.5 and Opus 5.5, and 1 in every arm
for Haiku 5.5**: task 02, sample 1, refused at turn 1 and then printing a wrong
output on a hidden case until its conversation closed, the same mistake in the
three arms, which no rule of the strict arm names. The message's value,
strict minus brief over the pairs a refusal reached, reads +0.0pp for Sonnet
(15 pairs) and Opus (16), -6.3pp for Haiku (16 pairs, one discordant).

## 4. The `none` context

Given the task and no specification, no model wrote a Heroes program: 0 green
of 270 verdicts, 132 `no_program` and 138 `format_slip`; of the 90 answers 46
held one fenced block and 44 none, and no code block held `@=`, `???`,
`.must()` or `fail(code:`. Read whole, one answer a model: Haiku 5.5
wrote C and said it was a C-style guess; Sonnet 5.5 and Opus 5.5 declined and
asked for the specification. **The reading is confounded** (defect 2169): the
`none` prompt keeps the template's opening, *You are given the complete
specification*, and then says no specification is given, so a model that asks
for the promised document is answering the contradiction as much as its
knowledge of the language. The three models declare a training cutoff of June
2026, before this repository was public; that no code block used a Heroes-only
spelling agrees with it and is not, under that prompt, a measurement of it.

## 5. What the protocol learns

- **Extraction.** Every one of the 90 answers under the specification held
  exactly one fenced block, so the pre-registered rule (the last block holding
  `function main`) and the first or the longest pick the same program on
  these tasks.
- **Output tokens, thinking included,** per answer at turn 1 under the
  specification: Haiku 5.5 median 8,222, max 11,987; Sonnet 5.5 median 8,450,
  max 13,943; Opus 5.5 median 8,173, max 15,638. Later turns: medians 1,352,
  1,027 and 873, max 3,623. The 32,000 cap was never approached.
- **The cache.** One request wrote the specification's 10,363 tokens to the
  cache, Haiku 5.5's first; the 89 later requests under the specification, the
  first of Sonnet 5.5 and of Opus 5.5 included, each reported reading the same
  10,363, and the three models' prompt counts were equal in all 30 of the
  (task, sample) cells. That a
  cache entry would be read across models, and that three models would count
  one text alike where the skill's model table gives Haiku 5.5 a tokenizer
  counting about 30% more, are a question this measurement does not settle.
- **The difficulty target is met and does not prevent a ceiling.** Green@1 in
  the strict arm per task, of 3: Sonnet 5.5 `3 2 2 3 1 1 0 1 1 1` (tasks 01 to
  10), median task 0.33; Haiku 5.5 median 0.5; Opus 5.5 median 0.5, inside the
  target `docs/metrics/tasks.md` sets (0.3 to 0.7 on Sonnet 5.5). Green@3 is
  30 of 30 for Sonnet 5.5 and Opus 5.5 in every arm.
- **Correlation within a task**, green@1 in the strict arm, one-way ICC over
  tasks with 3 samples each: Haiku 5.5 0.493, Sonnet 5.5 0.104, Opus 5.5
  0.100, pooled over the 30 task-model groups 0.209.
- **Discordance**: 4 of 90 pairs at turn 1, 0 at turn 5.
- **Retries**: no request was lost past its retries; how many requests were
  retried before answering the instrument does not count, so it is unrun.
- **Cost per request**, from the usage the API reported at the prices of
  `thesis_price.hero`: turn 1 under the specification Haiku 5.5 $0.0041,
  Sonnet 5.5 $0.0875, Opus 5.5 $0.1701; a later turn $0.0017, $0.0298 and
  $0.0529; under `none` $0.0007, $0.0093, $0.0232.

## 6. What the instrument met for the first time

Four defects, each found by the trial's own data and filed the same evening:
2168 (a module doc's citation that turned all four CI legs of push `9142b99c3`
red on `spec/anchors`, repaired at `9e71e4c53`), 2169 (§ 4), 2170 (`report`
refused 540 verdicts as none, repaired at `52441a985`), 2171 (the report's
`tokens in` is the uncached input alone, its `abort` counts timeouts and
crashes, and green@3 of a one-turn run is printed without its cap).

## 7. What it means, and what it does not

**On these ten tasks the primary as pre-registered cannot tell the arms apart.**
Its contrast is green@3, and at green@3 two of the three models are at 30 of
30 in every arm, while the one effect the thesis claims for the strict arm,
a mistake caught at compile time that would otherwise run silently, had no
occasion: the control arm let through no program that failed silently on
Sonnet 5.5 or Opus 5.5. The assistant's suite carries no `traps.txt`, so none
of its tasks was written to tempt such a mistake. Whether the held-out suite's
traps, a harder difficulty target, or another primary answers this is a
decision for the sitting whose resolution these numbers inform (panel 226,
issue 2159), not a finding of this run. What the run does show: the strict
arm's turn-1 price on these models is a refusal for form that one message
repairs, 4 pairs of 90 and none left by turn 3.

## Corrected underneath, 2026-10-20, the same evening

**§ 7's *the control arm let no program through that failed silently on Sonnet
5.5 or Opus 5.5* holds at turn 3 and not at every turn**, read before the
decision issue it would have rested on was filed. Every turn of the 270
verdicts listed by outcome, one turn of Sonnet 5.5 was `compiled_wrong` beside
Haiku 5.5's task 02: task 02, sample 3, the control arm, turn 2. Its turn 1,
the generation every arm shares, was refused by the strict arm for
`shadowed_binding` alone and by the control arm, which drops that rule, for
the type stage's `bad_operand` and `builtin_shape` three times. The control
arm's turn-2 program, graded again by `grade --task tests/thesis/02` in each
arm: **the strict arm refuses it, `shadowed_binding`; the control arm builds it
and it prints a wrong output on all 9 cases**, the visible example among them.
The model was shown that example's output, and its turn 3 passed every case,
as the strict arm's turn 3 did. So the thesis's mechanism ran once in the
trial, a mistake the strict arm names that the control arm runs to a wrong
output at exit 0, and it did not move green@3, because the visible example
showed it: the primary can move only where such a mistake passes the visible
example and fails a hidden case.

## Added underneath, 2026-10-20, later the same evening: the `none` context asked again, its prompt repaired

Defect 2169 repaired at `dca2b6267` (the `none` prompt now opens *You are given
one programming task in a small programming language called Heroes* and makes
no promise of a specification), the `none` context was asked again at turn 1
from that commit: `build/thesis-runs/00fb571c-none2`, 90 requests from 22:53:43
to 23:02:23, 0 lost, $1.20 by the instrument's count under a cap of $10, so
the trial's two contexts and five turns spent $14.12 against the author's $40.
**Every one of the 90 answers held a fenced block, and none was a Heroes
program**: 270 verdicts `format_slip`, no block holding `function main`, and no
block holding `@=`, `???`, `.must()` or `fail(code:`. The entry points the
models wrote instead, by the block's first one: `func main` 31 (Haiku 5.5 9,
Sonnet 5.5 10, Opus 5.5 12), `fn main` 21, `int main` 3, an `import` or a
bare `main` heading most of the rest; the median answer was 1,130 to 1,225
output tokens. So with the contradiction gone the models no longer decline,
and they guess a language they know: this is § 4's reading without its
confound, and it agrees with the June 2026 cutoff the provider declares.

## Corrected underneath, 2026-10-21: every turn-1 answer above was Haiku 5.5's

**Defect 2176.** The instrument sent every turn-1 request with the plan's first
model, `claude-haiku-5-5`, whatever model the run was asking for; a later
turn named the right one. Read back from each answer's `answered_by`: all 180
turn-1 generations of § 1 to § 3, and all 90 of the `none` context asked
again, were answered by `claude-haiku-5-5`, 60 and 30 of them kept under
`claude-sonnet-5-5` and `claude-opus-5-5`; the 154 later turns were answered
by the model each names (Haiku 5.5 59, Sonnet 5.5 46, Opus 5.5 49). The
defect came to light when the `harness` rows were asked again and stopped at
their first Sonnet 5.5 request under defect 2173's rule, *asked
`claude-sonnet-5-5`, answered by `claude-haiku-5-5`*. So, section by section:

- **§ 2** is 90 samples of Haiku 5.5 at turn 1, in three groups of 30 that
  differ by draw and not by model: the 4 discordant pairs, the codes and the
  "-10.0pp" named for Sonnet 5.5 are Haiku 5.5's.
- **§ 3**'s rows named Sonnet 5.5 and Opus 5.5 are those models repairing a
  first answer Haiku 5.5 wrote; their green@3 of 30 of 30 is reached from that
  start, and none of their own first tries was asked.
- **§ 4 and its addition** are Haiku 5.5's alone: Sonnet 5.5 and Opus 5.5
  were never asked without the specification.
- **§ 5**: the output tokens at turn 1, the cache read across models and the
  equal prompt counts are Haiku 5.5's, and the two questions they raised are
  this defect, answered.
- **§ 1's cost** priced Haiku 5.5's answers at the rates of the model named.
  At the rates of the model that answered, the instrument's own table, turn 1
  cost $0.43 (not $8.85), the `none` context asked again $0.06 (not $1.20),
  turns 2 to 5 $4.06 as written, and the trial's API rows $4.56 in all; the
  two answers Haiku 4.5 gave the first `harness` rows (defect 2173) have no
  price in that table.
- **§ 7's reading** holds for Sonnet 5.5 and Opus 5.5 repairing Haiku 5.5's
  first answers and is not a reading of their own first tries. The trial is
  asked again with every model named in its request, its measurement the next
  number.
