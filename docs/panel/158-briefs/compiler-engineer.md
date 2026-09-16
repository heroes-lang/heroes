# Panel 158 — compiler-engineer brief

Read `docs/panel/158-briefs/00-shared.md` first. You judge the ceiling
(design.md §1.1, §1.7) and implementation cost, and you have a veto on soundness.

## What only you can price

**Option 3 is the one §1.7 favours and nobody has costed it.** Deleting the
parse refusal at `selfhost/parse/type.hero:51-58` REMOVES a special case, which
is the direction that section prefers. Price it: how many lines come out, what
does the parser do with `T???`, and does anything downstream assume a fallible's
payload is never fallible? Grep for that assumption rather than trusting it —
`selfhost/check/`, `selfhost/ir/`, `selfhost/emit/` each have a fallible walk.

**Option 1 was priced at ~30 lines and closes one door of three.** Re-measure
it: the refusal of `{K: V?}` at the declaration. Which door does it leave open,
and is a partial closure worse than none — a reader who learns `{K: V?}` is
refused may conclude the nesting cannot happen.

**The fifth option.** Ask what would have to be true for a route nobody listed
to exist. One shape the coordinator noticed and did not price, endorsed by
nobody: the checker could REFUSE at the point a fallible is wrapped in another
— the `find` call, the `m[k]` read — rather than at a declaration or a written
type, which is where the value is actually produced.

Also measure: the IR prints `i64??` and the diagnostic prints `i64??`. Is there
one renderer or two, and does a repair have one place to change or several?

Your verdict owes R1-R4, measured costs, a falsifiable prediction with its
instrument, your condition, what you left UNRUN, and whether you cast your veto.
Write to `docs/panel/158-reports/compiler-engineer.md` first.
