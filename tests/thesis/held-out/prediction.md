# The pre-registration of the first held-out run

Written at the freeze, 2026-10-24, in the commit that adds the sealed suite beside this file
(M-thesis-harness step 12). It states, before the run, what the run measures, what each outcome
means, the numbers expected, how many tasks, how they are adjudicated, and, in the fenced
`preregistered` block at the end, the protocol and the digests the run will be held to: `report
<out> --labels tests/thesis/held-out` sets every run's `provenance.json` against that block field
by field, each difference a row of its deviations (`tests/harness/thesis_report.hero`). The rules
it rests on are panel 226 (R1 to R13, ratified 2026-10-21 with R8 amended, issue 2159) and panel
227 (ratified 2026-10-22, issue 2187); the four choices the freeze owed are the author's, given
2026-10-24 on the coordinator's recommendations, *1a 2a 3a 4a*.

## The suite

- **40 held-out tasks**, `suite.enc`, digest `c24a5b813f6bba6da2f8ca28f63aa15f76a790be5e012e74468f6924f7ad2274`,
  numbered 01 to 40, label `held-out`, a canary in each; **20 in reserve**, `reserve.enc`, digest
  `b27ea1a9949cd555646f816f73ac5473276156eea7bd5556eeb019a700f77dd4`, numbered 41 to 60
  (`hashes.txt`, `hashes-reserve.txt`, each what `tasks --sealed` prints of its archive less the
  archive's own line, as the CI's `thesis-attest` compares them).
- Written by the author, every one a trap (decision 2172): `traps.txt` lists all 60;
  `constructs.txt` gives each its classes, the suite holding records in 8 tasks, a variant in 10,
  propagation in 9, an `@` parameter in 5, a map in 6 and a function value in 6, each at or above
  `docs/metrics/tasks.md`'s floor.
- No model has been given a held-out task, as the author reports: they were written in a folder no
  session of this repository may read, their references and mutants graded by `dry` alone, and
  sealed (`docs/metrics/writing-a-held-out-task.md`).

## What is measured: one primary

**`green@3`(strict, full message) − `green@3`(control) on `claude-sonnet-5-5` in the `spec`
context**, over the 40 tasks at three samples each (panel 226's R2). `green@k` is *the hidden cases
passed by turn k under the compiler's feedback*. The interval is **the cluster bootstrap over
tasks**, the task the cluster and its three samples never nine independent pairs, 2,000 resamples,
the seed in the run's provenance; the test is **McNemar's mid-p over the discordant pairs**, the
exact conditional p reported beside it and never as the primary's. **The smallest effect of
interest is 10 percentage points**: a confirmed difference under it is reported as confirmed and
small, never as success. Everything else is exploratory and says so where it is reported.

**The first named secondary** is the silent rate at turn 3, control minus strict, with its own
interval: a program that exits 0 with a wrong output on a hidden case, the failure the thesis is
about. The primary is also reported per construct class and class-weighted beside the flat
aggregate (`constructs.txt`).

## The three outcomes, named before the run

- **Confirmed**: the bootstrap interval's lower bound is above 0.
- **Falsified**: its upper bound is below 0.
- **Inconclusive**: otherwise. **Rule, the author's choice (1a)**: an inconclusive result is
  published as inconclusive, with its interval as measured. n is not extended once the result is
  seen, since extending a sample after reading it inflates the false positives it then reports;
  the reserve is drawn only to replace a task the canary burns or the adjudication finds
  ambiguous.

## The numbers expected (the author's choice, 2a)

- `green@1`: the strict arm **below** the control by **5 to 15 points**, the price of refusing at
  turn 1, before any feedback.
- `green@3`: the strict arm **above** the control by **at least 10 points**, the smallest effect
  of interest.
- The silent rate at turn 3: the strict arm's **at most half** the control's.

The trial on the assistant's ten tasks (measurement 050) is not evidence for or against these:
those tasks tempt no trap, and its arms were level by turn 3 with no discordant pair of 90.

## n

**Fixed: 40 tasks, 3 samples each, per model and arm** (panel 226's R8 as the author amended it,
2026-10-21). R8 asked the trial to size n from its discordance; the trial found none, so the power
is declared unknown rather than computed from a number that is not there.

## Panel 227's two predictions, paid for its sentence (ledger row 115)

Scored at this run under the `spec` context, **on the whole suite (the author's choice, 3a)**:
ledger row 115 names the non-trap stratum, and decision 2172 leaves none, every held-out task being
a trap. **Declared**: the tasks were written by an author who had read panel 227 after its
sentence landed, so a task tempting `repeat`'s count or a `slice` with nothing to slice may stand
among them on purpose, and the rates below may read lower than an unprimed author's tasks would
give.

- **P1** (`repeat`): of the turn-1 answers calling `repeat`, pooled over the models run, at most
  half refused at its count (13 of 13 in measurement 050); Sonnet 5.5 and Opus 5.5 together at most
  a quarter; each count class, a padding count (`N - s.len()`) and a parsed one, scored where it
  holds at least 10 answers, else `lapsed`; the padding class refused at more than twice the parsed
  class's rate sends `len` (an `i64`) to a sitting.
- **P2** (`slice`): of Haiku 5.5's turn-1 answers calling `slice`, at most 10% with nothing to
  slice (10 of 26 in 050); Sonnet 5.5 and Opus 5.5 at most 2%; scored if at least 20 Haiku answers
  call it, else `lapsed`.

## Adjudication

As `docs/metrics/tasks.md` § Adjudication states it, unchanged: a task found ambiguous after the
run (two correct readings of its prose that differ on a hidden case) is replaced by the next
reserve task in order, decided by a reader who has not seen the arms' verdicts, two readers and
their agreement recorded; a sample with no answer after the transport's retries is excluded from
every arm; a sample `check` accepted and `build` refused (`build_failed`) is excluded, refiled as a
compiler defect and reported in a table of its own; nothing is resampled in silence, every
exclusion counted in the report's flow table, attempted to answered to extracted to compiled to ran
to green, per arm.

## The canary

- **On the assistant's ten tasks, the gate before this freeze** (the milestone page's run order;
  the author's choice, 4a), run 2026-10-24 with `canary-probe tests/thesis --live` on
  `claude-haiku-5-5`, `claude-sonnet-5-5`, `claude-opus-5-5` and `claude-fable-5-1`: **44 probes,
  0 completed, 0 lost**: 43 answers said `unknown` and one said nothing
  (`build/thesis-runs/canary-assistant/`, not tracked). Its cost the instrument does not record;
  44 short requests.
- **On the held-out suite, before this run**: a per-task GUID completed exactly burns that task,
  replaced by the next of the reserve in order; the suite GUID completed exactly stops the run and
  the author reads (`tests/thesis/held-out/README.md` § The canary).

## What else the run reports, exploratory

The other models' rows, Haiku 5.5, Opus 5.5 and Fable 5.1 as the ceiling; `green@1` and `green@5`;
Chen's pass@1 on the turn-1 samples and Olausson's pass@t beside `green@k`; tokens to green; the
`none` context, the task alone, as the models' knowledge of the language against their declared
cutoffs; and, on ten tasks, the variant contexts of panel 226's R13 (the specification without its
productions, panel 133's prediction 3). A run of another context writes another `contexts` field
than the block below, which `report` shows as a deviation by construction.

## The protocol and the digests the run is held to

What `preregister --sealed tests/thesis/held-out/suite.enc --model claude-haiku-5-5 --model
claude-sonnet-5-5 --model claude-opus-5-5 --model claude-fable-5-1 --context spec --arm all
--samples 3 --turns 5 --compiler ./heroes-next` printed on 2026-10-24 at `9ac9e9a8b`: the
compiler by the key of its sources, the prompt template, the request layout, the instrument's
digest over `tests/harness`, the suite, the specification the `spec` context sends, the models
and the effort each request carries, the arms, samples, turns and the bounds per case.

```preregistered
{"schema":1,"date":"2026-10-24","head":"9ac9e9a8b6093ae42f8dc0020f469e6365b78aa3","compiler":"./heroes-next","sources":"616f959062622f6a","template":"docs/metrics/first-try.md","template_sha":"888f3b67d143fa93e11219eb032e9f9e02b17a6ae47a4fdccfb955994c3177ae","layout":"cached","layout_sha":"185cc0a2cfa083781ba7f0f335c49e181abba0282d9b988f9fa0900e33cb450a","dialect":"anthropic","transport":"api","harness":"","instrument":"63a9da0bf0f7f6ffc8b38424a9ca7afea6ea5827a049d3d85beab72a7da1a42a","base":"https://api.anthropic.com","suite":"tests/thesis/held-out/suite.enc","suite_sha":"c24a5b813f6bba6da2f8ca28f63aa15f76a790be5e012e74468f6924f7ad2274","suite_label":"held-out","contexts":[{"name":"spec","path":"spec/heroes-spec.md","sha":"4d8e5251e78dee55f8c9a563a0e98e50dfac83b5d00c95cde4703ccd2a3e9cc7"}],"models":["claude-haiku-5-5","claude-sonnet-5-5","claude-opus-5-5","claude-fable-5-1"],"effort":"","efforts":[{"model":"claude-haiku-5-5","effort":"medium"},{"model":"claude-sonnet-5-5","effort":"high"},{"model":"claude-opus-5-5","effort":"medium"},{"model":"claude-fable-5-1","effort":"high"}],"max_tokens":32000,"arms":["strict","brief","control"],"samples":3,"turns":5,"case_seconds":10,"case_bytes":1048576}
```
