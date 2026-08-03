# Measurement harness — frozen until it can run

Methodology (arms, sample sizes, grading, metrics): **design.md Part 11** —
the single source; this file only points and states what is pending.

**Pending, both on the author (queued in `docs/debrief/QUEUE.md`):**
the v0 baseline (metric 2, n=20, against spec v0 = commit `d10fbec`) needs
`ANTHROPIC_API_KEY` or manual fresh sessions, and must run BEFORE any spec
amendment lands — spec v0 stays frozen until then (DESIGN-LOG). And 15 of
the 20 tasks must be author-written (held out — an assistant-written suite
would measure the assistant's priors).

## Layout

- `prompts/first-try.md` — the frozen prompt template (metric 2):
  single-turn, spec-only context, hashed.
- `mutations/operators.md` — metric 3's operators as data; applied
  mechanically to the golden corpus, per-operator kill rate; no API needed.
- `tasks/` — the frozen task suite (see its README for authorship rules).

Provenance, on every run: spec sha, compiler sha, model id, prompt sha,
suite sha, recorded into `docs/measurements/NNN.md` (directory born with the
first run). Never diff runs with different compiler shas unless flagged.
