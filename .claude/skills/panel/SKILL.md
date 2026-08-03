---
name: panel
description: Convene the Heroes design panel on a proposal — five judges with differentiated inputs and falsifiable predictions. Mandatory before changing the language (spec/, design.md Parts 1-11, surface syntax/semantics, a diagnostic class, architecture). Never blocks — adopts a conservative provisional default and queues the author's ratification. Writes docs/panel/NNN and a DESIGN-LOG line.
---

# /panel <proposal> — the design panel

Five judges, **differentiated by input, not by instruction** — that is what
makes their verdicts carry information instead of correlated opinion.

## Procedure

1. **Number the session**: next NNN in `docs/panel/`. State the proposal in
   ≤10 lines, as a spec diff where possible. If the decision is already taken
   (author's call), mark the session `retro-record` — record real objections,
   do not stage dissent.

2. **Prepare each judge's input** (this is the step that matters):
   - `compiler-engineer` ← the proposal + pointers into `crates/heroes/src/`
     (it must cite files and line counts).
   - `llm-ergonomist` ← ONLY `spec/heroes-spec.md`, the proposal as a spec
     diff, and 1–3 concrete tasks. **Never design.md, never the repo.** Where
     a status-quo-vs-proposal comparison is possible, present the two variants
     label-stripped (blind A/B).
   - `spec-warden` ← the measured token count of the spec before/after (run
     the counter; if only an estimate exists, say so — its verdict will be
     provisional).
   - `ffi-pragmatist` ← the proposal + the task of writing and compiling the
     binding C it implies.
   - `historian` ← the proposal; it must verify precedent via web search.

3. **Run the five as parallel subagents** (one message, five Task calls).

4. **Synthesize** into `docs/panel/NNN-<topic>.md`:
   - the proposal, verbatim
   - the verdict table (verdict · section · cost/delta · prediction ·
     condition, per judge)
   - disagreements, stated plainly — do not smooth them over
   - **the decision is the author's, asynchronously.** The synthesis adopts
     the most conservative resolution, marked `provisional — author
     ratification pending`, records what a veto would compel, and appends an
     item to `docs/debrief/QUEUE.md`. Work proceeds on the provisional
     default; the author's verdict is appended to this file when given (in
     `/debrief` or whenever), with follow-up work if they overturn it.
   - every judge's `prediction` is copied into a "Predictions to score"
     section with the milestone at which each becomes checkable. When the
     harness next runs, score them and append the results — judges accrue
     track records.

5. Append the DESIGN-LOG line. Commit the panel file (and, separately, any
   resulting spec/design.md amendment, citing `docs/panel/NNN`).
