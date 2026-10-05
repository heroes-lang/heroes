# Measurement harness — frozen until it can run

Methodology (arms, sample sizes, grading, metrics): **design.md Part 11** —
the single source; this file only points and states what is pending.

**Metric 1 is done and offline**: `heroes measure` counts the spec with two
vendored BPE tables (`vendor/tokenizers/`), no API key, and the maximum over the
two is what binds. **The ceiling this line named was 3000 until 2026-09-08**,
which design.md §1.6 had already left behind: it was raised twice on 2026-08-04
and the hard number has been **4096** since. The standing measurement lives in
`docs/ROADMAP.md` § Where we are and the per-amendment cost in
`docs/measurements/010-spec-budget-ledger.md`; frozen v0 measured 1989 against
the 2048 of its own day.

**Metric 2 needs a model, which is not the same as needing an API key**
(panel 011). Minimum viable protocol, in order of preference: a local
open-weights model running Heroes vs C with the same model; or paced
sampling — 2 pre-registered tasks per milestone, ~16 samples by v1. The
held-out tasks must be author-written either way: an assistant-written
suite would measure the assistant's priors.

**Metric 3 is primary but never pooled** (panel 011): per-operator kill
rates in three arms (`check`, `check --permissive`, and a one-time C +
Python transliteration of the golden corpus), plus a meaning-preserving
counter-arm — a language that rejects everything maximises catch rate.
`forget the @ marker` is excluded from any headline: its catch rate is 100%
by construction, so it carries no information.

## Layout

- `prompts/first-try.md` — the frozen prompt template (metric 2):
  single-turn, spec-only context, hashed.
- `mutations/operators.md` — metric 3's operators as data; applied
  mechanically to the golden corpus, per-operator kill rate; no API needed.
- `tasks/` — the frozen task suite (see its README for authorship rules).

Provenance, on every run: spec sha, compiler sha, model id, prompt sha,
suite sha, recorded into `docs/measurements/<NNN>.md` (directory born with the
first run). Never diff runs with different compiler shas unless flagged.
