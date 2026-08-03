---
name: step
description: Run one development step of the Heroes compiler with the full teaching protocol — uncued author prediction committed first, author-first diagnosis, explain-it-back, spaced questions, commit. Use for every implementation step; a step is not closed without all of it.
---

# /step <milestone.step> — the teaching protocol

The author is learning compiler construction; the assistant writes most code.
Learning therefore comes from **predicting, testing, and debugging** (design.md
Part 0), and this skill is the mechanism. Do the phases in order; never skip a
gate.

## Phase 1 — Goal (assistant)
1. Session bookends first: `git log --oneline -10`, DESIGN-LOG tail, last
   journal entry. Read `spec/heroes-spec.md` in full.
2. Create `docs/journal/NNN-<slug>.md` with ONLY section 1: what this step
   builds, why (with design.md § citations), and a concrete input example.
   **Do NOT describe or hint at the expected output. Not its shape, not its
   size, not an analogy.** The prediction must be uncued.

## Phase 2 — Prediction gate (author) — HARD STOP
3. Create `docs/journal/NNN-prediction.md` containing only the input example
   and blank prompts for a countable, falsifiable prediction (exact token
   list, number of basic blocks, exact C names — "roughly X" is not a
   prediction).
4. **STOP. Tell the author to fill it in and commit it.** Do not proceed, do
   not touch `crates/`, until `git log` shows the prediction commit. If the
   author explicitly waives the prediction for this step, record the waiver
   in the journal — their call, but it must be written.

## Phase 3 — Implement (assistant + author)
5. Implement. If this milestone's author-written function has not been done
   yet, hand it to the author now (a small, real one: `is_digit`, the
   precedence table, one `emit_*` case) and review their code.
6. Ask the author for their adversarial golden cases if this milestone still
   needs them (5 per milestone). Assistant-written bulk cases are labelled.
7. Run everything: `cargo test`, clippy, and from M5 the double-emit
   determinism diff and ASan goldens.

## Phase 4 — On failure: author-first diagnosis — HARD STOP
8. Post the raw symptom only (the golden diff, the clang error, the panic).
   **STOP until the author writes a hypothesis and names the file.** Then
   explain what actually broke — before fixing it — and fix.

## Phase 5 — Close (author + assistant)
9. Complete the journal: §3 what diverged from the prediction (diff it
   against NNN-prediction.md — this is the lesson), §4 what broke + the
   author's diagnosis first, §5 the author's explain-it-back (≤10 lines,
   written from memory, files closed; then diff their account against the
   code and list omissions).
10. Ask the author 3 spaced questions from journals ≥2 steps back, from
    memory. Record answers.
11. If a prediction missed badly, write/update the relevant
    `docs/theory/<concept>.md` note (that is the trigger for theory notes).
12. Append the DESIGN-LOG line if any decision was made. Commit:
    `M<n> step <k>: <what> (docs/journal/NNN)`. If this closes a milestone:
    run the mutation drill (inject one bug in committed compiler code; the
    author diagnoses from the failing golden alone), then the tag exit-quiz
    (author re-implements one small function on a throwaway branch), then tag.
