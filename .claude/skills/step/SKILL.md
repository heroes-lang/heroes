---
name: step
description: Run one development step of the Heroes compiler with the full teaching protocol — uncued author prediction committed first, author-first diagnosis, explain-it-back, spaced questions, commit. Use for every implementation step; a step is not closed without all of it.
---

# /step <milestone.step> — the teaching protocol

The author is learning compiler construction; the assistant writes most code.
Learning therefore comes from **predicting, testing, and debugging** (design.md
Part 0), and this skill is the mechanism. Do the phases in order; never skip a
gate.

**Pace (author instruction 2026-08-03): gates attach to concepts, not
steps.** Run Phase 2 only when the step introduces a new concept (typically
once per milestone); chain plumbing steps autonomously without stopping.
Phase 5 closes the milestone's journal, not every step's.

## Phase 1 — Goal (assistant)
1. Session bookends first: `git log --oneline -10`, DESIGN-LOG tail, last
   journal entry. Read `spec/heroes-spec.md` in full.
2. Create `docs/journal/NNN-<slug>.md` with ONLY section 1: what this step
   builds, why (with design.md § citations), and a concrete input example.
   **Do NOT describe or hint at the expected output. Not its shape, not its
   size, not an analogy.** The prediction must be uncued.

## Phase 2 — Prediction gate (author) — HARD STOP
3. Ask the author 1–4 closed prediction questions in conversation (a count,
   a choice among structures, an output value — click-or-one-word answers),
   with the input example fully visible in chat. Create
   `docs/journal/NNN-prediction.md` containing the input example and the
   questions only.
4. **STOP for the author's answers.** Transcribe them verbatim into
   `docs/journal/private/NNN-prediction-raw.md` (git-ignored — personal
   performance is never published), seal `NNN-prediction.md` with the
   SHA-256 of the raw record, and commit it. Do not proceed, do not touch
   `crates/`, until `git log` shows that commit. If the author explicitly
   waives the prediction for this step, record the waiver in the journal —
   their call, but it must be written.

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
9. Complete the journal: §3 what diverged from the prediction, written as an
   impersonal lesson — shapes and rules, never scores (compare against the
   private raw record); §4 what broke + the author's one-sentence hypothesis
   first; §5 the author's explain-it-back, dictated in any form with files
   closed — the assistant transcribes, diffs the account against the code,
   and lists omissions.
10. Ask the author 3 spaced questions from journals ≥2 steps back, from
    memory. What needed re-explaining goes to the glossary, not the journal.
11. If a prediction missed or any comprehension friction surfaced (a failed
    spot-check, an author's "spiegami meglio"), write/update the matching
    `docs/glossary/NNN-<concept>.md` entry — numbered in birth order,
    English, canonical analogies, origin cited, never deleted (see
    `docs/glossary/README.md`, which absorbed the old docs/theory notes).
12. Append one **story beat** to `docs/book/beats.md` (1–2 sentences, plain
    language: the surprise, the wrong turn, the small victory — the human
    fact the technical records drop; see `docs/book/README.md`).
13. Append the DESIGN-LOG line if any decision was made. Commit:
    `M<n> step <k>: <what> (docs/journal/NNN)`. If this closes a milestone:
    run the mutation drill (inject one bug in committed compiler code; the
    author diagnoses from the failing golden alone), then the tag exit-quiz
    (author re-implements one small function on a throwaway branch), then tag.
