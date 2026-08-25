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
   - `compiler-engineer` ← the proposal + pointers into `selfhost/` and
     `runtime/` (it must cite files and line counts). **Never `crates/`**: that
     tree is `archive/bootstrap-rs/`, nothing builds it, and a seat sent there
     measures a compiler that no longer ships. Give it the cheap route in the
     brief too — the seed builds in 3.4 s (`clang -I runtime seed/heroes.c
     runtime/runtime.c -o heroes`), while rebuilding from `selfhost/` is ~20
     minutes and kills a seat on the watchdog, which is what happened to four of
     five at panel 087.
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
   **Tell every judge that builds to build in a copy**, and say it in the brief
   rather than assuming it: `cp -r` the tree to the scratchpad and work there.
   Panel 054's compiler-engineer was asked to *"prototype it far enough that the
   number is real"* and prototyped **in the repository** — five files of a
   half-built `repeat`, which the coordinator then committed and pushed inside an
   unrelated commit, landing a language feature before its own panel had ruled
   (CLAUDE.md §4). Panel 053's engineer had copied the tree unprompted, which is
   how the difference showed. A judge's measurement is worth having and its
   working tree is not.

   **The copy is not sufficient on its own, and this is the second time that has
   been paid for** (author decision 2026-08-15, panel 056's process notes). Say
   **`rm -rf target build` after the copy**: a copied `target/` leaves
   `env!("CARGO_MANIFEST_DIR")` pointing at the **real** repository, so a judge's
   golden run silently measures the tree it was told not to touch. Panel 056's
   compiler-engineer found this in its own first run, discarded it and rebuilt from
   scratch — which is the only reason it is known rather than a wrong number in a
   verdict.

   **And the rule binds the coordinator during a sitting.** In panel 056 the
   coordinator put `-Werror=missing-include-dirs` into `FLAGS` between 23:37 and
   23:45 while judges were measuring, and one of them observed the effect from
   inside its own run. It was reverted before any commit and nothing landed wrong,
   but a rule that binds judges and not the seat that convened them has its hole
   exactly where the most privileged actor stands. The working tree is frozen from
   the moment the briefs go out until the synthesis is written.

4. **Synthesize** into `docs/panel/NNN-<topic>.md`:
   - the proposal, verbatim
   - the verdict table (verdict · section · cost/delta · prediction ·
     condition, per judge)
   - disagreements, stated plainly — do not smooth them over
   - **the decision is the author's, asynchronously.** The synthesis adopts
     the most conservative resolution, marked `provisional — author
     ratification pending`, records what a veto would compel, and appends an
     **open** item to `docs/work/DECIDE.md` naming the sitting as `panel NNN`.
     Work proceeds on the provisional
     default; the author's verdict is appended to this file when given (in
     `/decide` or whenever), with follow-up work if they overturn it.

     **The file and the spelling are both load-bearing, and this line named the
     wrong file until 2026-08-26.** `tests/harness/suite_records.hero`'s
     `verdicts` check reads `docs/work/DECIDE.md` and scans its `- [ ]` lines
     for `panel NNN`: a `Pending` verdict is allowed to stand for as long as the
     author likes *provided an open item names it*, and that pair is the whole
     invariant. This line said `QUEUE.md` — the record — so a sitting that
     obeyed the skill was invisible to the check built to catch it, which is how
     panels 069-079 sat unratified for eight days. Write `panel 091`, not
     `panel 91`; both parse, but the padded form is what the repository uses.
   - every judge's `prediction` is copied into a "Predictions to score"
     section with the milestone at which each becomes checkable. When the
     harness next runs, score them and append the results — judges accrue
     track records.

5. Append the DESIGN-LOG line. Commit the panel file (and, separately, any
   resulting spec/design.md amendment, citing `docs/panel/NNN`).
