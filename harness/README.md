# Measurement harness (design.md Part 11)

"It is what separates this project from an opinion." Everything here is frozen
and hashed; every run records provenance (spec sha, compiler sha, model id,
prompt sha, suite sha) into `docs/measurements/NNN.md`.

## Layout

- `tasks/` — the frozen task suite for metric 2 (first-try rate). Target: 20
  author-confirmed tasks. Assistant-drafted tasks are marked
  `# UNVERIFIED — author must confirm` until reviewed.
- `mutations/operators.md` — metric 3's mutation operators, as data. Applied
  mechanically to the golden corpus; per-operator kill rate reported. No API
  needed.
- `prompts/first-try.md` — the frozen prompt template for metric 2. Single
  turn, **spec-only context** (never design.md, never this repo — the measured
  model co-designed the language).

## Protocol (metric 2)

20 tasks × 5 samples per arm, Wilson intervals. Two arms per trial:
`heroes check` and `heroes check --permissive` (the control — same compiler,
thesis-bearing checks off). Two gradings once available: compile rate (M3d+),
tests-pass rate (M6+). Cap turns-to-green at 5 (metric 4).

## The v0 baseline (pending — needs the author)

The pre-amendment baseline must be taken against spec v0 (commit `d10fbec`)
BEFORE panels 002/003/005/006 amend the spec. No API key is configured on this
machine, so either: configure `ANTHROPIC_API_KEY` (or `ant auth login`) and
run the trials API-side, or run them by hand in fresh Claude sessions pasting
ONLY `spec/heroes-spec.md` + one task, recording results in
`docs/measurements/000-baseline.md`. Grading for the baseline is by-hand
review (the compiler doesn't exist yet): does the program conform to the spec?
