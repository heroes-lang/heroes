# Panel 166 — brief for the spec-warden

**Read `docs/panel/166-briefs/00-shared.md` first**, and
`.claude/rules/spec-shape.md`, which binds you. Every figure in the shared brief
was run.

## The difference between this sitting and the last one

At panel 165 you vetoed on Principle 0 and said the budget was clean **before**
giving the veto, so a ceiling argument could not stand in for a justification
argument. That was the right shape and it is why this brief does not lead with
the number.

**Principle 0 is not the question here.** Two of these routes repair a
**measured memory-corruption defect in shipped code** (063) and a **measured
falsification of two specification sentences** (065). CLAUDE.md § Precedence puts
robustness at rank 3, above token cost. So your question is not *does this earn
its way in* but **which wording is true, and what does the cheapest true wording
cost**.

## The two sentences that are currently false

`spec § 5`: *"`=` binds once, forever"*, and *"only a declared `@` name can be
mutated"*. `spec § 3`: *"Every value behaves as an independent copy. … No
aliasing exists among the values this language owns."*

Measured: a field lent with `f.ptr()` from a binding declared `=` is written by C
and read back changed, 72 → 65, exit 0.

**§ 3 already carries a carve-out for the foreign side** — *"A `ptr` or a `cstr`
is a copied ADDRESS, wherever it sits: two copies reach one foreign thing"* — and
your first job is to say whether that sentence already covers this case or
whether it does not. If it does, defect 065 is a **compiler** defect and not a
document one, and route A or B repairs it with **zero spec tokens**. If it does
not, say what is missing.

## What to price

Draft and price alone, on the REAL instrument:

1. the narrowing that makes § 5 true — a clause naming the `ptr` lend as the one
   way a `=` binding's bytes change;
2. route C's clause, an `extern` parameter declaring which sibling carries the
   extent;
3. route E's grammar change, a named extent `i8[SL_NAME_LEN]`, which is also
   panel 165's queued route 12;
4. route G alone — the hole written down and nothing checked.

Baseline, measured 2026-09-19: vendored **6089**, real **8106** on
`claude-opus-5`, digest `3c065c560426eb07`, ceiling **10240**, FFI floor 60.
**Rebuild `heroes` from the seed before measuring** — a stale binary reports the
DOCUMENT as stale and that cost this project two full-net runs yesterday.

`--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`, and
exits 2 with no `ANTHROPIC_API_KEY`. Either apply a draft to the real path,
refresh and revert, or write your number down as a **lower bound, in those
words**.

## The thing to weigh that is yours alone

Panel 165 found that **`spec § 13` enumerates fields and never parameters** — the
list of what a parameter may be lives in a diagnostic and in design.md §4.19, not
in the document. Routes B and C both add a parameter rule. **Say where it goes**,
and whether the document needs that list at all or can state the rule without it.

## What your verdict must carry

A verdict per route, the token delta for each draft with the instrument named and
whether it is real or a lower bound, the ledger row you would write, and **one
falsifiable prediction**. You hold a veto on a budget breach.
