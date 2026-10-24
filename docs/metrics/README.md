# Measurement harness — frozen until it can run

Methodology (arms, sample sizes, grading, metrics): **design.md Part 11** —
the single source; this file only points and states what is pending.

Until 2026-10-05 this was the top-level `harness/`, one folder per file, and
its name collided with `tests/harness/`, which is the net. It is `docs/metrics/`
since, one collection of one kind (`.claude/rules/records.md` § The top of
`docs/`); a record citing the old paths is followed by the `MOVED` table of
`tests/harness/suite_records.hero`.

**Metric 1 is done and offline**: `heroes measure` counts the spec with two
vendored BPE tables (`vendor/tokenizers/`), no API key, and the maximum over the
two is what binds. **The ceiling this line named was 3000 until 2026-09-08**,
which design.md §1.6 had already left behind: it was raised twice on 2026-08-04
and the hard number has been **4096** since. The standing measurement lives in
`ROADMAP.md` § Where we are and the per-amendment cost in
`docs/measurements/010-spec-budget-ledger.md`; frozen v0 measured 1989 against
the 2048 of its own day.

**Metrics 2 and 4 have one instrument since 2026-10-20**, `tests/harness/thesis.hero`, a
program of the net and no verb of `heroes` (panel 226, provisional pending the author's reading;
M-thesis-harness): `heroes run tests/harness/thesis.hero -- <word>` from the repository's root,
the words `tasks`, `dry`, `grade`, `plan`, `run --live` and `report`, and **nothing is sent to a
model without `--live`**. One generation per (task, sample) judged by three arms, strict, brief and
control, the control built and run with `build --permissive`; from turn 2 the conversations fork
with the same feedback in every arm; `green@k` is *solved by turn k*; the primary is
`green@3`(strict) − `green@3`(control) on Sonnet 5.5, a cluster bootstrap over tasks and McNemar's
mid-p over the discordant pairs. Until that day this paragraph read: *metric 2 needs a model, which
is not the same as needing an API key (panel 011); minimum viable protocol, in order of preference:
a local open-weights model running Heroes vs C with the same model; or paced sampling, 2
pre-registered tasks per milestone, ~16 samples by v1.* What stands from it: **the held-out tasks
must be author-written**, since an assistant-written suite would measure the assistant's priors;
the assistant's tasks in `tests/thesis/` prove the instrument and enter no published number.

**Metric 3 is primary but never pooled** (panel 011): per-operator kill
rates in three arms (`check`, `check --permissive`, and a one-time C +
Python transliteration of the golden corpus), plus a meaning-preserving
counter-arm — a language that rejects everything maximises catch rate.
`forget the @ marker` is excluded from any headline: its catch rate is 100%
by construction, so it carries no information.

## Layout

- `first-try.md` — the frozen prompt template (metrics 2 and 4): its turn-1 shape is
  single-turn and spec-only, and panel 226's R12 amends it before the freeze into the multi-turn,
  `system`-layout template whose sha, with the request layout's sha, names a measurement series.
- `operators.md` — metric 3's operators as data; applied
  mechanically to the golden corpus, per-operator kill rate; no API needed.
- `tasks.md` — the task suites' rules: who writes a task, how, the canary, the adjudication, and
  where the tasks live: the assistant's in clear under `tests/thesis/`, the author's sealed in this
  repository.
- `writing-a-held-out-task.md` — the order of work for whoever writes the held-out tasks, from the
  instrument's `scaffold` to the freeze's `seal`, and the folders no model reads.
- `trap-families.md` — the 27 rules the control arm drops and what it does with each, read from
  its witnesses, so a held-out task's trap is chosen from facts of the compiler.

Provenance, on every run: spec sha, compiler sha and `sources` key, model id with its effort
and declared cutoff, the template's sha and the request layout's sha, suite sha, per request the
id, timestamp and usage, recorded into `docs/measurements/<NNN>.md`. Never diff runs with
different compiler shas, template shas or layout shas unless flagged.
