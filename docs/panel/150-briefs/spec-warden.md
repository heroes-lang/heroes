# Panel 150 — brief for the spec-warden

Read `docs/panel/150-briefs/00-shared.md` first, including its three required
sittings. You judge design.md §1.2's cost formula and §1.6's budget, and
Principle 0's burden of proof. Veto on a budget breach.

## The count, and a debt this sitting inherits

`./heroes measure spec/heroes-spec.md`: **5988** on `cl100k_base`, and the
binding `real` row is **7974** on `claude-opus-5`, against a ceiling of
**10240**.

**There is an unpaid debt from panel 149 and you should rule on it first.** That
sitting adopted a **+4 real** substitution — *"after a result or `@`
out-parameter reaching a handle"* — and it never landed, because
`heroes measure --refresh` needs `ANTHROPIC_API_KEY` and this machine has none;
the instrument refuses a verdict against a stale count. **Does the spec text as
it stands now misdescribe what the compiler does?** Read `spec § 13` against
`selfhost/check/acquiring.hero` and say so plainly. If it does, that is a
correctness debt and not a budget one, and it outranks anything this sitting
adds.

## What to measure, and measure it rather than estimating

`heroes measure <file>` counts any file offline, but `real` prints only for the
spec path, so put each draft at that path inside your own copy. **If you cannot
get a `real` row, say the number is unrun rather than scaling from the vendored
ratio.**

1. **Q1's sentence, if it needs one.** Does the document already say what a `ptr`
   producer owes? `spec § 13`'s last clause reads *"where a group consumes a
   handle type every call handing one back says which it is"* — and a `ptr` is
   not a handle by the document's own definition. Price the smallest text that
   would cover it, merged rather than appended, and price the merged form against
   the appended one as panel 122 asks.
2. **Q2's sentence.** The document says nothing about a fixed array of handles
   behind one mark. Price a sentence that answers it, and price the REFUSAL form
   separately: a refusal owes a design.md Part 6 row naming the program fact that
   would make it wrong, and Part 6 rows are free.
3. **Principle 0.** Does either question enter v1 because the compiler needs it?
   Measure: `grep -rn "record [A-Za-z0-9_]* tag " selfhost` and the `ptr` counts
   in the shared brief. If neither, each needs a measured design.md Part 11
   effect or it waits.

## Predict something falsifiable

One prediction with the command that would settle it.
