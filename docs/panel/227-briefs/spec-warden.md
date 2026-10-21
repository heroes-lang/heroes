# Panel 227, spec-warden

Read `docs/panel/227-briefs/00-shared.md` first. Your question is the price and Principle 0's burden
of proof (design.md §1.2, §1.6; CLAUDE.md § 2).

**The counts you are given**, measured 2026-10-21 and written in the shared brief with their
requests and answers kept: route A costs **+13 real tokens, 10,286 → 10,299 in the ledger's unit**
(the wrapper of 6 subtracted as `heroes measure --refresh` does; 10,292 → 10,305 as the API
answered the file whole), `claude-opus-5`, and +10 on each vendored table. You may re-run the
vendored counts (`./heroes-next measure <file>` from the lane, read-only); you have no route to the
API's counter and need none. Route C costs 0. Route B is unpriced: if you judge it, draft it under
`.claude/worktrees/thesis-drafts/227/spec/after-b.md` (inside the root, ignored) and count it with
the vendored tables, saying the real figure is unrun.

**Answer:**

1. Does route A owe a ledger row in `docs/measurements/010-spec-budget-ledger.md`, and under panel
   012's rule (*an addition needs a named removal or a pre-registered falsifiable prediction*) which
   of the two pays for it? **Price it against design.md §1.6's consolidation budget**, 1,500 real
   tokens for the block M-thesis-harness belongs to, 238 spent by the ROADMAP's reading, and say
   whether measurement 050 is *the thesis harness has run* in that paragraph's sense.
2. **If a prediction pays, say where it can be scored out of sample.** The trial's ten tasks are the
   assistant's, they never enter a published number, and the `repeat` evidence is 2 of them
   (`6f236494` 8 refusals, `c7a30e5b` 5) and the `slice` evidence 6 (the shared brief); a prediction
   scored on the next run of those ten tasks is scored in sample. Name an instrument that would see
   the effect, or say none exists before the held-out run, and what the prediction is then.
3. Whether 13 tokens could be found inside § 11 or elsewhere as a removal, and what.
4. Whether the sentence's other entries carry the same fault (a built-in written without what it
   takes, where a reader would copy the call), measured by reading § 11 and the inventory
   (`selfhost/inventory.hero`), not by assumption. **The data already holds two**: 3 Haiku refusals
   used `to_i64(...)` as an `i64` (line 330 says `i64?`; the sentence writes `to_i64` bare), and 2
   answers defined `all` and `ok`, names the sentence lists (the shared brief). Say what each would
   cost to state and whether it should.
5. Panel 120 refused signatures in the specification (*the spec buys prose and not signatures*):
   does route A's `(`n` a `u64`)` meet that refusal, and does route B?

Write your report to `docs/panel/227-reports/spec-warden.md`: verdict (approve / object / veto),
section, cost, a falsifiable prediction, the condition. You hold a veto on a spec token spent
without a ledger row. Write nothing else in the tree; never write outside the repository's root.
