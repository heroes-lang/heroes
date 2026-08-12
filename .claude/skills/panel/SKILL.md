---
name: panel
description: Convene the Heroes design panel on a proposal. Two lanes: the SOUNDNESS lane (compiler-engineer + ffi-pragmatist) for a change with no surface, no diagnostic and no spec token; the full five judges with differentiated inputs and falsifiable predictions for everything else. Mandatory before changing the language (spec/, design.md Parts 1-11, surface syntax/semantics, a diagnostic class, architecture). Never blocks — adopts a conservative provisional default and queues the author's ratification. Writes docs/panel/NNN and a DESIGN-LOG line.
---

# /panel <proposal> — the design panel

Five judges, **differentiated by input, not by instruction** — that is what
makes their verdicts carry information instead of correlated opinion.

## Two lanes, and picking the wrong one is the failure this section exists for

**The full panel is expensive**: five judges, every one of them compiling. Panel
037 cost **thirty minutes of wall clock and four hand-built runtimes** for a
question two judges answered identically, and the author stopped it mid-flight.
The instrument was right and the gear was wrong.

**Soundness lane** — `compiler-engineer` and `ffi-pragmatist` only. Use it when
the proposal changes **no surface, no diagnostic and no spec token**: a runtime
primitive, an emitter mechanism, an internal representation. Those two are the
judges who compile, and a question with no reader-facing half has nothing for the
other three to be differentiated *about*. The synthesis is written the same way,
records which lane ran, and says what the lane gave up.

**Full panel** — everything else, and the trigger list in CLAUDE.md §4 is
unchanged. Surface syntax, a diagnostic class, anything the spec must state,
anything a reader of the spec could get wrong.

**When in doubt take the full panel.** The lane's cost is real: panel 037 ran
full, and the llm-ergonomist — reading only the spec — produced the session's
other finding, that the document's single cost sentence implies the quadratic
answer and a reader overrides it on a hunch. The soundness lane would have lost
that. A question that *looks* internal but has a sentence in the spec behind it
is a full panel.

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
     `/unblock` or whenever), with follow-up work if they overturn it.
   - every judge's `prediction` is copied into a "Predictions to score"
     section with the milestone at which each becomes checkable. When the
     harness next runs, score them and append the results — judges accrue
     track records.

5. Append the DESIGN-LOG line. Commit the panel file (and, separately, any
   resulting spec/design.md amendment, citing `docs/panel/NNN`).
