# Measurement harness — frozen until it can run

Methodology (arms, sample sizes, grading, metrics): **design.md Part 11** —
the single source; this file only points and states what is pending.

**Metric 1 is done and offline**: `heroes measure` counts the spec with two
vendored BPE tables (`vendor/tokenizers/`), no API key. Frozen v0 measures
1989 / 2048; the maximum binds against §1.6's 3000 ceiling.

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
